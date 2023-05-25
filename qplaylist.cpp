#include <QDebug>
#include "qplaylist.h"
#include <QRandomGenerator64>

QPlaylist::QPlaylist(QWidget *parent)
    : QListWidget{parent}
{
    setAcceptDrops(0);
    setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
    setDefaultDropAction(Qt::TargetMoveAction);
    setDragDropOverwriteMode(0);
    setDropIndicatorShown(0);
    setDragEnabled(0);
}

void QPlaylist::next()
{
    if (_currentPlaying){
        int target_index = this->row(_currentPlaying)+1;
        if (target_index<count()){
            play_track_by_index(target_index);
        }
        else{
            play_track_by_index(0);
        }
    }
    else{
        if (!is_empty()){
            play_track_by_index(0);
        }
    }
}

void QPlaylist::previous()
{
    if (_currentPlaying){
        int target_index = this->row(_currentPlaying)-1;
        if (target_index>=0){
            play_track_by_index(target_index);
        }
        else{
            play_track_by_index(count()-1);
        }
    }
    else{
        if (!is_empty()){
            play_track_by_index(0);
        }
    }
}

void QPlaylist::play_track_by_index(int index)
{
    QPlaylistTrack *item = static_cast<QPlaylistTrack*>(this->item(index));
    QPlaylistItemWidget* w = item->getWidget();
    if (_currentPlaying){
        _currentPlaying->getWidget()->stop();
    }
    _currentPlaying = item;
    w->play();
    emit TrackPlayed(item);

}

void QPlaylist::exclude_track_by_index(int index)
{
    QPlaylistTrack *item = static_cast<QPlaylistTrack*>(this->item(index));
    if (_currentPlaying == item){
        _currentPlaying=nullptr;
        emit TrackStopped();
    }
    delete this->takeItem(index);

}

void QPlaylist::shuffle()
{
    QRandomGenerator64 *gen = QRandomGenerator64::global();
    for (int i = 0; i<this->count(); i++){
        quint64 val = gen->generate()%this->count();

        //qDebug()<<val;
        //auto val = i+1;

        this->model()->moveRow(QModelIndex(), i, QModelIndex(), val);
    }
}

void QPlaylist::edit_enter()
{
    for (int i = 0; i < this->count(); i++){
        QPlaylistTrack *item = static_cast<QPlaylistTrack*>(this->item(i));
        QPlaylistItemWidget* w = item->getWidget();
        if (w)
            w->edit();
    }
    setDragEnabled(1);
    this->now_editing = 1;
}

void QPlaylist::edit_done()
{
    for (int i = 0; i < this->count(); i++){
        QPlaylistTrack *item = static_cast<QPlaylistTrack*>(this->item(i));
        QPlaylistItemWidget* w = item->getWidget();
        if (w)
            w->done();
    }
    setDragEnabled(0);
    this->now_editing = 0;
}

void QPlaylist::clear_tracks()
{
    for (int i = this->count()-1; i>=0; i--){
        exclude_track_by_index(i);
    }
}

void QPlaylist::play_current()
{
    this->currentPlaying()->getWidget()->play();
    emit TrackPlayed(this->currentPlaying());
}

void QPlaylist::pause_current()
{
    this->currentPlaying()->getWidget()->pause();
    emit TrackPaused(this->currentPlaying());
}

void QPlaylist::start_playing()
{
    if (!_currentPlaying){
        if (!is_empty()){
            play_track_by_index(0);
        }
    }
}

void QPlaylist::dragMoveEvent(QDragMoveEvent *event)
{
    auto target = this->row(this->itemAt(event->position().toPoint()));
    auto current = this->currentRow();

    if(
            (target == current + 1)
                or
            (current == this->count() - 1 and target == -1)
        )
        event->ignore();
    else{
        QListWidget::dragMoveEvent(event);
    }
}

