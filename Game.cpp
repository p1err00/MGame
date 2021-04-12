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
    _linkPicture = "";
    _linkCouverture = "";
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
    _linkPicture = "";
    _linkCouverture = "";
}

Game::Game(QString name, QString directory, QString path, QString date, int timePlayed, QString dateLastUse, QString desc, QList<QString> types, QString linkPicture, QString linkCouverture) :
    _name(name),
    _directory(directory),
    _path(path),
    _date(date),
    _dateLastUse(dateLastUse),
    _desc(desc),
    _timePlayed(timePlayed),
    _types(types),
    _linkPicture(linkPicture),
    _linkCouverture(linkCouverture)
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
QString Game::linkPicture(){
    return _linkPicture;
}
QString Game::linkCouverture() const{
    return _linkCouverture;
}
//Change time played
void Game::addTimePlayed(int timePlayed){

    _timePlayed += timePlayed;
}

//Change date last use
void Game::addDateLastUse(QString dateLastUse){
    _dateLastUse += dateLastUse;
}



QJsonObject Game::toJson(){

    QJsonObject json;

    json.insert("name", name());
    json.insert("directory", directory());
    json.insert("path", path());
    json.insert("date", date());
    json.insert("dateLastUse", dateLastUse());
    json.insert("desc", desc());
    json.insert("timePlayed", timePlayed());
    json.insert("linkPicture", linkPicture());
    json.insert("linkCouverture", linkCouverture());
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
void Game::setDateLastUse(QString dateLastUse){
    _dateLastUse = dateLastUse;
}
void Game::setDesc(QString desc){
    _desc = desc;
}
void Game::setTypes(QList<QString> types){
    _types = types;
}
void Game::setTimePlayed(int timePlayed){
    _timePlayed = timePlayed;
}
void Game::setLinkPicture(QString linkPicture){
    _linkPicture = linkPicture;
}
void Game::setLinkCouverture(QString linkCouverture){
    _linkCouverture = linkCouverture;
}
void Game::fromJson(QJsonObject json){

    this->setName(json.value("name").toString().toUtf8());
    this->setDirectory(json.value("directory").toString().toUtf8());
    this->setDate(json.value("date").toString().toUtf8());
    this->setPath(json.value("path").toString().toUtf8());
    this->setDateLastUse(json.value("dateLastUse").toString());
    this->setDesc(json.value("desc").toString().toUtf8());
    this->setTimePlayed(json.value("timePlayed").toInt());
    this->setLinkPicture(json.value("linkPicture").toString());
    this->setLinkCouverture(json.value("linkCouverture").toString());
    QList<QString> list;
    for(auto item : json.value("type").toString())
        list.append(item);
    this->setTypes(list);
}
