#ifndef MAINSETTINGSDIALOG_H
#define MAINSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class MainSettingsDialog;
}

class MainSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainSettingsDialog(QWidget *parent = nullptr);
    ~MainSettingsDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainSettingsDialog *ui;
};

#endif // MAINSETTINGSDIALOG_H
