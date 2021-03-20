#include "FavoriDialog.h"

FavoriDialog::FavoriDialog()
{

}

QList<QString> FavoriDialog::addFav(QList<QString> favList, QString name){

    favList.append(name);
    return favList;
}
QList<QString> FavoriDialog::delFav(QList<QString> favList, QString name){

    for(int i = 0; i < favList.size(); i++){
        if(favList.at(i) == name)
            favList.removeAt(i);
    }

    QFile file("favori.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return favList;
    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    int count = 0;
    for(QJsonValue value : jsonArray){

        if(!value.isObject())
            continue;

        QJsonObject json = value.toObject();

        if(json.value("fav") == name){
            qDebug() << "Find";
            jsonArray.removeAt(count);
            favList.removeAt(count);
        }
        count++;
    }


    return favList;
}
void FavoriDialog::saveFile(QList<QString> favList){

    loadFile();
    QFile file("favori.json");
    QJsonArray jsonArray;

    if(file.exists()){

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream stream(&file);
        jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
        file.close();
        file.remove();
    }

    QJsonObject json;
    for(auto i : favList){
        json.insert("fav", i);
    }

    jsonArray.append(json);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    stream << QJsonDocument(jsonArray).toJson();
    file.close();

}
QList<QString> FavoriDialog::loadFile(){

    QFile file("favori.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return favList;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(QJsonValue value : jsonArray){
        if(!value.isObject())
            continue;
    QJsonObject json = value.toObject();
    favList.append(json.value("fav").toString());
    }
    return favList;
}
