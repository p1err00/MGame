#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setStyleSheet(
                    "QListWidget {"
                    "background : qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 white, stop:1 #969696);"
                    "color : black;"
                    "min-width : 10em;"
                    "height : 2em;"
                    "padding : 10px;"
                    "}"
                    "QPushButton {"
                    "background-color : #858585;"
                    "color : white;"
                    "width : 250px;"
                    "height : 40px;"
                    "border-width : 4px;"
                    "border-style : solid;"
                    "border-radius : 10px;"
                    "}"
                    "QPushButton:hover {"
                    "background : qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #C5C5C5, stop:1 #666666);"
                    "}");
    w.show();
    return a.exec();
}
