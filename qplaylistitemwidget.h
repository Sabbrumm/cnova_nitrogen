
#ifndef QPLAYLISTITEMWIDGET_H
#define QPLAYLISTITEMWIDGET_H


#include <QPushButton>
#include <QLabel>
#include <QListWidgetItem>
#include <QWidget>

class QPlaylistItemWidget : public QWidget
{
public:
    enum TrackState{
        Playing, Paused, Stopped
    };
private:
    Q_OBJECT

    QString _title;
    TrackState _state = Stopped;
    //QPlaylistItem *parent=nullptr;
    bool _showremove = 0;
    void _update();


    QPushButton *removebutton;
    QLabel *track_name;
    QLabel *playing_icon;

public:
    explicit QPlaylistItemWidget(QString title, QWidget *parent = nullptr);

    void setTitle(QString& title){
        _title = title;
        _update();
    }
    void setShowRemove(bool val=1){
        _showremove = val;
        _update();
    }

    QString title(){return _title;}

    TrackState trackState(){return _state;}

    bool showRemove(){return _showremove;}

    void play(){
        _state = TrackState::Playing;
        _update();
    }

    void pause(){
        _state = TrackState::Paused;
        _update();
    }

    void stop(){
        _state = TrackState::Stopped;
        _update();
    }

    void edit(){
        _showremove = 1;
        _update();
    }

    void done(){
        _showremove = 0;
        _update();
    }

private slots:
    void on_removeButton_Clicked();

signals:
    void removeButtonClicked(QPlaylistItemWidget *source);
};



#endif // QPLAYLISTITEMWIDGET_H
