#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "MyThread.h"
#include "Game.h"
#include "Dialog/SelectTypeDialog.h"

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
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set stylesheet listWidget

    ui->listWidget->setStyleSheet(
                "QListWidget::item {"
                   "max-width: 80px;"
                   "background: white;"
                   "border-bottom: 2px solid black;"
                   "border-top-right-radius: 7px;"
                   "margin-top: 10px;"
                   "min-height: 32px;"
                "}"
                "QListWidget::item:hover {"
                "border-left: 2px solid black;"
                "border-bottom: 0;"
                "}"
                "QListWidget::item:selected {"
                   "background: qlineargradient( x1:0 y1:0, x2:1 y2:0, stop:0 grey, stop:1 #d6d6d6);"
                "}");
    ui->teDesc->setFrameStyle(QFrame::NoFrame);
    ui->lwType->setFrameStyle(QFrame::NoFrame);
    loadList();

    //Right click to lwGame
    wid = ui->listWidget;
    wid->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_listWidget_customContextMenuRequested(const QPoint&)));

    //Set property of item in twUserInfo
    QTabWidget *m_tabWidget = ui->twUserGame;
    m_tabWidget->setTabText(0, "Main information");
    m_tabWidget->setTabText(1, "Game information");
    m_tabWidget->setTabText(2, "Other");

    loadgameFromFile();
    int count = 0;
    for(int i = 0; i < ui->listWidget->count(); i++){
        count++;
    }
    ui->lNumbergame->setText(QString::number(count));

    listMenu->setTitle("Tableau");
    listMenu->addAction(action_itemAdd_listMenu);
    listMenu->addAction(action_itemReload_listMenu);

    itemMenu->setTitle("Game");
    itemMenu->addAction(action_itemFavori_itemMenu);
    itemMenu->addAction(action_itemDel_itemMenu);
    itemMenu->addAction(action_itemRemoveFromCollection_itemMenu);
    itemMenu->addAction(action_itemProperty_itemMenu);

    collectionMenu->setTitle("Collection");
    collectionMenu->addAction(action_addToCollection_collectionMenu);
    collectionMenu->addAction(action_renameCollection_colectionMenu);
    collectionMenu->addAction(action_deleteCollection_collectionMenu);
    collectionMenu->addAction(action_reduceCollection_collectionMenu);
    collectionMenu->addAction(action_displayCollection_collectionMenu);
    collectionMenu->addAction(action_reduceAllCollections_collectionMenu);
    collectionMenu->addAction(action_developAllCollections_collectionMenu);

    listMenu->addMenu(itemMenu);
    listMenu->addMenu(collectionMenu);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbAdd_clicked()
{
    AddDialog *addDialog = new AddDialog;
    addDialog->exec();
}


void MainWindow::on_pbDel_clicked()
{

    QFile file("save.json");

    loadgameFromFile();

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
    //ui->lwinfo->clear();

    listGame.clear();
    loadgameFromFile();

    file.remove();

    for(auto item : listGame){
        if(item->name() != selectGame->name())
                saveGame(item);
    }
    loadList();
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
    json = game->toJson(*game);

    if(!jsonArray.contains(json) || json.value("name").toString() != game->name() || json.value("date").toString() != game->date())
        jsonArray.append(json);
    else {
        int count = 0;
        for(auto item : jsonArray){
            if(item.toObject() == json){
                jsonArray.replace(count, json);
            }
            count ++;
        }
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    stream << QJsonDocument(jsonArray).toJson();
    file.close();

}

void MainWindow::loadgameFromFile(){

    QFile file("save.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(QJsonValue value : jsonArray){
        if(!value.isObject())
            continue;

        Game *game = new Game();

        *game = game->fromJson(value.toObject());
        for(auto i : game->type())
            qDebug() << game->type();
        listGame.append(game);
    }
}

void MainWindow::loadList(){

    //Destroy all widget and layout to clean dashboard
    qDeleteAll(ui->listGameLayout->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));

    listGame.clear();

    loadgameFromFile();
    ui->listWidget->clear();

    for(Game *item : listGame){
        ui->listWidget->addItem(item->name());
    }
    ui->listWidget->sortItems();
}

void MainWindow::startProgram(){
    oStartTime = QDateTime::currentDateTime();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    /* Change description after click on other game
    if(selectGame)
        saveDescGame();
    */
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

            //Get size of folder
            QString directoryPath = object.toObject().value("directory").toString();
            qint64 size = dirSize(directoryPath);

            //File tabInfoUser
            ui->lLastUse->setText(object.toObject().value("dateLastUse").toString());
            ui->lName->setText(object.toObject().value("name").toString());
            ui->teDesc->setText(object.toObject().value("desc").toString());
            ui->lTimePlayed->setText(QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec));
            if(object.toObject().value("dateLastUse").toString() == "")
                ui->lLastUse->setText("Never used");
            else
                ui->lLastUse->setText(object.toObject().value("date").toString());

            ui->lDate->setText(object.toObject().value("date").toString());
            ui->lDir->setText(object.toObject().value("directory").toString());
            ui->lPath->setText(object.toObject().value("path").toString());
            ui->lwType->clear();
            QListWidgetItem *it;
            for(int i = 0; i < object.toObject().value("type").toArray().count(); i++){
                it = new QListWidgetItem(object.toObject().value("type").toArray().at(i).toString());
                ui->lwType->addItem(it);
            }
            ui->lSizeFolder->setText(formatSize(size));

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
    qDebug() << "Launch";
    /*
    QString program =  selectGame->path().section("/", -1);
    QStringList arguments;
    QString workingDirectory = selectGame->directory();

    QProcess *process = new QProcess();


    arguments << "-f" << selectGame->path().section("/", -1);

    qDebug() << arguments;

    MyThread *thread = new MyThread;

    startProgram();

    process->startDetached(program
                           , arguments, workingDirectory);

    process->moveToThread(thread);
    thread->start();

    //connect(thread, SIGNAL(finished()), this, SLOT(saveTimePlayed(game)));
    //connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    */
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{

    QPoint globalpos = ui->listWidget->mapToGlobal(pos);

    QListWidgetItem* pointedItem = ui->listWidget->itemAt(pos);

    QAction* selectedAction;
    //Add item
    if(!pointedItem)
        itemMenu->setEnabled(false);

    selectedAction = listMenu->exec(globalpos);

    if(selectedAction == action_itemAdd_listMenu) {
        AddDialog *addDialog = new AddDialog;
        addDialog->show();
    }

    if(selectedAction == action_itemReload_listMenu)
        loadList();

    if(selectedAction == action_itemProperty_itemMenu) {

    }

    if(selectedAction == action_itemDel_itemMenu) {
        auto item = ui->listWidget->itemAt(pos);
        for(auto itemG : listGame){
            if(item->text() == itemG->name()){
                selectGame = itemG;
            }
        }
        on_pbDel_clicked();
    }

    if(selectedAction == action_itemFavori_itemMenu){
        qDebug() << "Favori";
    }

    itemMenu->setEnabled(true);
}
//Calculate and convert size of folder
qint64 MainWindow::dirSize(QString dirPath) {
    qint64 size = 0;
    QDir dir(dirPath);
    //calculate total size of current directories' files
    QDir::Filters fileFilters = QDir::Files|QDir::System|QDir::Hidden;
    for(QString filePath : dir.entryList(fileFilters)) {
        QFileInfo fi(dir, filePath);
        size+= fi.size();
    }
    //add size of child directories recursively
    QDir::Filters dirFilters = QDir::Dirs|QDir::NoDotAndDotDot|QDir::System|QDir::Hidden;
    for(QString childDirPath : dir.entryList(dirFilters))
        size+= dirSize(dirPath + QDir::separator() + childDirPath);
    return size;
}

QString MainWindow::formatSize(qint64 size) {
    QStringList units = {"Bytes", "KB", "MB", "GB", "TB", "PB"};
    int i;
    double outputSize = size;
    for(i=0; i<units.size()-1; i++) {
        if(outputSize<1024) break;
        outputSize= outputSize/1024;
    }
    return QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
}

void MainWindow::on_pbAddType_clicked()
{
    SelectTypeDialog *st = new SelectTypeDialog;
    QList<QString> list;
    st->exec();

    if(st->listType.isEmpty())
        return;

    for(auto i : st->listType){
        list.append(i);
    }

    Game *game = new Game();
    QJsonObject json = game->toJson(*game);
    game = new Game(selectGame->name(), selectGame->directory(), selectGame->path(), selectGame->date(), selectGame->desc(), list);
    listGame.removeOne(selectGame);
    json.swap(json);
    selectGame->changeType(json, list);
    listGame.append(game);
    QListWidgetItem *it = new QListWidgetItem(game->name());

    on_listWidget_itemClicked(it);

}

void MainWindow::saveDescGame(){

    if(!selectGame)
        return;

    selectGame->changeDesc(selectGame->toJson(*selectGame), descChange);
    on_pbDel_clicked();
    saveGame(selectGame);
    listGame.removeOne(selectGame);
    loadList();

}
void MainWindow::on_teDesc_textChanged()
{
    descChange.append(ui->teDesc->toPlainText());
}
