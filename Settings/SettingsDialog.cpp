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

    if(!game->linkPicture().isNull())
        linkPicture = game->linkPicture();
    if(!game->linkCouverture().isNull())
        linkCouverture = game->linkCouverture();

    if(!game->types().isEmpty()){
        for(auto i : game->types()){
            type << i;
        }
    }

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

    fillType(game->types());
    file.close();
}

void SettingsDialog::fillType(QList<QString> list){
    qDebug() << "Enter fill";
    for(int i = 0; i < ui->lwListType->count(); i++){
        for(auto it : list){
            if(ui->lwListType->item(i)->text() == it){
                qDebug() << "Checked";
                ui->lwListType->item(i)->setCheckState(Qt::CheckState::Checked);
            }
        }
    }
}

Game SettingsDialog::on_buttonBox_accepted()
{
    Game *game;

    if(ui->leName->text().isEmpty())
        QMessageBox::warning(this, "Name invalid", "Name can't be empty");

    for(int i = 0; i < ui->lwListType->count(); i++){
        if(ui->lwListType->item(i)->checkState() == Qt::Checked){
            listType.append(ui->lwListType->item(i)->text());
        }
    }
    game = new Game(ui->leName->text(),
                    ui->ldirectory->text(),
                    ui->lpath->text(),
                    ui->ldate->text(),
                    ui->lTimePlayed->text().toInt(),
                    ui->lDatelastUse->text(),
                    ui->teDesc->toPlainText(),
                    listType,
                    linkPicture,
                    linkCouverture);

    return *game;
}
