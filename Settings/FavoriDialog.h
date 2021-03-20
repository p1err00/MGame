#ifndef FAVORIDIALOG_H
#define FAVORIDIALOG_H

#include <QList>
#include <QJsonArray>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class FavoriDialog
{
public:
    FavoriDialog();

    QList<QString> addFav(QList<QString> favList, QString name);
    QList<QString> delFav(QList<QString> favList, QString name);
    void saveFile(QList<QString> favList);
    QList<QString> loadFile();

    QList<QString> favList;
};

#endif // FAVORIDIALOG_H
