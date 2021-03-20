#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "Game.h"

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    //SettingsDialog();

    void displayGame(Game *game);
private slots:


private:
    Ui::SettingsDialog *ui;

    QString name;
    QList<QString> listType;

public slots:
    Game on_buttonBox_accepted();
};

#endif // SETTINGSDIALOG_H
