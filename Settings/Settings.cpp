#include "Settings.h"

Settings::Settings()
{
    QSettings settings("MGame", "Mon programme");
    settings.beginGroup("File");
    settings.setValue("nameFileGame", "save.txt");
    settings.endGroup();
}
