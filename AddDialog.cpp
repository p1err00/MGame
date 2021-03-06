#include "AddDialog.h"
#include "ui_AddDialog.h"
#include "MainWindow.h"
#include "Game.h"
#include "Dialog/SelectTypeDialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QMap>
#include <QCheckBox>
#include <QDebug>
#include <QMessageBox>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);

    //Set item into QCombox
    //Changed Value to a list to _listType

    QList<QString> list;
    list << "Action" << "Aventure" << "rpg" << "Mes couilles";

    for(int i = 0; i < list.size(); i++){

        QCheckBox *btn = new QCheckBox(list.at(i));

        connect(btn, &QCheckBox::stateChanged, [=](){

            if(btn->isChecked()){
                QList<QString> list;
                list << btn->text();

                l.append(btn->text());

            }
        });
    }
    //LeName
    ui->leName->setPlaceholderText("Name : ");
    ui->leName->setEnabled(false);
    //ListType
    QFile file("Type.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "not file";
    }
    QTextStream stream(&file);
    QString line;
    QListWidgetItem *lHeader = new QListWidgetItem("Type :");

    ui->lwListType->addItem(lHeader);

    while (stream.readLineInto(&line)) {
        QListWidgetItem *w = new QListWidgetItem(line);
        w->setFlags(w->flags() | Qt::ItemIsUserCheckable);
        w->setCheckState(Qt::Unchecked);
        ui->lwListType->addItem(w);

    }
    file.close();
}

AddDialog::~AddDialog()
{
    delete ui;
}

QMap<QString, QString> AddDialog::on_pbOpen_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Open","*.exe");

        p = file.absoluteFilePath();
        d = p.section("/",0,-2);
        n = p.section("/", -1).toUtf8();

        da = QDateTime::currentDateTime();

        QMap<QString, QString> keys;
        keys.insert("p", p);
        keys.insert("d", d);
        keys.insert("da", da.toString());

        ui->ldirectory->setText(d);
        ui->lpath->setText(p);
        ui->ldate->setText(da.toString());
        ui->leName->setText(n);

        return keys;
}

QString AddDialog::on_teDesc_textChanged()
{
    QString data = ui->teDesc->toHtml();
    QString dataText = ui->teDesc->toPlainText();

    return dataText;
}

void AddDialog::on_buttonBox_accepted()
{
    if(!n.isEmpty()){
        QString desc = on_teDesc_textChanged();

        for(int i = 0; i < ui->lwListType->count(); i++){
            if(ui->lwListType->item(i)->checkState() == Qt::Checked){
                l.append(ui->lwListType->item(i)->text());
            }
        }

        game = new Game(n,d,p, QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm"), desc, l);

        MainWindow *mw = new MainWindow;

        mw->listGame.append(game);

        qDebug()<< "Save game \n";
        mw->saveGame(game);

        qDebug()<< "Game loading";
        mw->loadgameFromFile();
    } else {
        QMessageBox::warning(this, "Warning", "No game selected");
    }

}

