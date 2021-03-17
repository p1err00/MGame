#include "MainWindow.h"
#include "Game.h"

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

    w.setStyleSheet(
                    "QListWidget {"
                    "background : #dfdede;"
                    "color : black;"
                    "min-width : 13em;"
                    "height : 2em;"
                    "padding-left : 10px;"
                    "padding-right : 10px;"
                    "border-bottom-right-radius: 15px;"
                    "border-top-right-radius: 15px;"
                    "font: 12pt 'Courier New', 'monospace';"
                    "}"
                    "QScrollBar:vertical {"
                    "border: none;"
                    "background:#dfdede;"
                    "width: 7px;"
                    "margin-left: 3px;"
                    "margin-top: 3px;"
                    "}"
                    "QScrollBar::handle:vertical {"
                        "background: black;"
                        "min-height: 0px;"
                    "}"
                    "QScrollBar::add-line:vertical {"
                        "background: red;"
                        "height: 0px;"
                        "subcontrol-position: bottom;"
                        "subcontrol-origin: margin;"
                    "}"
                    "QScrollBar::sub-line:vertical {"
                        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                        "stop: 0  rgb(32, 47, 130), stop: 0.5 rgb(32, 47, 130),  stop:1 rgb(32, 47, 130));"
                        "height: 0 px;"
                        "subcontrol-position: top;"
                        "subcontrol-origin: margin;"
                    "}"
                    );
    //w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return a.exec();
}
