#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::displayGame(Game *game){

    ui->leName->setText(game->name());
    ui->teDesc->setText(game->desc());
    ui->ldate->setText(game->date());
    ui->ldirectory->setText(game->directory());
    ui->lpath->setText(game->path());

    timePlayed = game->timePlayed();
    dateLastUse = game->dateLastUse();

    //Convert int into time
    int ti = game->timePlayed();
    int sec = 0, min = 0, hour = 0;
    sec = ti/1000;
    //Convert to minutes
    while(sec >= 60){
        min += 1;
        sec -= 60;
    }
    while(min >= 60){
        hour +=1;
        min -= 60;
    }

    ui->lTimePlayed->setText(QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec));

    if(game->dateLastUse() == "")
        ui->lDatelastUse->setText("Never used");
    else
        ui->lDatelastUse->setText(game->dateLastUse());

    if(!game->linkPicture().isNull())
        linkPicture = game->linkPicture();
    if(!game->linkCouverture().isNull())
        linkCouverture = game->linkCouverture();

    QLabel *l;
    for(auto i : game->types()){
        l = new QLabel(i);
        ui->layoutInfo->addWidget(l);
    }
}

void SettingsDialog::fillType(QList<QString> list){
    qDebug() << "Enter fill";

}

Game SettingsDialog::on_buttonBox_accepted()
{

    listType.clear();
    if(ui->leName->text().isEmpty())
        QMessageBox::warning(this, "Name invalid", "Name can't be empty");

    n_game = new Game(ui->leName->text(),
                    ui->ldirectory->text(),
                    ui->lpath->text(),
                    ui->ldate->text(),
                    timePlayed,
                    dateLastUse,
                    ui->teDesc->toPlainText(),
                    type,
                    linkPicture,
                    linkCouverture);

    return *n_game;
}
