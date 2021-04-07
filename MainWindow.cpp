#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "MyThread.h"
#include "Game.h"
#include "Dialog/SelectTypeDialog.h"
#include "Collection/Collection.h"
#include "Settings/SettingsDialog.h"
#include "Settings/FavoriDialog.h"
#include "WorkerThread.h"
#include "TransmissionProcess.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QLineEdit>
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
#include <QFuture>
#include <QtConcurrent/QtConcurrent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->leDesc->setFrameStyle(QFrame::NoFrame);
    ui->lwType->setFrameStyle(QFrame::NoFrame);

    //Right click to lwGame
    wid = ui->listWidget;
    wid->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_listWidget_customContextMenuRequested(const QPoint&)));

    //Set property of item in twUserInfo
    QTabWidget *m_tabWidget = ui->twUserGame;
    m_tabWidget->setTabText(0, "Game");
    m_tabWidget->setTabText(1, "Torrent");
    m_tabWidget->setTabText(2, "Network");

    loadgameFromFile();
    int count = 0;
    for(int i = 0; i < ui->listWidget->count(); i++){
        count++;
    }
    ui->lNumbergame->setText(QString::number(count));

    listMenu->setTitle("Tableau");
    listMenu->addAction(action_itemAdd_listMenu);
    listMenu->addAction(action_itemReload_listMenu);
    listMenu->addAction(action_itemDel_itemMenu);

    itemMenu->setTitle("Game");
    itemMenu->addAction(action_itemFavori_itemMenu);
    itemMenu->addAction(action_itemRemoveFromCollection_itemMenu);
    itemMenu->addAction(action_itemProperty_itemMenu);

    collectionMenu->setTitle("Collection");
    collectionMenu->addAction(action_createCollection_collectionMenu);
    collectionMenu->addAction(action_addToCollection_collectionMenu);
    collectionMenu->addAction(action_renameCollection_colectionMenu);
    collectionMenu->addAction(action_deleteCollection_collectionMenu);
    collectionMenu->addAction(action_reduceCollection_collectionMenu);
    collectionMenu->addAction(action_displayCollection_collectionMenu);
    collectionMenu->addAction(action_reduceAllCollections_collectionMenu);
    collectionMenu->addAction(action_developAllCollections_collectionMenu);

    listMenu->addMenu(itemMenu);
    listMenu->addMenu(collectionMenu);
    collectionMenu->setEnabled(false);

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

void MainWindow::reloadGame(){

    selectGame->types();
    if(selectGame == nullptr)
        QMessageBox::warning(this, "Warning", selectGame->name());

    QFile file("save.json");

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

        if(json.value("path") == selectGame->path()){
            qDebug() << "Find";
            jsonArray.removeAt(count);
            listGame.removeAt(count);
        }
        count++;
    }
    QJsonDocument doc = QJsonDocument(jsonArray);
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
    loadgameFromFile();
}

void MainWindow::on_pbDel_clicked()
{

    if(selectGame == nullptr)
        QMessageBox::warning(this, "Warning", selectGame->name());

    QFile file("save.json");

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

        if(json.value("path") == selectGame->path()){
            qDebug() << "Find";
            jsonArray.removeAt(count);
            listGame.removeAt(count);
            ui->listWidget->removeItemWidget(ui->listWidget->item(count));
        }
        count++;
    }

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
    json = game->toJson();

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

        game->fromJson(value.toObject());

        for(auto i : game->types()){
            qDebug() << i;
        }

        listGame.append(game);
    }
}

void MainWindow::loadList(){

    //Destroy all widget and layout to clean dashboard
    qDeleteAll(ui->listGameLayout->findChildren<QObject *>(QString(), Qt::FindDirectChildrenOnly));

    listGame.clear();

    loadgameFromFile();
    ui->listWidget->clear();
    QListWidgetItem *it;
    for(Game *item : listGame){
        it = new QListWidgetItem(item->name());
        ui->listWidget->addItem(it);
    }
    ui->listWidget->sortItems();
    loadFavList();
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

            //Get size of folder
            QString directoryPath = object.toObject().value("directory").toString();
            qint64 size = dirSize(directoryPath);

            //File tabInfoUser
            ui->lLastUse->setText(object.toObject().value("dateLastUse").toString());
            ui->lName->setText(object.toObject().value("name").toString());
            ui->leDesc->setText(object.toObject().value("desc").toString());
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

    if(!selectGame)
        return;

    QString program =  selectGame->path();
    QStringList arguments;
    QString workingDirectory = selectGame->directory();

    QProcess *process = new QProcess();


    arguments << "start /c " << selectGame->path().section("/", -1);

    qDebug() << arguments;

    MyThread *thread = new MyThread;

    QDateTime startProcess =QDateTime::currentDateTime();

    process->execute(program, arguments);
    process->moveToThread(thread);

    thread->start();

    connect(thread, &QThread::finished, [=](){
        qDebug() << "Go to save time";

        QDateTime stopProcess = QDateTime::currentDateTime();

        quint64 timeDiff = calculateTime(startProcess, stopProcess);

        QDate value = QDate::currentDate();

        //int timePlayed = json.value("timePlayed").toInt();
        //timePlayed += timeDiff;

        Game *game = selectGame;
        game->setTimePlayed(selectGame->timePlayed() + timeDiff);
        game->setDateLastUse(value.toString());
        QList<QString> list;
        for(int i = 0; i < ui->lwType->count(); i++){

            list.append(ui->lwType->item(i)->text());

        }
        game->setTypes(list);

        reloadGame();
        saveGame(selectGame);
        loadgameFromFile();
        loadList();
    });

    //connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));

}

//Calculate difference between start and stop time program
quint64 MainWindow::calculateTime(QDateTime startProcess, QDateTime stopProcess){

    quint64 difference = qAbs(startProcess.date().daysTo(stopProcess .date()));
    difference  *= static_cast<quint64>(24); // days to hours
    difference  *= static_cast<quint64>(60); // hours to minutes
    difference  *= static_cast<quint64>(60); // minutes to seconds
    difference  *= static_cast<quint64>(1000); // seconds to milliseconds
    difference += qAbs(startProcess.time().msecsTo(stopProcess .time()));

    qDebug() << difference;
    return difference;
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
        Game *game;
        SettingsDialog *st = new SettingsDialog();
        st->displayGame(selectGame);
        st->exec();
        *selectGame = st->on_buttonBox_accepted();
        qDebug() << selectGame->name();
        game = selectGame;
        reloadGame();
        saveGame(game);
        on_pbDel_clicked();
        loadgameFromFile();
        loadList();
    }

    if(selectedAction == action_itemDel_itemMenu) {
        auto item = ui->listWidget->itemAt(pos);
        for(auto itemG : listGame){
            if(item->text() == itemG->name()){
                qDebug() << "IF";
                selectGame = itemG;
                on_pbDel_clicked();
                reloadGame();
                loadList();
            }
        }

    }

    if(selectedAction == action_itemFavori_itemMenu){
        FavoriDialog *fd = new FavoriDialog;
        if(pointedItem->icon().isNull()){

            pointedItem->setIcon(QIcon("star.png"));
            favList = fd->addFav(favList, pointedItem->text());
            fd->saveFile(favList);
            qDebug() << favList;
         } else{
            favList = fd->delFav(favList, pointedItem->text());
            pointedItem->setIcon(QIcon());
            qDebug() << favList;

        }
    }

    if(selectedAction == action_createCollection_collectionMenu){
        Collection *collection = new Collection("");
        collection->createCollection();
        if(collection->getName().isEmpty())
            return;
        listCollection.append(collection);

        displayCollectionInLayout(collection);
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

    for(auto i : st->listType)
        list.append(i);

    //Change type
    Game *game = selectGame;
    game->setTypes(list);
    reloadGame();
    saveGame(game);
    loadgameFromFile();
    loadList();
}

void MainWindow::saveDescGame(){

    Game *game = selectGame;
    game->setDesc(ui->leDesc->text());
    reloadGame();
    loadgameFromFile();
    loadList();

}

void MainWindow::displayCollectionInLayout(Collection *collection){

    QListWidget *wCollection = new QListWidget;
    QLabel *lNameCollection = new QLabel(collection->getName());

    ui->layoutCollection->addWidget(lNameCollection);
    ui->layoutCollection->addWidget(wCollection);
}

void MainWindow::loadFavList(){
    int count = 0;
    for(int i = 0; i < ui->listWidget->count(); i++){
        for(auto is : favList){
            if(is.isNull())
                continue;
            if(ui->listWidget->item(count)->text() == is){
                ui->listWidget->item(count)->setIcon(QIcon("star.png"));
            }
        }
        count++;
    }
}

void MainWindow::loadTransmission(){

    TransmissionProcess tp;
    //QFunctionPointer p = tp.launch();
    //QFuture<void> future = QtConcurrent::run(p);
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    qDebug() << arg1;


}
