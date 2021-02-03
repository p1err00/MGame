#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDate>
#include <QTextEdit>
#include <QFormLayout>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDialog>
#include <QProcess>
#include <QStyle>
#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>
#include <QSpacerItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVector>
#include <QThread>
#include <QByteArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){

    closeEvent(event);
    //saveGame();
}

void MainWindow::on_pbAdd_clicked()
{

    AddDialog *addDialog = new AddDialog;
    addDialog->show();
}


void MainWindow::on_pbDel_clicked()
{
    //listGame.removeOne(selectGame);
}



void MainWindow::saveGame(Game *game){

    QFile file("save.json");
    QJsonArray jsonArray;

    if(file.exists()){

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream stream(&file);
        jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
        file.close();
        file.remove();
    }

    QJsonObject json;

    json.insert("name", game->name());
    json.insert("directory", game->directory());
    json.insert("path", game->path());
    json.insert("date", game->date());
    json.insert("timePlayed", QString::number(game->timePlayed()));

    if(jsonArray.contains(json)){
        qDebug() << "game exist";
    } else {
        jsonArray.append(json);
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    stream << QJsonDocument(jsonArray).toJson();
    file.close();

}

void MainWindow::loadGame(){

    QFile file("save.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(QJsonValue value : jsonArray){
        if(!value.isObject())
            continue;

        QJsonObject json = value.toObject();
        QString name = json.value("name").toString();
        QString directory = json.value("directory").toString();
        QString path = json.value("path").toString();
        QString date = json.value("date").toString();
        int timePLayed = json.value("timePlayed").toInt();
        Game *game = new Game(name, directory, path, date, timePLayed);

        listGame.append(*game);

    }
    file.close();

}

void MainWindow::on_pbLoad_clicked()
{
    listGame.clear();

    loadGame();

    qDeleteAll(ui->listGameLayout->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->destroyed(layout->widget());

    QPushButton *btnLaunch = new QPushButton("Launch");

    for(Game item : listGame){

        QPushButton *pbLaunch = new QPushButton;
        ui->launchLayout->removeWidget(pbLaunch);

        pbLaunch->setText(item.name());
        pbLaunch->setFixedSize(300, 30);

        //Create button connexion for each object in my file
        connect(pbLaunch, &QPushButton::clicked, [=](){

            //Insert name game into selectGame
            selectGame = item.name();

            //Insert value into dashboard
            ui->lName->setText(item.name());
            ui->lDirectory->setText(item.directory());
            ui->lTimePlayed->setText(QString::number(item.timePlayed()));
            ui->lDateInstall->setText(item.date());

        });

        layout->addWidget(pbLaunch);

        //add launch button
        ui->launchLayout->addWidget(btnLaunch);

        connect(btnLaunch, &QPushButton::clicked, [=](){
            on_pbLaunch_clicked(item);

        });
        ui->verticalLayout_2->removeWidget(btnLaunch);
    }
    ui->listGameLayout->addLayout(layout);

}

void MainWindow::on_pbLaunch_clicked(Game game)
{
    qDebug() << "Launch game";

    //init timer & process
    QDateTime *dateTimeStart = new QDateTime;
    QProcess *process = new QProcess;
    QProcess *cmdProcess = new QProcess;

    process->startDetached("cmd /c start "+game.path());

    dateTimeStart->currentDateTime().toString();
    qDebug() << "Timer start : " + dateTimeStart->currentDateTime().toString();

}
