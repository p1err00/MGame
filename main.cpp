#include "MainWindow.h"
#include "Game.h"
#include "Settings/FavoriDialog.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Creation settings
        //Add list game
    QSettings settings("MGame", "Mon programme");
    settings.beginGroup("File");
    settings.setValue("nameFileGame", "save.txt");
    settings.endGroup();
    qDebug() << settings.value("File/nameFileGame").toString();
    w.file.setFileName(settings.value("File/nameFileGame").toString());


    //Set stylesheet
    QFile file("css/Darkeum.qss");
    if(!file.open(QFile::ReadOnly))
        qDebug() << "pas trouver";
    file.open(QFile::ReadOnly);
    QString styleSheet { QLatin1String(file.readAll())};

    a.setStyleSheet(styleSheet);

    FavoriDialog fd;
    w.favList = fd.loadFile();
    for(auto i : w.favList)
        qDebug() << i;
    w.loadFavList();

    //w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return a.exec();
}
