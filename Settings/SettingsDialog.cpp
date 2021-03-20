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
                    listType);

    return *game;
}
