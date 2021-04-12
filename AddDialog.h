#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include "Game.h"

#include <QDialog>
#include <QTextCharFormat>
#include <QDateTime>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();

    QString n;
    QString p;
    QString d;
    QDateTime da;
    QList<QString> l;
    Game *game;

private slots:
    QMap<QString, QString> on_pbOpen_clicked();

    QString on_teDesc_textChanged();

    void on_buttonBox_accepted();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
