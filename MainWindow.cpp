#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "MyThread.h"

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
#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pushButton_clicked();
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

        if(selectGame == nullptr)
            QMessageBox::warning(this, "Warning", selectGame->name());

        if(json.value("name") == selectGame->name()){
            listGame.removeOne(selectGame);
            file.remove();
        }
    }

    ui->listGameLayout->destroyed(ui->listGameLayout->widget());
    for(auto item : listGame){
        saveGame(item);
    }

    on_pushButton_clicked();
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
    json.insert("tdirectory", game->directory());
    json.insert("path", game->path());
    json.insert("date", game->date());
    json.insert("timePlayed", game->timePlayed());
    json.insert("dateLastuse", game->dateLastUse());
    json.insert("desc", game->desc());

    if(!jsonArray.contains(json) && json.value("name").toString() == game->name() && json.value("date").toString() == game->date())
        jsonArray.append(json);

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
        QString time = json.value("timePlayed").toString();
        int timePlayed = time.toInt();
        QString dateLastUse = json.value("dateLastUse").toString();
        QString desc = json.value("desc").toString();
        QList<QString> type;
        type.append("action");
        type.append("aventure");
        type.append("RPG");
        type.append("Mes couilles");

        Game *game = new Game(name, directory, path, date, timePlayed, dateLastUse, desc, type);
        listGame.append(game);
    }
    file.close();
}

void MainWindow::loadList(){

    //Destroy all widget and layout to clean dashboard
    qDeleteAll(ui->listGameLayout->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));

    listGame.clear();

    loadGame();
    ui->listWidget->clear();

    for(Game *item : listGame){
        ui->listWidget->addItem(item->name());
    }
}


void MainWindow::startProgram(){
    oStartTime = QDateTime::currentDateTime();
}


void MainWindow::saveTimePlayed(Game *game){

    QDateTime oEndtTime = QDateTime::currentDateTime();

    int timeDiff = calculateTime(oStartTime, oEndtTime);

    qDebug() << "Enter into save function";
    QFile file("save.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonDocument jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8());
    file.close();

    for(auto item : jsonArray.array()){
        if(!item.isObject())
            continue;

        QJsonObject rootJson = item.toObject();

        if(rootJson.value("name").toString() == game->name()){

            qDebug() << "Find my game";
            //Call caclulate tim function

            listGame.removeOne(game);

            QJsonValue value = timeDiff + item.toObject().value("timePlayed").toInt();

            QList<QString> type;
            type.append("action");
            type.append("aventure");
            type.append("RPG");
            type.append("Mes couilles");

            game = new Game(game->name(), game->directory(), game->path(), game->date(), value.toInt(), game->dateLastUse(), game->desc(), type);

            rootJson.remove(game->name());

            selectGame = game;

            on_pbDel_clicked();

            saveGame(game);

            changeDateLastUse(selectGame);

            loadList();

        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    loadList();
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    QFile file("save.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(auto object : jsonArray){
        if(!object.isObject())
            continue;

        if(item->text() == object.toObject().value("name").toString()){
            ui->lName->setText(object.toObject().value("name").toString());
            ui->lDirectory->setText(object.toObject().value("directory").toString());

            //Convert int into time
            int ti = object.toObject().value("timePlayed").toInt();
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
            ui->lTimePlayed->setText(QString::number(hour)+" : "+QString::number(min)+" : " + QString::number(sec));
            ui->lDateInstall->setText(object.toObject().value("date").toString());
            ui->lLastUse->setText(object.toObject().value("dateLastuse").toString());
            ui->ldesc->setText(object.toObject().value("desc").toString());

            //Set the current game
            for(auto game : listGame){
                if(game->name() == item->text()){
                    selectGame = game;
                }
            }
        }
    }
}

void MainWindow::on_pbLaunch_clicked()
{

    qDebug() << selectGame->name();

    QString program = selectGame->path();

    QProcess *process = new QProcess();
    QStringList arguments;

    arguments << "start /c";

    MyThread *thread = new MyThread;

    startProgram();

    process->execute(program, arguments);

    process->moveToThread(thread);
    thread->start();

    connect(thread, SIGNAL(finished()), this, SLOT(saveTimePlayed(game)));
    //connect(thread, SIGNAL(finished()), this, SLOT(changeDateLastUse(game)));
    saveTimePlayed(selectGame);
    //changeDateLastUse(selectGame);
}

quint64 MainWindow::calculateTime(QDateTime oStartTime, QDateTime oEndtTime){

    quint64 difference = qAbs(oStartTime.date().daysTo(oEndtTime .date()));
    difference  *= static_cast<quint64>(24); // days to hours
    difference  *= static_cast<quint64>(60); // hours to minutes
    difference  *= static_cast<quint64>(60); // minutes to seconds
    difference  *= static_cast<quint64>(1000); // seconds to milliseconds
    difference += qAbs(oStartTime.time().msecsTo(oEndtTime .time()));

    qDebug() << difference;
    return difference;
}

void MainWindow::changeDateLastUse(Game *game){

    qDebug() << "Enter into change function";
    QFile file("save.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonDocument jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8());
    file.close();

    for(auto item : jsonArray.array()){
        if(!item.isObject())
            continue;

        QJsonObject rootJson = item.toObject();

        if(rootJson.value("name").toString() == game->name()){

            qDebug() << "Find my game";
            //Call caclulate tim function

            listGame.removeOne(game);

            QDate value = QDate::currentDate();

            QList<QString> type;
            type.append("action");
            type.append("aventure");
            type.append("RPG");
            type.append("Mes couilles");

            game = new Game(game->name(), game->directory(), game->path(), game->date(), game->timePlayed(), value.toString(), game->desc(), type);

            rootJson.remove(selectGame->name());

            on_pbDel_clicked();

            saveGame(game);
            loadList();

        }
    }
}
