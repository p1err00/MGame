#include "MainSettingsDialog.h"
#include "ui_MainSettingsDialog.h"

#include "PreferenceSettingsDialog.h"

MainSettingsDialog::MainSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainSettingsDialog)
{
    ui->setupUi(this);

}

MainSettingsDialog::~MainSettingsDialog()
{
    delete ui;
}

void MainSettingsDialog::on_pushButton_clicked()
{
    PreferenceSettingsDialog *p = new PreferenceSettingsDialog;
    ui->l_main->addWidget(p);
}
