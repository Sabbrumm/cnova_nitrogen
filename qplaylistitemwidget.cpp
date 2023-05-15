
#include "qplaylistitemwidget.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPixmap>

void QPlaylistItemWidget::_update()
{
    removebutton->setVisible(showRemove());

    switch (trackState()){
        case (Paused):
            this->playing_icon->setPixmap(QPixmap(":/icons/button_pause.png").scaled(playing_icon->width(), playing_icon->height()));
            break;
        case (Playing):
            this->playing_icon->setPixmap(QPixmap(":/icons/button_play.png").scaled(playing_icon->width(), playing_icon->height()));
            break;
        case (Stopped):
            this->playing_icon->setPixmap(QPixmap(1,0));
            break;
    }

}

QPlaylistItemWidget::QPlaylistItemWidget(QString title, QWidget *parent)
    : QWidget{parent}
{
    track_name = new QLabel(title);
    playing_icon = new QLabel();
    playing_icon->setMinimumSize(20, 20);
    removebutton = new QPushButton("-");
    removebutton->setMaximumHeight(16);
    removebutton->setStyleSheet("background-color:red;");


    playing_icon->setMaximumSize(QSize(32, 32));


    _title = title;
    _update();

    connect(removebutton, SIGNAL(clicked()), this, SLOT(on_removeButton_Clicked()));

    QHBoxLayout *item_layout = new QHBoxLayout(this);
    item_layout->addWidget(playing_icon);
    item_layout->addWidget(track_name);
    item_layout->addSpacerItem(new QSpacerItem(10,10, QSizePolicy::Expanding));
    item_layout->addWidget(removebutton);
    setLayout(item_layout);
}

void QPlaylistItemWidget::on_removeButton_Clicked()
{
    emit removeButtonClicked(this);
}

