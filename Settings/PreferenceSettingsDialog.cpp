#include "PreferenceSettingsDialog.h"
#include "ui_PreferenceSettingsDialog.h"
#include "MainWindow.h"

#include <QFile>
#include <QApplication>
#include <QDebug>

PreferenceSettingsDialog::PreferenceSettingsDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreferenceSettingsDialog)
{
    ui->setupUi(this);
}

PreferenceSettingsDialog::~PreferenceSettingsDialog()
{
    delete ui;
}

void PreferenceSettingsDialog::on_comboBox_activated(const QString &arg1)
{

    MainWindow mw;
    if(arg1 == "Combinear.qss"){
        QFile file("css/Combinear.qss");

        if(!file.open(QFile::ReadOnly))
            return;

        file.open(QFile::ReadOnly);

        QString styleSheet { QLatin1String(file.readAll())};

        mw.setStyleSheet(styleSheet);
    }
}
