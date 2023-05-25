
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QListWidgetItem>
#include "qplaylistitemwidget.h"
#include "qplaylisttrack.h"
#include <QAudioInput>
#include <QAudioOutput>
#include <QTimer>
#include "qplaylist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT


    bool do_repeat=0;
    QPlaylist* saved_playlist=nullptr;


    QAudioOutput* output;
    QTimer* eq_timer;




    void set_play_icon();
    void set_pause_icon();
    void set_metadata(QPlaylistTrack* source);
    void set_default_metadata();
    void player_stop();
    void player_start(QString filepath);
    void player_pause();
    void player_play();



    QStringList ask_for_tracks();
    QString ask_for_playlist();
    QString ask_for_saving_playlist();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_page_playlist_button_clicked();

    void on_page_player_button_clicked();

    void on_playlist_act_triggered();

    void on_player_act_triggered();

    void on_change_button_clicked();

    void on_tr_list_itemDoubleClicked(QListWidgetItem *item);

    void on_add_button_clicked();

    void removeButton_clicked(QPlaylistItemWidget* source);

    void on_shuffle_button_clicked();

    void on_next_button_clicked();

    void on_previous_button_clicked();

    void on_playpause_button_clicked();

    void do_track_stop();

    void do_track_pause(QPlaylistTrack* source);

    void do_track_play(QPlaylistTrack* source);

    void do_timing_change(qint64 position);

    void track_end(QMediaPlayer::MediaStatus status);
    void on_time_slider_sliderPressed();

    void on_time_slider_sliderMoved(int position);

    void on_time_slider_sliderReleased();

    void eq_randomize();

    void eq_normalize();

    void player_error(QMediaPlayer::Error err, QString what);

    void on_open_file_act_triggered();

    void on_repeat_clicked();

    void save_current_playlist();

    void open_playlist();

    void on_save_playlist_act_triggered();

    void on_open_playlist_act_triggered();

private:
    Ui::MainWindow *ui;

private:
    QMediaPlayer* player;

};

#endif // MAINWINDOW_H
