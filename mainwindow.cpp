#include <QListWidgetItem>
#include "mainwindow.h"
#include "qaudiodevice.h"
#include "ui_mainwindow.h"
#include "qplaylisttrack.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMediaDevices>
#include <QAudioOutput>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QAudioDecoder>
#include <LiquidEqualizerWidget.h>
#include <QMediaMetaData>
#include <QMediaFormat>
#include <QAudioSink>
#include <QRandomGenerator64>
#include <QMessageBox>

void MainWindow::set_play_icon()
{
    ui->playpause_button->setIcon(QIcon(":/icons/button_play.png"));
}

void MainWindow::set_pause_icon()
{
    ui->playpause_button->setIcon(QIcon(":/icons/button_pause.png"));
}

QStringList MainWindow::ask_for_tracks()
{
    QString filters = "MPEG Audio (*.mp3);;Ogg Vorbis (*.ogg);;Windows Wave File (*.wav)";
    QStringList filename = QFileDialog::getOpenFileNames(this, "Открыть файл", "/", filters);
    return filename;
}

QString MainWindow::ask_for_playlist()
{
    QString filters = "Плейлист C-NOVA Nitrogen (*.npl)";
    QString filename = QFileDialog::getOpenFileName(this, "Открыть плейлист", "/", filters);
    return filename;
}

QString MainWindow::ask_for_saving_playlist()
{
    QString filters = "Плейлист C-NOVA Nitrogen (*.npl)";
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить плейлист", "/", filters);
    return filename;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ 
    ui->setupUi(this);
    //ui->cover_label->setVisible(0);
//    ui->tr_list->setAcceptDrops(0);
//    ui->tr_list->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
//    ui->tr_list->setDefaultDropAction(Qt::DropAction::MoveAction);
//    ui->tr_list->setDragDropOverwriteMode(0);
    ui->tr_list->setDragEnabled(0);

    connect(ui->tr_list, SIGNAL(TrackPaused(QPlaylistTrack*)), this, SLOT(do_track_pause(QPlaylistTrack*)));
    connect(ui->tr_list, SIGNAL(TrackPlayed(QPlaylistTrack*)), this, SLOT(do_track_play(QPlaylistTrack*)));
    connect(ui->tr_list, SIGNAL(TrackStopped()), this, SLOT(do_track_stop()));
    set_default_metadata();




    //Определяем плеер
    player = new QMediaPlayer(this);
    connect(player, SIGNAL(errorOccurred(QMediaPlayer::Error,QString)), this, SLOT(player_error(QMediaPlayer::Error,QString)));

    //Ставим в настоящий вывод девайс по умолчанию
    output = new QAudioOutput();
    output->setDevice(QMediaDevices::defaultAudioOutput());


    player->setAudioOutput(output);

    eq_timer = new QTimer();
    eq_timer->setInterval(500);

    connect(eq_timer, SIGNAL(timeout()), this, SLOT(eq_randomize()));



    ui->eq_widget->setEnabled(0);
    eq_normalize();


    //Inherit accent color from palette
    ui->eq_widget->setAccentColor(ui->eq_widget->palette().dark().color());
    ui->eq_widget->setGridVisible(false);
    qDebug()<<ui->eq_widget->getAnimationDuration();

    connect(this->player, SIGNAL(positionChanged(qint64)), this, SLOT(do_timing_change(qint64)));
    connect(this->player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(track_end(QMediaPlayer::MediaStatus)));


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_page_playlist_button_clicked()
{
    ui->pages_widget->setCurrentIndex(1);
    ui->page_playlist_button->setEnabled(0);
    ui->page_player_button->setEnabled(1);
    ui->player_act->setChecked(0);
    ui->playlist_act->setChecked(1);
}


void MainWindow::on_page_player_button_clicked()
{
    ui->pages_widget->setCurrentIndex(0);
    ui->page_playlist_button->setEnabled(1);
    ui->page_player_button->setEnabled(0);
    ui->player_act->setChecked(1);
    ui->playlist_act->setChecked(0);
}


void MainWindow::on_playlist_act_triggered()
{
    if (ui->pages_widget->currentIndex()==1){
        ui->playlist_act->setChecked(1);
    }
    else{
        ui->page_playlist_button->click();
    }
}


void MainWindow::on_player_act_triggered()
{
    if (ui->pages_widget->currentIndex()==0){
        ui->player_act->setChecked(1);
    }
    else{
        ui->page_player_button->click();
    }
}


void MainWindow::on_change_button_clicked()
{
    ui->change_button->setChecked(ui->change_button->isChecked());
    // Вход в режим изменений
    if (ui->change_button->isChecked()){
        ui->change_button->setText("Готово");
        ui->change_button->setIcon(QIcon(":/icons/button_playlist_done.png"));
        ui->tr_list->edit_enter();

    }
    // Выход из режима изменений
    else{
        ui->change_button->setText("Изменить");
        ui->change_button->setIcon(QIcon(":/icons/button_playlist_edit.png"));
        ui->tr_list->edit_done();
    }



}


void MainWindow::on_tr_list_itemDoubleClicked(QListWidgetItem *item)
{

    QPlaylistTrack *pitem = static_cast<QPlaylistTrack*>(item);

    switch (pitem->getWidget()->trackState()){

        case QPlaylistItemWidget::Playing:
            ui->tr_list->pause_current();
            break;
        case QPlaylistItemWidget::Paused:
            ui->tr_list->play_current();
            break;
        case QPlaylistItemWidget::Stopped:
            player_stop();
            ui->tr_list->play_track_by_index(ui->tr_list->row(item));
    }
}


void MainWindow::on_add_button_clicked()
{
    auto filepaths = ask_for_tracks();
    if (filepaths.isEmpty())
        return;
    foreach (QString filepath, filepaths) {
        QFile track(filepath);
        QFileInfo trackfileinfo(track);

        QString title = trackfileinfo.fileName();
        QString path = track.fileName();



        //QString title = QString("Трек ") + QString::number(ui->tr_list->count()+1);
        QPlaylistItemWidget *item_widget = new QPlaylistItemWidget(title);
        if (ui->tr_list->isEditing())
            item_widget->edit();


        QPlaylistTrack *newItem = new QPlaylistTrack(path, title, item_widget, ui->tr_list);


        ui->tr_list->addItem(newItem);
        ui->tr_list->setItemWidget(newItem, item_widget);
        if (this->saved_playlist){
            saved_playlist->addItem(newItem);
            saved_playlist->setItemWidget(newItem, item_widget);
        }
        connect(item_widget, SIGNAL(removeButtonClicked(QPlaylistItemWidget*)), this, SLOT(removeButton_clicked(QPlaylistItemWidget*)));
    }
}


void MainWindow::removeButton_clicked(QPlaylistItemWidget *source)
{

    for (int i = 0; i<this->ui->tr_list->count(); i++){
        if (source == this->ui->tr_list->itemWidget(this->ui->tr_list->item(i))){
            ui->tr_list->exclude_track_by_index(i);
            return;

        }
    }


}


void MainWindow::on_shuffle_button_clicked()
{
    ui->tr_list->shuffle();
}


void MainWindow::on_next_button_clicked()
{
    player_stop();
    ui->tr_list->next();
}


void MainWindow::on_previous_button_clicked()
{
    player_stop();
    ui->tr_list->previous();
}


void MainWindow::on_playpause_button_clicked()
{
    if (ui->tr_list->currentPlaying()){
        switch (ui->tr_list->currentPlaying()->getWidget()->trackState()){
        case QPlaylistItemWidget::Playing:
            ui->tr_list->pause_current();
            break;
        case QPlaylistItemWidget::Paused:
            ui->tr_list->play_current();
            break;
        default:
            break;
        }
    }
    else{
        ui->tr_list->start_playing();
    }
}


void MainWindow::set_metadata(QPlaylistTrack *source)
{
    ui->artist_label->setText("");
    ui->trackname_label->setText(source->getWidget()->title());
    ui->time_slider->setEnabled(1);
}

void MainWindow::set_default_metadata()
{
    ui->artist_label->setText("Абсолютная тишина");
    ui->trackname_label->setText("Ничего не играет");
    ui->time_dur_label->setText("");
    ui->time_pos_label->setText("");
    ui->time_slider->setEnabled(0);
}

void MainWindow::player_stop(){
    player->stop();
    player->setSource(QUrl(""));

}
void MainWindow::player_start(QString filepath){
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
}



void MainWindow::player_pause(){
    player->pause();
}
void MainWindow::player_play(){
    player->play();
}



void MainWindow::do_track_pause(QPlaylistTrack* source){
    qDebug() << source->filename() << " на паузе";

    eq_timer->stop();
    eq_normalize();

    if (player->hasAudio()){
        player_pause();
    }

    set_play_icon();
    set_metadata(source);
}

void MainWindow::do_track_play(QPlaylistTrack* source){
    qDebug() << source->filename() << " играет";

    eq_timer->start();

    set_pause_icon();
    set_metadata(source);

    if (player->hasAudio()){
        player_play();
    }
    else{
        player_start(source->filename());
    }


}

void MainWindow::do_timing_change(qint64 position)
{
    qint64 dur = this->player->duration();


    QString dur_s = QDateTime::fromMSecsSinceEpoch(dur).toString("mm:ss");
    QString pos_s = QDateTime::fromMSecsSinceEpoch(position).toString("mm:ss");

    ui->time_dur_label->setText(dur_s);
    ui->time_pos_label->setText(pos_s);

    ui->time_slider->setMinimum(0);
    ui->time_slider->setMaximum(dur);
    ui->time_slider->setSliderPosition(position);



}

void MainWindow::track_end(QMediaPlayer::MediaStatus status)
{
    if (status==QMediaPlayer::EndOfMedia){
        player_stop();
        do_track_stop();
        if (do_repeat){
            ui->tr_list->next();
            player_stop();
            do_track_stop();
            ui->tr_list->previous();
        }
        else{
            ui->tr_list->next();
        }

    }
}

void MainWindow::do_track_stop()
{
    eq_timer->stop();
    eq_normalize();

    if (player->hasAudio()){
        player_stop();
    }
    qDebug() << "ничего не играет";
    set_play_icon();
    set_default_metadata();
}


void MainWindow::on_time_slider_sliderPressed()
{
    if (ui->tr_list->currentPlaying()){
        player->pause();
        eq_timer->stop();
        eq_normalize();

    }

}


void MainWindow::on_time_slider_sliderMoved(int position)
{
    qint64 dur = ui->time_slider->maximum();
    QString dur_s = QDateTime::fromMSecsSinceEpoch(dur).toString("mm:ss");
    QString pos_s = QDateTime::fromMSecsSinceEpoch(position).toString("mm:ss");
    this->player->setPosition(position);
    ui->time_dur_label->setText(dur_s);
    ui->time_pos_label->setText(pos_s);
}


void MainWindow::on_time_slider_sliderReleased()
{
    if (ui->tr_list->currentPlaying())
        if (ui->tr_list->currentPlaying()->getWidget()->trackState() == QPlaylistItemWidget::Playing)
        {
            player->play();
            eq_timer->start();

        }
}

void MainWindow::eq_randomize()
{
    QVector<double> vector_of_values;
    QRandomGenerator64 *gen = QRandomGenerator64::global();
    for (int i = 0; i<30; i++){
        vector_of_values.push_back(
            (gen->generateDouble()*20)-10
            );
    }

    ui->eq_widget->setBands(vector_of_values);
}

void MainWindow::eq_normalize()
{
    QVector<double> vector_of_values;
    for (int i = 0; i<30; i++)
        vector_of_values.push_back(0.0);

    ui->eq_widget->setBands(vector_of_values);
}

void MainWindow::player_error(QMediaPlayer::Error err, QString what)
{
    ui->tr_list->exclude_track_by_index(ui->tr_list->row(ui->tr_list->currentPlaying()));

    QString msg = "Возникла ошибка:\n"; msg.append(what);
    QMessageBox::critical(this, "Ошибка", msg);

    do_track_stop();




    qDebug()<<what;
}


void MainWindow::on_open_file_act_triggered()
{
    on_add_button_clicked();
}


void MainWindow::on_repeat_clicked()
{
    ui->repeat->setChecked(ui->repeat->isChecked());
    // Выход из репит
    if (!ui->repeat->isChecked()){
        ui->repeat->setIcon(QIcon(":/icons/button_repeat_off.png"));
        do_repeat=0;
        //ui->tr_list->edit_enter();

    }
    // Вход в репит
    else{
        ui->repeat->setIcon(QIcon(":/icons/button_repeat_on.png"));
        do_repeat=1;
        //ui->tr_list->edit_done();
    }
}

void MainWindow::save_current_playlist()
{
    QString filename = ask_for_saving_playlist();
    QString to_write;

    if (!filename.isEmpty()){
        QFile plist(filename);
        plist.open(QIODevice::WriteOnly);
        if (plist.isOpen()){
            QTextStream out(&plist);


            for (int i = 0; i<ui->tr_list->count();i++){
                out<<static_cast<QPlaylistTrack*>(ui->tr_list->item(i))->filename();
                out<<"\n";
            }
            plist.close();


        }


    }


}

void MainWindow::open_playlist()
{
    QString filename = ask_for_playlist();
    QString to_read;
    QMessageBox k;

    if (!filename.isEmpty()){
        QFile plist(filename);
        plist.open(QIODevice::ReadOnly);
        if (plist.isOpen()){
            QTextStream out(&plist);
            to_read.append(out.readAll());
            QStringList filepaths = to_read.split('\n');
            if (!filepaths.isEmpty()){
                ui->tr_list->clear_tracks();
                foreach (QString filepath, filepaths) {
                    QFile track(filepath);
                    track.open(QIODevice::ReadOnly);
                    if (not track.isOpen()){
                        continue;
                    }
                    track.close();

                    QFileInfo trackfileinfo(track);

                    QString title = trackfileinfo.fileName();
                    QString path = track.fileName();
                    QPlaylistItemWidget *item_widget = new QPlaylistItemWidget(title);
                    if (ui->tr_list->isEditing())
                        item_widget->edit();


                    QPlaylistTrack *newItem = new QPlaylistTrack(path, title, item_widget, ui->tr_list);


                    ui->tr_list->addItem(newItem);
                    ui->tr_list->setItemWidget(newItem, item_widget);
                    if (this->saved_playlist){
                        saved_playlist->addItem(newItem);
                        saved_playlist->setItemWidget(newItem, item_widget);
                    }
                    connect(item_widget, SIGNAL(removeButtonClicked(QPlaylistItemWidget*)), this, SLOT(removeButton_clicked(QPlaylistItemWidget*)));
                }



            }
            else{
                k.critical(this, "Ошибка", "У плейлиста неверный формат!");
            }

            plist.close();


        }


    }
}

void MainWindow::on_save_playlist_act_triggered()
{
    if (ui->tr_list->count()>0){
        save_current_playlist();
        QMessageBox k;
        k.information(this, "Сохранение", "Плейлист успешно сохранён");
    }
    else{
        QMessageBox k;
        k.critical(this, "Ошибка", "Невозможно сохранить пустой плейлист!");
    }
}


void MainWindow::on_open_playlist_act_triggered()
{
    open_playlist();
}

