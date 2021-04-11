#include "GameInstallManager.h"
#include "GameInstall.h"
#include "MainWindow.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

GameInstallManager::GameInstallManager()
{

}

void GameInstallManager::save(GameInstall *gameInstall){

    QFile file("saveGameInstall.json");
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
    json = gameInstall->toJson();

    if(!jsonArray.contains(json))
        jsonArray.append(json);
    else {
        int count = 0;
        for(auto item : jsonArray){
            if(item.toObject() == json){
                jsonArray.replace(count, json);
            }
            count ++;
        }
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    stream << QJsonDocument(jsonArray).toJson();
    qDebug() << "File save";
    file.close();
}

QList<GameInstall> GameInstallManager::load(){
    QList<GameInstall> list;

    return list;
}
