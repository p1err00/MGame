#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "Thread.h"

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

void MainWindow::on_pbAdd_clicked()
{

    AddDialog *addDialog = new AddDialog;
    addDialog->show();
}


void MainWindow::on_pbDel_clicked()
{
    /*
    qDebug() << selectGame;
    for (int i = 0; i<listGame.size(); i++) {
        qDebug() << "for";
        qDebug() << listGame.takeAt(i)->name();
        if(listGame.takeAt(i)->name() == selectGame){
            qDebug() << "if";

            listGame.removeAt(i);
        }
    }
    */
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

        listGame.append(game);

    }
    file.close();

}

void MainWindow::on_pbLoad_clicked()
{

    qDeleteAll(ui->listGameLayout->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));
    listGame.clear();

    loadGame();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->destroyed(layout->widget());

    QPushButton *pbDisplay;

    for(Game *item : listGame){

        pbDisplay = new QPushButton;
        ui->launchLayout->removeWidget(pbDisplay);

        pbDisplay->setText(item->name());
        pbDisplay->setFixedSize(300, 30);

        layout->addWidget(pbDisplay);

        //Create button connexion for each object in my file
        connect(pbDisplay, &QPushButton::clicked, [=](){

            //Insert name game into selectGame
            selectGame = item;
            qDebug() << item->name();

            displayGame(selectGame);
        });

    }
    ui->listGameLayout->addLayout(layout);
}

void MainWindow::displayGame(Game *game){

    //Clear all game's info
    qDeleteAll(ui->verticalLayout_2->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));

    QPushButton *btn;

    //add launch button
    btn = new QPushButton("launch");


    ui->verticalLayout_2->addWidget(btn);
    //Insert value into dashboard
    ui->lName->setText(game->name());
    ui->lDirectory->setText(game->directory());
    ui->lTimePlayed->setText(QString::number(game->timePlayed()));
    ui->lDateInstall->setText(game->date());

    //Set the current game
    selectGame = game;

    connect(btn, &QPushButton::clicked, [=](){
        qDebug() << selectGame->path();
        pbLaunchClicked(game);
    });

}

void MainWindow::pbLaunchClicked(Game *game){

    QProcess *process = new QProcess;
    QStringList arguments;
    arguments << "start /c" << game->path();
    qDebug() << arguments;
    QString program = "cmd.exe";
    QThread *thread = new QThread;
    process->startDetached(program, arguments);
    process->moveToThread(thread);
    thread->start();
    qDebug() << QThread::currentThreadId();
    connect(thread, SIGNAL(finished()), this, SLOT(finishProgram()));

    if(thread->isFinished()){
        qDebug() << "Finish";
    }

}

void MainWindow::finishProgram(){

    qDebug() << "Ca marche";
}
