#ifndef MAINWINDOWINTERFACE_H
#define MAINWINDOWINTERFACE_H

#include "Game.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QDialog>
#include <QProcess>
#include <QJsonObject>

class MainWindowInterface
{
public:
    MainWindowInterface();

private:
    QString f;
    QString path;
    QString dir;
    QPushButton *pbOpen;
    QMap<QString, Game*> listGame;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *launch;
    QJsonObject m_currentJsonObject;
};

#endif // MAINWINDOWINTERFACE_H
