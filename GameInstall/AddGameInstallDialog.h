#ifndef ADDGAMEINSTALLDIALOG_H
#define ADDGAMEINSTALLDIALOG_H

#include "GameInstall.h"

#include <QWidget>

namespace Ui {
class AddGameInstallDialog;
}

class AddGameInstallDialog : public QWidget
{
    Q_OBJECT

public:
    explicit AddGameInstallDialog(QWidget *parent = nullptr);
    ~AddGameInstallDialog();

    QString name;
    QString folder;
    qint64 size;
    bool install;

private slots:
    void on_pbOpen_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddGameInstallDialog *ui;

    QString formatSize(qint64 size);
    qint64 dirSize(QString dirPath);

public slots:
    void on_pushButton_clicked();
};

#endif // ADDGAMEINSTALLDIALOG_H
