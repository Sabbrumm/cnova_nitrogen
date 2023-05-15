
#include "qplaylisttrack.h"

QPlaylistTrack::QPlaylistTrack(QString filename, QString title,  QPlaylistItemWidget *child, QListWidget* parent):QListWidgetItem(parent)
{
    flags().setFlag(Qt::ItemIsDragEnabled, false);
    flags().setFlag(Qt::ItemIsDropEnabled, false);
    flags().setFlag(Qt::ItemIsSelectable);
    _filename = filename;
    widget = child;
    if (child){
        child->setTitle(title);
        setSizeHint(child->sizeHint());
    }

}
