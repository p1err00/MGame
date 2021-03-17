#include "SelectTypeDialog.h"
#include "ui_SelectTypeDialog.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QListWidgetItem>
#include <QCheckBox>

SelectTypeDialog::SelectTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectTypeDialog)
{
    ui->setupUi(this);

    QFile file("Type.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "not file";
        return;
    }
    QTextStream stream(&file);
    QString line;
    while (stream.readLineInto(&line)) {
        QListWidgetItem *w = new QListWidgetItem(line);
        w->setFlags(w->flags() | Qt::ItemIsUserCheckable);
        w->setCheckState(Qt::Unchecked);
        ui->lwListType->addItem(w);
        ui->lwListType->sortItems();
    }
    file.close();
}

SelectTypeDialog::~SelectTypeDialog()
{
    delete ui;
}

QList<QString> SelectTypeDialog::on_buttonBox_accepted()
{
    for(int i = 0; i < ui->lwListType->count(); i++){
        if(ui->lwListType->item(i)->checkState() == Qt::Checked){
            listType.append(ui->lwListType->item(i)->text());
        }
    }
    return listType;
}
