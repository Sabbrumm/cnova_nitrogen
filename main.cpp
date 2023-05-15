
#include "mainwindow.h"

#include <QApplication>

#include <QLocale>
#include <QTranslator>
#include <QFile>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile stylesheet(":/styles/Darkeum.qss");
    stylesheet.open(QFile::ReadOnly);
    QString style = {QLatin1String(stylesheet.readAll())};
    a.setStyleSheet(style);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "nitrogen_player_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
