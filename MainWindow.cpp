#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "EditDialog.h"
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
#include <QFont>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_pushButton_clicked();

    //Right click to lwGame
    wid = ui->listWidget;
    wid->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_listWidget_customContextMenuRequested(const QPoint&)));
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

    loadGame();

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    int count = 0;

    for(QJsonValue value : jsonArray){

        if(!value.isObject())
            continue;

        QJsonObject json = value.toObject();

        if(selectGame == nullptr)
            QMessageBox::warning(this, "Warning", selectGame->name());

        if(json.contains(selectGame->name()) && json.value("path") == selectGame->path()){

            jsonArray.removeAt(value.toObject().count());
        }
        count++;
    }
    ui->lwinfo->clear();

    listGame.clear();
    loadGame();

    file.remove();

    for(auto item : listGame){
        if(item->name() != selectGame->name())
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
    json.insert("directory", game->directory());
    json.insert("path", game->path());
    json.insert("date", game->date());
    json.insert("timePlayed", game->timePlayed());
    json.insert("dateLastuse", game->dateLastUse());
    json.insert("desc", game->desc());

    if(!jsonArray.contains(json) || json.value("name").toString() != game->name() || json.value("date").toString() != game->date())
        jsonArray.append(json);
    else
        saveTimePlayed(game);

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
        int timePlayed = json.value("timePlayed").toInt();
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
    ui->listWidget->sortItems();
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
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(auto item : jsonArray){

        if(!item.isObject())
            continue;

        QJsonObject rootJson = item.toObject();

        if(rootJson.value("name").toString() == game->name()){

            //Call caclulate tim function

            QJsonValue value = timeDiff + item.toObject().value("timePlayed").toInt();

            QList<QString> type;
            type.append("action");
            type.append("aventure");
            type.append("RPG");
            type.append("Mes couilles");

            //rootJson.insert("timePlayed", value.toInt());

            listGame.removeOne(game);

            game = new Game(game->name(), game->directory(), game->path(), game->date(), value.toInt(), game->dateLastUse(), game->desc(), type);

            jsonArray.removeAt(item.toObject().count());

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

            //Fill lwInfo
            ui->lwinfo->clear();


            QList<QString> listDesc;
            listDesc << object.toObject().value("name").toString()
                     << object.toObject().value("desc").toString()
                     << "Date d'installation : \t" + object.toObject().value("date").toString()
                     << "Derniere utilisation : \t" + object.toObject().value("dateLastuse").toString()
                     << "Temps de jeu : \t\t" + QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec)
                     << "Type : \t" + object.toObject().value("type").toString()
                     << "Folder : \t" + object.toObject().value("path").toString();

            //Set property of item in lwInfo


            ui->lwinfo->addItems(listDesc);

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

    QString program = "\"" + selectGame->path().section("/", -1) + "\"";

    qDebug() << program;

    QProcess *process = new QProcess();
    QStringList arguments;

    arguments << "cd /d " << selectGame->directory() << " start /c " << selectGame->path().section("/", -1);

    qDebug() << arguments;

    MyThread *thread = new MyThread;

    startProgram();

    process->execute(program, arguments);
    qDebug() << process->exitStatus();

    process->moveToThread(thread);
    thread->start();

    //connect(thread, SIGNAL(finished()), this, SLOT(saveTimePlayed(game)));
    //connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    saveTimePlayed(selectGame);
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
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(auto item : jsonArray){
        if(!item.isObject())
            continue;

        QJsonObject rootJson = item.toObject();

        if(rootJson.value("name").toString() == game->name()){

            //Call calculate time function

            QDate value = QDate::currentDate();

            QList<QString> type;
            type.append("action");
            type.append("aventure");
            type.append("RPG");
            type.append("Mes couilles");

            listGame.removeOne(game);

            game = new Game(game->name(), game->directory(), game->path(), game->date(), game->timePlayed(), value.toString(), game->desc(), type);

            jsonArray.removeAt(item.toObject().count());

            on_pbDel_clicked();

            saveGame(game);
            loadList();
        }
    }
}


//////////////////////////////////////////////////////////////////////***Game install***///////////////////////////////////////////////////////////////////////////////




void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalpos = ui->listWidget->mapToGlobal(pos);

        QMenu menuBeyondItem;
        QAction* action_addElement = menuBeyondItem.addAction("Add");

        QMenu menuForItem;
        QAction* action_editElement = menuForItem.addAction("Edit");
        QAction* action_deleteElement = menuForItem.addAction("Delete");

        QListWidgetItem* pointedItem = ui->listWidget->itemAt(pos);

        QAction* selectedAction;

        //Add item
        if(!pointedItem) {
            selectedAction = menuBeyondItem.exec(globalpos);
            if(selectedAction) {
                if(selectedAction == action_addElement) {
                    AddDialog *addDialog = new AddDialog;
                    addDialog->show();
                }
            }
        }
        //Edit item
        else {
            selectedAction = menuForItem.exec(globalpos);
            if(selectedAction) {
                if(selectedAction == action_editElement) {
                    EditDialog *edit = new EditDialog;
                    auto item = ui->listWidget->itemAt(pos);
                    for(auto itemG : listGame){
                        if(item->text() == itemG->name()){
                            qDebug() << "item selected";
                            selectGame = itemG;
                            edit->game = selectGame;
                            edit->show();
                        }
                    }

                }
                else/*(selectedAction == action_editElement)*/ {
                    qDebug() << "Delete";
                    auto item = ui->listWidget->itemAt(pos);
                    for(auto itemG : listGame){
                        if(item->text() == itemG->name()){
                            selectGame = itemG;
                        }
                    }
                    on_pbDel_clicked();
                }
            }
        }
}
