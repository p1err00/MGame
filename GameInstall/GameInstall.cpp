#include "GameInstall.h"

GameInstall::GameInstall()
{

}
GameInstall::GameInstall(QString name, QString folder, qint64 size, bool install){

    _name = name;
    _folder = folder;
    _size = size;
    _install = install;
}

QString GameInstall::name() const{
    return _name;
}
QString GameInstall::folder() const{
    return _folder;
}
qint64 GameInstall::size() const{
    return _size;
}
bool GameInstall::install() const{
    return _install;
}

void GameInstall::setName(QString name){
    _name = name;
}
void GameInstall::setFolder(QString folder){
    _folder = folder;
}
void GameInstall::setSize(qint64 size){
    _size = size;
}
void GameInstall::setInstall(bool install){
    _install = install;
}

void GameInstall::fromJson(QJsonObject json){

    this->setName(json.value("name").toString());
    this->setFolder(json.value("folder").toString());
    this->setSize(json.value("size").toInt());
    this->setInstall(json.value("install").toBool());

}

QJsonObject GameInstall::toJson(){
    QJsonObject json;

    json.insert("name", name());
    json.insert("folder", folder());
    json.insert("size", size());
    json.insert("install", install());

    return json;
}
