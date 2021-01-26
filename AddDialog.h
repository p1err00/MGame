#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include "MainWindow.h"
#include "Game.h"

#include <QWidget>
#include <QPushButton>

namespace Ui {
class AddDialog;
}

class AddDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();


    QString f;
    QString path;
    QString dir;

private slots:
    Game on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::AddDialog *ui;
};

#endif // ADDDIALOG_H
