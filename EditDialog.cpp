#include "EditDialog.h"
#include "ui_EditDialog.h"


#include <QDebug>

EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);


    Game *newGame = game;
    qDebug() << newGame->name();
    //ui->ldate->setText(game->date());


}

EditDialog::~EditDialog()
{
    delete ui;
}

QString EditDialog::on_teDesc_textChanged()
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

Game EditDialog::on_buttonBox_accepted()
{
    QString desc = on_teDesc_textChanged();

    _n = ui->leName->text();
    QList<QString> l;
    l << "action";


    MainWindow *mw = new MainWindow;

    mw->listGame.append(game);

    qDebug()<< "Save game \n";
    mw->saveGame(game);

    qDebug()<< "Game loading";
    mw->loadGame();
    return *game;
}

Game EditDialog::on_leName_textChanged(const QString &arg1)
{
    ui->lname->setText(arg1);
    return *game;
}
