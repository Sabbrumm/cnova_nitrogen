QT       += core gui
QT       += multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qplaylist.cpp \
    qplaylistitemwidget.cpp \
    qplaylisttrack.cpp

HEADERS += \
    mainwindow.h \
    qplaylist.h \
    qplaylistitemwidget.h \
    qplaylisttrack.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    nitrogen_player_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/Darkeum.qss \
    resources/dark_teal.qss \
    resources/icons/pause.png \
    resources/icons/play.png \
    resources/icons/plus.png \
    resources/icons/skip-next.png \
    resources/icons/skip-previous.png \
    resources/icons/stop.png

RESOURCES += \
    resources.qrc
