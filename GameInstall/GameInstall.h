#ifndef GAMEINSTALL_H
#define GAMEINSTALL_H

#include <QString>
#include <QJsonObject>
#include <QFile>

class GameInstall
{
public:
    GameInstall();
    GameInstall(QString name, QString folder, qint64 size, bool install = false);

    QString name() const;
    QString folder() const;
    qint64 size() const;
    bool install() const;

    void setName(QString name);
    void setFolder(QString folder);
    void setSize(qint64 size);
    void setInstall(bool install);

    void fromJson(QJsonObject json);

    QJsonObject toJson();

private:
    QString _name;
    QString _folder;
    qint64 _size;
    bool _install;

};

#endif // GAMEINSTALL_H
