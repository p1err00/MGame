#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "MyThread.h"
#include "Game.h"
#include "Dialog/SelectTypeDialog.h"
#include "Settings/SettingsDialog.h"
#include "Settings/FavoriDialog.h"
#include "WorkerThread.h"
#include "TransmissionProcess.h"
#include "Settings/MainSettingsDialog.h"
#include "GameInstall/AddGameInstallDialog.h"
#include "GameInstall/GameInstallManager.h"
#include "Dialog/ChangeCouvertureDIalog.h"

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
#include <QStyle>
#include <QPixmap>


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

    loadgameFromFile();
    int count = 0;
    for(int i = 0; i < listGame.count(); i++){
        count++;
    }
    ui->lNumbergame->setText(QString::number(count));

    loadListGameInstall();

    listMenu->setTitle("Tableau");
    listMenu->addAction(action_itemAdd_listMenu);
    listMenu->addAction(action_itemReload_listMenu);
    listMenu->addAction(action_itemDel_itemMenu);

    itemMenu->setTitle("Game");
    itemMenu->addAction(action_itemFavori_itemMenu);
    itemMenu->addAction(action_itemProperty_itemMenu);

    itemInstallMenu->setTitle("Install");
    itemInstallMenu->addAction(action_itemAdd_itemInstallMenu);
    itemInstallMenu->addAction(action_itemReload_itemInstallMenu);
    itemInstallMenu->addAction(action_itemAddList_itemInstallMenu);
    itemInstallMenu->addAction(action_itemDel_itemInstallMenu);

    couvertureMenu->addAction(action_itemChange_couvertureMenu);

    listMenu->addMenu(itemMenu);

    QWidget *wid2;
    wid2 = ui->lwGameInstall;
    wid2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid2, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_lwGameInstall_customContextMenuRequested(const QPoint&)));

    QWidget *wid3;
    wid3 = ui->lCouverturel;
    wid3->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid2, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_lCouverturel_customContextMenuRequested(const QPoint &pos)));


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
    if(!selectGame)
        return;

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

    if(!selectGame)
        return;

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

void MainWindow::loadGameInstallFromFile(){

    QFile file("saveGameInstall.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(QJsonValue value : jsonArray){
        if(!value.isObject())
            continue;
        GameInstall *gameInstall = new GameInstall();

        gameInstall->fromJson(value.toObject());

        listGameInstall.append(gameInstall);
    }
}

void MainWindow::loadListGameInstall(){

    listGameInstall.clear();
    loadGameInstallFromFile();

    ui->lwGameInstall->clear();
    QListWidgetItem *it;
    for(GameInstall *gameInstall : listGameInstall){
        it = new QListWidgetItem(gameInstall->name());
        ui->lwGameInstall->addItem(it);
    }
    ui->lwGameInstall->sortItems();

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
            ui->leDesc->setWordWrap(1);
            ui->leDesc->setText(object.toObject().value("desc").toString());

            ui->lTimePlayed->setText(QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec));
            if(object.toObject().value("dateLastUse").toString() == "")
                ui->lLastUse->setText("Never used");
            else
                ui->lLastUse->setText(object.toObject().value("dateLastUse").toString());

            ui->lDate->setText(object.toObject().value("date").toString());
            ui->lDir->setText(object.toObject().value("directory").toString());
            ui->lPath->setText(object.toObject().value("path").toString());
            ui->lPath->setWordWrap(1);
            ui->lwType->clear();
            QListWidgetItem *it;
            for(int i = 0; i < object.toObject().value("type").toArray().count(); i++){
                it = new QListWidgetItem(object.toObject().value("type").toArray().at(i).toString());
                ui->lwType->addItem(it);
            }
            ui->lSizeFolder->setText(formatSize(size));
            QPixmap *pixPicture = new QPixmap(object.toObject().value("linkPicture").toString());
            *pixPicture = pixPicture->scaled(ui->lImage->size(), Qt::KeepAspectRatio);
            ui->lImage->setPixmap(*pixPicture);

            QPixmap *pixCouverture = new QPixmap(object.toObject().value("linkCouverture").toString());
            *pixCouverture = pixCouverture->scaled(ui->lCouverturel->size(), Qt::KeepAspectRatio);
            ui->lCouverturel->setPixmap(*pixCouverture);

            //Set image
            QString aze = "C:/Users/p1err0/Documents/build-Project-Desktop_Qt_5_12_6_MSVC2015_64bit-Debug/debug/img/cyberpunk.jpg";
            for(int i = 0; i < 5; i++){
                QPixmap *pixi = new QPixmap;
                QLabel *l = new QLabel;
                l->setPixmap(*pixi);
                ui->layouListScreen->addWidget(l);
            }
            //Set the current game

            for(auto game : listGame){
                if(game->name() == item->text()){
                    selectGame = game;
                }
            }
        }
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_lwGameInstall_itemClicked(QListWidgetItem *item)
{
    ui->stackedWidget->setCurrentIndex(1);

    QFile file("saveGameInstall.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    for(auto object : jsonArray){
        if(!object.isObject())
            continue;

        if(item->text() == object.toObject().value("name").toString()){
            ui->lNameInstall->setText(object.toObject().value("name").toString());
            ui->lFolderInstall->setText(object.toObject().value("folder").toString());
            ui->lSizeInstall->setNum(object.toObject().value("size").toInt());

            if(object.toObject().value("install").toBool()){
                ui->cbInstall->setCheckState(Qt::CheckState(true));
            } else {
                ui->cbInstall->setCheckState(Qt::CheckState(false));
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

    selectedAction = listMenu->exec(globalpos);

    if(!pointedItem)
        action_itemDel_itemMenu->setEnabled(false);

    if(selectedAction == action_itemAdd_listMenu) {
        AddDialog *addDialog = new AddDialog;
        addDialog->show();
        return;
    }

    if(selectedAction == action_itemReload_listMenu){
        loadList();
        return;
    }

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
        return;
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
        return;
    }

    if(selectedAction == action_itemDel_itemMenu) {
        auto item = ui->listWidget->itemAt(pos);
        for(auto itemG : listGame){
            if(item->text() == itemG->name()){
                selectGame = itemG;
                on_pbDel_clicked();

            }
        }
        reloadGame();
        loadList();
        return;
    }

    action_itemDel_itemMenu->setEnabled(true);
    return;
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
    QMessageBox::warning(this, "Warning", "Function not implemented yet");
}

void MainWindow::on_actionParametre_triggered()
{
    MainSettingsDialog *s = new MainSettingsDialog;

    s->show();
}

void MainWindow::closeEvent(QCloseEvent *event){

    saveFavori();
}

void MainWindow::saveFavori(){
    FavoriDialog fav;
    QFile file("favori.json");
    QList<QString> n_list;
    QJsonArray jsonArray;

    if(!file.open(QIODevice::ReadOnly))
        return;

    for(int i = 0; i < ui->listWidget->count(); i++){
        if(!ui->listWidget->item(i)->icon().isNull())
            n_list << ui->listWidget->item(i)->text();
    }

    file.remove();
    QJsonObject json;
    for(auto i : n_list){
        json.insert("fav", i);
        jsonArray.append(json);
    }

    jsonArray.append(json);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    stream << QJsonDocument(jsonArray).toJson();
    file.close();
}

void MainWindow::on_lwGameInstall_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "ca rentre";
    QPoint globalpos = ui->lwGameInstall->mapToGlobal(pos);

    QListWidgetItem* pointedItem = ui->lwGameInstall->itemAt(pos);

    QAction* selectedAction;

    selectedAction = itemInstallMenu->exec(globalpos);

    if(!pointedItem){
        action_itemDel_itemInstallMenu->setEnabled(false);
        action_itemAddList_itemInstallMenu->setEnabled(false);
    }
    if(selectedAction == action_itemAdd_itemInstallMenu){
        AddGameInstallDialog *ad = new AddGameInstallDialog;
        ad->show();
    }

    if(selectedAction == action_itemReload_itemInstallMenu){
        qDebug() << "Reload";
       loadListGameInstall();
    }

    action_itemDel_itemInstallMenu->setEnabled(true);
    action_itemAddList_itemInstallMenu->setEnabled(true);

}

void MainWindow::on_pbChangeImage_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Open", QApplication::applicationDirPath()+"\\img","*");

    QPixmap pix;
    pix.load(file.absoluteFilePath());
    if(pix.isNull())
        return;

    pix = pix.scaled(ui->lImage->size(), Qt::KeepAspectRatio);

    ui->lImage->setPixmap(pix);


    Game *game = selectGame;
    QList<QString> list;
    for(int i = 0; i < ui->lwType->count(); i++){

        list.append(ui->lwType->item(i)->text());

    }
    game->setTypes(list);
    game->setLinkPicture(file.absoluteFilePath());
    reloadGame();
    saveGame(game);
    loadgameFromFile();
    loadList();
}

void MainWindow::on_lCouverturel_customContextMenuRequested(const QPoint &pos)
{
    qDebug() << "ca rentre";
    QPoint globalpos = ui->lCouverturel->mapToGlobal(pos);

    QAction* selectedAction;
    selectedAction = couvertureMenu->exec(globalpos);

    if(selectedAction == action_itemChange_couvertureMenu){
        QFileInfo file = QFileDialog::getOpenFileName(this, "Open", QApplication::applicationDirPath()+"\\img","*");

        QPixmap pix;
        pix.load(file.absoluteFilePath());
        if(pix.isNull())
            return;

        pix = pix.scaled(ui->lImage->size(), Qt::KeepAspectRatio);

        ui->lCouverturel->setPixmap(pix);

        Game *game = selectGame;
        QList<QString> list;
        for(int i = 0; i < ui->lwType->count(); i++){

            list.append(ui->lwType->item(i)->text());

        }
        game->setTypes(list);
        game->setLinkCouverture(file.absoluteFilePath());
        reloadGame();

        saveGame(game);
        loadgameFromFile();
        loadList();
    }
}
