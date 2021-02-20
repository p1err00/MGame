#include "Game.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QDateTime>

Game::Game(QString name, QString directory, QString path, QString date, QString desc, QList<QString> type):
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _desc(desc),
    _type(type)
{
    _timePlayed = 0.0;

}

Game::Game(QString name, QString directory, QString path, QString date, int timePlayed, QString dateLastUse, QString desc, QList<QString> type) :
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _dateLastUse(dateLastUse),
    _desc(desc),
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
int Game::timePlayed() const
{
    return _timePlayed;
}
QString Game::dateLastUse() const
{
    return _dateLastUse;
}
QString Game::desc() const
{
    return _desc;
}
QList<QString> Game::type() const
{
    return _type;
}
