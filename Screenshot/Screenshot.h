#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QString>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QDir>

class Screenshot
{
public:
    Screenshot(QString folder = "", QString fileName = "");

    void shootScreen();

    void saveScreen(QPixmap pix);

    void setFolder(QString folder);

    void setFileName(QString fileName);
    void setEnableCounter(bool enableCounter);
    void setShotCounter(int shotCounter = 0);
    int screenShotCount;

private:

    QString folder;
    QString fileName;
    bool enableCounter;

};

#endif // SCREENSHOT_H
