#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


class Game
{
public:
    Game(QString name, QString directory, QString path, QString date, QString desc, QList<QString> types);
    Game(QString name, QString directory, QString path, QString date, int timePlayed, QString dateLastUse, QString desc, QList<QString> types);
    void addTimePlayed(int timePlayed);
    void addDateLastUse(QString dateLastUse);

    //Setter
    void setName(QString name);
    void setDirectory(QString directory);
    void setPath(QString path);
    void setDate(QString date);
    void setDateLastUse(QString date);
    void setDesc(QString desc);
    void setTypes(QList<QString> list);

    Game();

    QString name() const;
    QString directory() const;
    QString path() const;
    QString date() const;
    int timePlayed() const;
    QString dateLastUse() const;
    QString desc() const;
    QList<QString> types() const;

private:
    int _timePlayed;
    QString _name;
    QString _directory;
    QString _path;
    QString _date;
    QString _dateLastUse;
    QString _desc;
    QList<QString> _types;
    QDateTime startProcess;

public slots:

    void fromJson(QJsonObject json);

    QJsonObject toJson();

private slots:

    QJsonObject changeTimePlayed(QJsonObject json, QDateTime startProcess);

    QJsonObject changeDateLastUse(QJsonObject json);

    quint64 calculateTime(QDateTime startProcess, QDateTime stopProcess);

public slots:
    QJsonObject changeTypes(QJsonObject json, QList<QString> list);

    QJsonObject changeDesc(QJsonObject json, QString desc);

};

#endif // GAME_H
