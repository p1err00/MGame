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

                for(int i = 0; i < list.size(); i++){

                    QLabel *label = new QLabel(list.at(i));
                    ui->layoutInfogame->addWidget(label);
                }
            }
        });
    }
    //LeName
    ui->leName->setPlaceholderText("Name : ");
    //ListType
    QFile file("Type.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "not file";
        return;
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

        //d.replace(" ", "\\ ");

        da = QDateTime::currentDateTime();

        QMap<QString, QString> keys;
        keys.insert("p", p);
        keys.insert("d", d);
        keys.insert("da", da.toString());

        ui->ldirectory->setText(d);
        ui->lpath->setText(p);
        ui->ldate->setText(da.toString());

        return keys;
}

QString AddDialog::on_teDesc_textChanged()
{
    QString data = ui->teDesc->toHtml();
    QString dataText = ui->teDesc->toPlainText();

    if(data.length() >= 20){
        ui->ldesc->setText("\n");
    }
    ui->ldesc->setText(dataText);
    //ui->ldesc->as

    return dataText;
}

Game AddDialog::on_buttonBox_accepted()
{
    QString desc = on_teDesc_textChanged();

    n = ui->leName->text();

    for(int i = 0; i < ui->lwListType->count(); i++){
        if(ui->lwListType->item(i)->checkState() == Qt::Checked){
            l.append(ui->lwListType->item(i)->text());
        }
    }

    Game *game = new Game(n,d,p, QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm"), desc, l);

    MainWindow *mw = new MainWindow;

    mw->listGame.append(game);

    qDebug()<< "Save game \n";
    mw->saveGame(game);

    qDebug()<< "Game loading";
    mw->loadgameFromFile();
    return *game;
}

void AddDialog::on_leName_textChanged(const QString &arg1)
{
    ui->lname->setText(arg1);
}
