#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QDate>


class Game
{
public:
    Game(QString name, QString directory, QString path, QString date);
    Game(QString name, QString directory, QString path, QString date, int timePlayed);
    void addTimePlayed(int timePlayed);
    Game();

    QString name() const;
    QString directory() const;
    QString path() const;
    QString date() const;
    long timePlayed() const;

private:
    long _timePlayed;
    QString _name;
    QString _directory;
    QString _path;
    QString _date;

};

#endif // GAME_H
