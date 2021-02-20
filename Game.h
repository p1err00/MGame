#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QDate>


class Game
{
public:
    Game(QString name, QString directory, QString path, QString date, QString desc, QList<QString> type);
    Game(QString name, QString directory, QString path, QString date, int timePlayed, QString dateLastUse, QString desc, QList<QString> type);
    void addTimePlayed(int timePlayed);
    Game();

    QString name() const;
    QString directory() const;
    QString path() const;
    QString date() const;
    int timePlayed() const;
    QString dateLastUse() const;
    QString desc() const;
    QList<QString> type() const;

private:
    int _timePlayed;
    QString _name;
    QString _directory;
    QString _path;
    QString _date;
    QString _dateLastUse;
    QString _desc;
    QList<QString> _type;

};

#endif // GAME_H
