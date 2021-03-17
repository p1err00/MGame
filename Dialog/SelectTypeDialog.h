#ifndef SELECTTYPEDIALOG_H
#define SELECTTYPEDIALOG_H

#include "Game.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class SelectTypeDialog;
}

class SelectTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTypeDialog(QWidget *parent = nullptr);
    ~SelectTypeDialog();

    QList<QString> listType;

private slots:

private:
    Ui::SelectTypeDialog *ui;

public slots:
    QList<QString> on_buttonBox_accepted();
};

#endif // SELECTTYPEDIALOG_H
