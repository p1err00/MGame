#include "MainWindow.h"
#include "Game.h"
#include "Settings/FavoriDialog.h"

#include <QApplication>
#include <QSettings>
#include <QCoreApplication>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Creation settings
        //Add list game
    QCoreApplication::setOrganizationName("azeaze");
    QCoreApplication::setOrganizationDomain("monentreprise.com") ;
    QCoreApplication::setApplicationName("MonApplication") ;


    QSettings settings("MGame", "Mon programme");
    settings.beginGroup("File");
    settings.setValue("saveFileGame", "save.txt");
    settings.endGroup();
    qDebug() << settings.value("File/nameFileGame").toString();
    w.file.setFileName(settings.value("File/nameFileGame").toString());


    //Set stylesheet
    QFile file("css/SyNet.qss");
    if(!file.open(QFile::ReadOnly))
        qDebug() << "pas trouver";
    file.open(QFile::ReadOnly);
    QString styleSheet { QLatin1String(file.readAll())};

    a.setStyleSheet(styleSheet);

    FavoriDialog fd;
    w.favList = fd.loadFile();
    w.loadList();
    w.loadFavList();
    w.loadTransmission();

    w.setWindowState(Qt::WindowMaximized);
    int widthScreen = QApplication::desktop()->width();
    int heighScreen = QApplication::desktop()->height();
    w.setFixedSize(widthScreen, heighScreen);

    w.show();
    return a.exec();
}
