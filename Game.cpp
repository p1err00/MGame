#include "Game.h"

#include <QObject>
#include <QString>
#include <QDate>
#include <QDateTime>

Game::Game(QString name, QString directory, QString path, QString date, QString desc, QList<QString> types):
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _desc(desc),
    _types(types)
{
    _timePlayed = 0.0;

}

Game::Game(QString name, QString directory, QString path, QString date, int timePlayed, QString dateLastUse, QString desc, QList<QString> types) :
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _dateLastUse(dateLastUse),
    _desc(desc),
    _timePlayed(timePlayed),
    _types(types)
{

}

Game::Game(){

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
QList<QString> Game::types() const
{
    return _types;
}

//Change time played
void Game::addTimePlayed(int timePlayed){

    _timePlayed += timePlayed;
}

//Change date last use
void Game::addDateLastUse(QString dateLastUse){
    _dateLastUse += dateLastUse;
}

//Calculate difference between start and stop time program
quint64 Game::calculateTime(QDateTime startProcess, QDateTime stopProcess){

    quint64 difference = qAbs(startProcess.date().daysTo(stopProcess .date()));
    difference  *= static_cast<quint64>(24); // days to hours
    difference  *= static_cast<quint64>(60); // hours to minutes
    difference  *= static_cast<quint64>(60); // minutes to seconds
    difference  *= static_cast<quint64>(1000); // seconds to milliseconds
    difference += qAbs(startProcess.time().msecsTo(stopProcess .time()));

    qDebug() << difference;
    return difference;
}

QJsonObject Game::toJson(){

    QJsonObject json;

    json.insert("name", name());
    json.insert("directory", directory());
    json.insert("path", path());
    json.insert("date", date());
    json.insert("dateLastUse", dateLastUse());
    json.insert("desc", desc());
    int count = 0;
    QJsonArray t;
    for(auto item : this->types()){
        t.insert(count, item);
    }
    json.insert("type", t);
    json.insert("timePlayed", timePlayed());
   return json;

}

void Game::setName(QString name){
    _name = name;
}
void Game::setDirectory(QString directory){
    _directory = directory;
}
void Game::setPath(QString path){
    _path = path;
}
void Game::setDate(QString date){
    _date = date;
}
void Game::setDesc(QString desc){
    _desc = desc;
}
void Game::setTypes(QList<QString> types){
    _types = types;
}
void Game::fromJson(QJsonObject json){

    QList<QString> list;
    for(auto item : json.value("type").toString())
        list.append(item);

    this->setName(json.value("name").toString());
    this->setDirectory(json.value("directory").toString());
    this->setPath(json.value("path").toString());
    this->setDateLastUse(json.value("dateLastUse").toString());
    this->setDesc(json.value("desc").toString());
    this->setTypes(list);
}

QJsonObject Game::changeTimePlayed(QJsonObject json, QDateTime startProcess){
    //get current time
    QDateTime stopProcess = QDateTime::currentDateTime();

    int timeDiff = calculateTime(startProcess, stopProcess);

    //int timePlayed = json.value("timePlayed").toInt();
    //timePlayed += timeDiff;

    json = changeDateLastUse(json);

    Game game;
    game.fromJson(json);
    game.addTimePlayed(timeDiff + json.value("timePlayed").toInt());
    json = game.toJson();

    return json;
}

QJsonObject Game::changeDateLastUse(QJsonObject json){

    QDate value = QDate::currentDate();

    Game game;
    game.addDateLastUse(value.toString());
    json = game.toJson();

    return json;
}

