#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //w.setStyleSheet("QLineEdit { background-color: yellow }");

    w.isFullScreen();
    w.show();
    return a.exec();
}
