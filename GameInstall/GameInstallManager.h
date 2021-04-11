#ifndef GAMEINSTALLMANAGER_H
#define GAMEINSTALLMANAGER_H

#include "GameInstall.h"

#include <QFile>


class GameInstallManager
{
public:
    GameInstallManager();

    void save(GameInstall *gameInstall);

    QList<GameInstall> load();

    void add(GameInstall gameInstall);

    void deleted(GameInstall gameInstall);

private:
    GameInstall *gameInstall;
};

#endif // GAMEINSTALLMANAGER_H
