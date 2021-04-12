#include "Screenshot.h"

#include <QScreen>
#include <QWidget>
#include <QWindow>
#include <QPixmap>
#include <QDebug>
#include <QRandomGenerator>

Screenshot::Screenshot(QString folder, QString fileName)
{
    setShotCounter();
    setFolder(folder);
    setFileName(fileName);
    setEnableCounter(enableCounter);
}

void Screenshot::shootScreen(){

    qDebug() << "Shoot screen";

    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap originalPixmap = screen->grabWindow(QApplication::desktop()->hasFocus());

    saveScreen(originalPixmap);
}


void Screenshot::saveScreen(QPixmap pix)
{
    // - Save this picture

    QFile file(fileName);
    quint64 random = QRandomGenerator::global()->generate();
    pix.save(QDir::currentPath()+"\\Screenshot\\"+fileName+QString::number(random)+".png");
}

void Screenshot::setFolder(QString folder)
{
    this->folder = folder+("" != folder && !folder.endsWith("/") ? "/" : "");
    // - Check if folder exists
    if ("" != folder && !QDir(folder).exists()) {
        QDir().mkpath(folder);
    }
}

void Screenshot::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void Screenshot::setEnableCounter(bool enableCounter)
{
    this->enableCounter = enableCounter;
}

void Screenshot::setShotCounter(int screenShotCount)
{
    this->screenShotCount = screenShotCount;
}
