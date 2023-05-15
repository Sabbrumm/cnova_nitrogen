
#ifndef QPLAYLIST_H
#define QPLAYLIST_H


#include <QWidget>
#include <QListWidget>
#include <QDropEvent>
#include "qplaylisttrack.h"

class QPlaylist : public QListWidget
{
    Q_OBJECT


    bool now_editing = 0;
    QPlaylistTrack* _currentPlaying = nullptr;

public:
    explicit QPlaylist(QWidget *parent = nullptr);


    void next();
    void previous();
    void play_track_by_index(int index);
    void exclude_track_by_index(int index);
    void shuffle();
    void push_back(QPlaylistTrack *item);
    bool is_empty(){return this->count()==0;}
    void edit_enter();
    void edit_done();
    bool isEditing(){return now_editing;}

    void play_current();
    void pause_current();

    void start_playing();

    QPlaylistTrack* currentPlaying(){return _currentPlaying;}


protected:
    void dragMoveEvent(QDragMoveEvent *event);

signals:
    void TrackPlayed(QPlaylistTrack* target);
    void TrackPaused(QPlaylistTrack* target);
    void TrackStopped();
public slots:

};

#endif // QPLAYLIST_H
