#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include "Game.h"
#include "MainWindow.h"

#include <QDialog>

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();

    Game *game;


private:
    Ui::EditDialog *ui;

    QString _n;
    QString _p;
    QString _d;
    QDateTime _da;

private slots:
    QString on_teDesc_textChanged();

    Game on_buttonBox_accepted();

    Game on_leName_textChanged(const QString &arg1);
};

#endif // EDITDIALOG_H
