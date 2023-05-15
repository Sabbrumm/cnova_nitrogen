
#ifndef PLAYLISTELEMENT_H
#define PLAYLISTELEMENT_H

#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
class PlaylistElement{
private:
    QWidget* _representation;
    QString _filename;
    QString _trackname;

    void _init_repr(){
        _representation = new QWidget();
        auto layout = new QHBoxLayout(_representation);
        layout->addWidget(new QLabel(_filename));
        layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Fixed));
        layout->addWidget(new QLabel(_trackname));
    }


public:
    PlaylistElement(QString filename, QString trackname){
        _filename = filename;
        _trackname = trackname;
        _init_repr();
    }

    QWidget* representation(){
        return _representation;
    }
    QString filename(){
        return _filename;
    }

};





#endif // PLAYLISTELEMENT_H
