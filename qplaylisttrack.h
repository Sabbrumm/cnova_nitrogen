
#ifndef QPLAYLISTTRACK_H
#define QPLAYLISTTRACK_H

#include "qplaylistitemwidget.h"
#include <QListWidgetItem>


class QPlaylistTrack: public QObject, public QListWidgetItem {

    Q_OBJECT
    QPlaylistItemWidget *widget;
    QString _filename;




public:

    explicit QPlaylistTrack(QString filename, QString title = "", QPlaylistItemWidget *child = nullptr, QListWidget* parent=nullptr);
    void setFilename(QString& filename){
        _filename = filename;
    }
    QString filename(){return _filename;}
    QPlaylistItemWidget* getWidget(){
        return widget;
    }


};

#endif // QPLAYLISTTRACK_H
