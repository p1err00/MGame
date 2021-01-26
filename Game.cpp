#include "Game.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QDateTime>

Game::Game(QString name, QString directory, QString path, QString date):
    _name(name),
    _directory(directory),
    _path(path),
    _date(date)
{
    _timePlayed = 0.0;
    QDate lastUse;

}

Game::Game(QString name, QString directory, QString path, QString date, int timePlayed) :
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _timePlayed(timePlayed)
{

}

void Game::addTimePlayed(int timePlayed){
    _timePlayed += timePlayed;
}

QString Game::name() const
{
    return _name;
}
QString Game::directory() const
{
    return _directory;
}
QString Game::path() const
{
    return _path;
}
QString Game::date() const
{
    return _date;
}
long Game::timePlayed() const
{
    return _timePlayed;
}

