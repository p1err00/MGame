#include "AddDialog.h"
#include "MainWindow.h"
#include "Game.h"
#include "ui_AddDialog.h"

#include <QDialog>
#include <QPushButton>
#include <QMap>
#include <QComboBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>

AddDialog::AddDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
}

AddDialog::~AddDialog()
{
    delete ui;
}

Game AddDialog::on_pushButton_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Open","*.exe");

    path = file.absoluteFilePath();
    f = path.section("/",-1,-1);
    dir = path.section("/",0,-2);

    Game *game = new Game(f,dir,path, QDateTime::currentDateTime().toString("dd.MM.yyyy hh.mm"));

    ui->label->setText(game->name());
    ui->label_2->setText(game->path());

    MainWindow *mw = new MainWindow;

    mw->listGame.append(game);

    qDebug()<< "Save game \n";
    mw->saveGame(game);

    qDebug()<< "Game loading";
    mw->loadGame();

    return *game;
}

void AddDialog::on_pushButton_3_clicked()
{

    this->close();
}
