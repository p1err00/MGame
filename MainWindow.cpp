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

void MainWindow::list(Game *game)
{



}

void MainWindow::on_pbDel_clicked()
{



    //on_pbList_clicked();


    //saveGame();

}

void MainWindow::processFinished(int code , QProcess::ExitStatus status, QDateTime dateTimeStart){

    qDebug() << code;
    qDebug() << status;
    QDateTime *dateTimeStop = new QDateTime;

}

void MainWindow::saveGame(Game *game){

    QFile file("file.json");

    if(!file.exists()){
        QFile file("file.json");
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        file.errorString();

        QJsonObject jsonObject;


        jsonObject.insert("name", game->name());
        jsonObject.insert("directory", game->directory());
        jsonObject.insert("path", game->path());
        jsonObject.insert("date", game->date());
        jsonObject.insert("timePlayed", QString::number(game->timePlayed()));
        qDebug() << game->name();

        QJsonArray array = QJsonDocument::fromJson(file.fileName().toUtf8()).array();
        array.append(jsonObject);
        qDebug()<< array;
        QJsonDocument doc;
        doc.setArray(array);
        file.write(doc.toJson());

    file.close();

}

void MainWindow::loadGame(){


    qDebug() << "Load game";
    QFile file;
    file.setFileName("file.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Append)){
        file.errorString();
    } else {

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        QJsonArray array = doc.array();

        QJsonObject json;
        for()
        json.value("name").toString();

        qDebug() << array;

        foreach (const QJsonValue & val, array) {
            QString valDoc;

            QString name = val.toObject().value("name").toString();
            qDebug() << name;
            QString directory = val.toObject().value("directory").toString();
            QString path = val.toObject().value("path").toString();
            QString date = val.toObject().value("date").toString();
            int timePlayed = val.toObject().value("timePlayed").toInt();
            Game *game = new Game(name, directory, path, date);
            listGame.append(*game);

        }
        doc.setObject(jsonObject);
        doc.setArray(array);

        qDebug() << "Finishing parsing document";

        QPushButton *pbName = new QPushButton;

        for(Game item : listGame){

            qDebug()<< item.name();
            pbName->setText(item.name());


            QObject::connect(pbName, &QPushButton::clicked, [=](){
                qDebug()<<"Ca connecte";

            });
        }
        qDebug() << "Display button";
    ui->listGame->addWidget(pbName);
    }

    file.close();


}
void MainWindow::launchGame(Game *game){

    //init timer & process
    QDateTime *dateTimeStart = new QDateTime;
    QProcess *process = new QProcess;
    QProcess *cmdProcess = new QProcess;

    process->startDetached("cmd /c start "+game->path());
    process->waitForFinished();

    //process->execute("cmd /c start "+game->path());


    dateTimeStart->currentDateTime().toString();
    qDebug() << "Timer start : " + dateTimeStart->currentDateTime().toString();


}



void MainWindow::gameStop(){

    qDebug() << "Dis moi que ca marche";
}


void MainWindow::on_pbSave_clicked()
{
    //saveGame();
}

void MainWindow::on_pbLoad_clicked()
{
    loadGame();
}
