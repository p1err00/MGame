#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "Game.h"
#include "Dialog/SelectTypeDialog.h"
#include "Settings/SettingsDialog.h"
#include "Settings/FavoriDialog.h"
#include "TransmissionProcess.h"
#include "Settings/MainSettingsDialog.h"
#include "Dialog/ChangeCouvertureDIalog.h"
#include "Screenshot/Screenshot.h"

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
#include <QShortcut>
#include <QScreen>


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

    listMenu->setTitle("Tableau");
    listMenu->addAction(action_itemAdd_listMenu);
    listMenu->addAction(action_itemReload_listMenu);
    listMenu->addAction(action_itemDel_itemMenu);

    itemMenu->setTitle("Game");
    itemMenu->addAction(action_itemFavori_itemMenu);
    itemMenu->addAction(action_itemProperty_itemMenu);

    couvertureMenu->addAction(action_itemChange_couvertureMenu);

    typeMenu->addAction(action_openList_typeMenu);

    listMenu->addMenu(itemMenu);

    QWidget *wid3;
    wid3 = ui->lCouverturel;
    wid3->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid3, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_lCouverturel_customContextMenuRequested(const QPoint &pos)));

    QWidget *wid4;
    wid4 = ui->lwType;
    wid4->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(wid4, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_lwType_customContextMenuRequested(const QPoint &pos)));

    //Shortcut

    space = new QShortcut(this);
    space->setKey(Qt::Key_Space);

    QObject::connect(space, &QShortcut::activated, this,[=]{
        qDebug() << "Shoot screen";

        QScreen *screen = QGuiApplication::primaryScreen();

        QPixmap originalPixmap = screen->grabWindow(QApplication::desktop()->hasFocus());
        quint64 random = QRandomGenerator::global()->generate();
        originalPixmap.save(QDir::currentPath()+"\\Screenshot\\"+QString::number(random)+".png");
    });

    //Set example picture
    QString aze = "C:\\Users\\p1err0\\Documents\\build-Project-Desktop_Qt_5_12_6_MSVC2015_64bit-Debug\\debug\\img\\cyberpunk.jpg";
    for(int i = 0; i < 5; i++){
        QPixmap *pixi = new QPixmap(aze);
        QLabel *l = new QLabel;
        l->setPixmap(*pixi);
        l->setStyleSheet("QLabel {"
                         "border: 0;"
                         "}");
        ui->layouListScreen->addWidget(l);
    }

    //Set stylesheet menu
    listMenu->setStyleSheet("QMenu {"
                            "background-color: #777777;"
                            "min-width: 75px"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #ff4f4f;"
                            "border: 1px solid red;"
                            "}");
    itemMenu->setStyleSheet("QMenu {"
                            "background-color: #777777;"
                            "min-width: 75px"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #ff4f4f;"
                            "min-height: 25px;"
                            "}");
    couvertureMenu->setStyleSheet("QMenu {"
                            "background-color: #777777;"
                            "min-width: 75px"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #ff4f4f;"
                            "min-height: 25px;"
                            "}");
    typeMenu->setStyleSheet("QMenu {"
                            "background-color: #777777;"
                            "min-width: 75px"
                            "}"
                            "QMenu::item:selected {"
                            "background-color: #ff4f4f;"
                            "min-height: 25px;"
                            "}");

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

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    QFile file("save.json");
    QPixmap *pixPicture;
    QPixmap *pixCouverture;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    QJsonArray jsonArray = QJsonDocument::fromJson(stream.readAll().toUtf8()).array();
    file.close();

    ui->lImage->clear();
    ui->lCouverturel->clear();

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
            ui->lLastUse->setText(object.toObject().value("dateLastUse").toString().toUtf8());
            ui->lName->setText(object.toObject().value("name").toString().toUtf8());
            ui->leDesc->setWordWrap(1);
            ui->leDesc->setText(object.toObject().value("desc").toString().toUtf8());

            ui->lTimePlayed->setText(QString::number(hour) + ":" + QString::number(min) + ":" + QString::number(sec));
            if(object.toObject().value("dateLastUse").toString() == "")
                ui->lLastUse->setText("Never used");
            else
                ui->lLastUse->setText(object.toObject().value("dateLastUse").toString());

            ui->lDate->setText(object.toObject().value("date").toString());
            ui->lDir->setText(object.toObject().value("directory").toString().toUtf8());
            ui->lPath->setText(object.toObject().value("path").toString().toUtf8());
            ui->lPath->setWordWrap(1);
            ui->lwType->clear();
            QListWidgetItem *it;
            for(int i = 0; i < object.toObject().value("type").toArray().count(); i++){
                it = new QListWidgetItem(object.toObject().value("type").toArray().at(i).toString());
                ui->lwType->addItem(it);
            }
            ui->lSizeFolder->setText(formatSize(size));

            pixPicture = new QPixmap(object.toObject().value("linkPicture").toString());
            *pixPicture = pixPicture->scaled(ui->lImage->size(), Qt::KeepAspectRatio);
            ui->lImage->setPixmap(*pixPicture);

            pixCouverture = new QPixmap(object.toObject().value("linkCouverture").toString());
            *pixCouverture = pixCouverture->scaled(ui->lCouverturel->size(), Qt::KeepAspectRatio);
            ui->lCouverturel->setPixmap(*pixCouverture);

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

    if(!selectGame)
        return;
    QFutureWatcher<void> *watcher = new QFutureWatcher<void>();
    QFuture<void> future = QtConcurrent::run(&MainWindow::launchGame, selectGame);
    QDateTime startProcess = QDateTime::currentDateTime();
    qDebug() << "Launch";



    QObject::connect(watcher, &QFutureWatcher<void>::finished, [=](){

        qDebug() << "Go to save time";
        qDebug() << startProcess;

        QDateTime stopProcess = QDateTime::currentDateTime();

        quint64 timeDiff = calculateTime(startProcess, stopProcess);

        qDebug() << timeDiff;

        QDate value = QDate::currentDate();

        Game *n_game = selectGame;
        n_game->setTimePlayed(selectGame->timePlayed() + timeDiff);
        n_game->setDateLastUse(value.toString());
        QList<QString> list;
        for(auto i :  selectGame->types()){
            list.append(i);
        }
        n_game->setTypes(list);

        reloadGame();
        saveGame(n_game);
        loadgameFromFile();
        loadList();
    });

    watcher->setFuture(future);
}

void MainWindow::takeScreen(){
    qDebug() << "Screen Mes couilles";
    Screenshot *sc = new Screenshot("C:\\Users\p1err0\\Documents\\build-Project-Desktop_Qt_5_12_6_MSVC2015_64bit-Debug\\Screenshot", "screen");
    sc->shootScreen();
    //screenShotCounter++;
}

void MainWindow::launchGame(Game *game){

    QString program =  game->path();
    QStringList arguments;
    QString workingDirectory = game->directory();

    QProcess *process = new QProcess();

    arguments << "start /c " << game->path().section("/", -1);

    QDateTime startProcess =QDateTime::currentDateTime();

    process->execute(program, arguments);
}

//Calculate difference between start and stop time program
quint64 MainWindow::calculateTime(QDateTime startProcess, QDateTime stopProcess){

    quint64 difference = qAbs(startProcess.date().daysTo(stopProcess .date()));
    difference  *= static_cast<quint64>(24); // days to hours
    difference  *= static_cast<quint64>(60); // hours to minutes
    difference  *= static_cast<quint64>(60); // minutes to seconds
    difference  *= static_cast<quint64>(1000); // seconds to milliseconds
    difference += qAbs(startProcess.time().msecsTo(stopProcess .time()));
    return difference;
}

void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{

    QPoint globalpos = ui->listWidget->mapToGlobal(pos);

    QListWidgetItem* pointedItem = ui->listWidget->itemAt(pos);

    QAction* selectedAction;

    selectedAction = listMenu->exec(globalpos);


    if(selectedAction == action_itemAdd_listMenu) {
        AddDialog *addDialog = new AddDialog;
        addDialog->exec();
    }

    if(selectedAction == action_itemReload_listMenu){
        loadList();
    }

    if(selectedAction == action_itemProperty_itemMenu) {

        if(!pointedItem)
            return;

        for(auto i : listGame){
            if(i->name() == pointedItem->text()){
                selectGame = i;
            }
        }
        QList<QString> list = selectGame->types();
        SettingsDialog *st = new SettingsDialog();
        st->displayGame(selectGame);
        st->exec();
        selectGame = st->n_game;
        selectGame->setTypes(list);
        reloadGame();
        saveGame(selectGame);
        on_pbDel_clicked();
        loadgameFromFile();
        loadList();
    }

    if(selectedAction == action_itemFavori_itemMenu){

        if(!pointedItem)
            return;

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

    if(selectedAction == action_itemDel_itemMenu) {

        if(!pointedItem)
            return;

        auto item = ui->listWidget->itemAt(pos);
        for(auto itemG : listGame){
            if(item->text() == itemG->name()){
                selectGame = itemG;
                on_pbDel_clicked();

            }
        }
        reloadGame();
        loadList();
    }

    action_itemDel_itemMenu->setEnabled(true);
    action_itemFavori_itemMenu->setEnabled(true);
    action_itemProperty_itemMenu->setEnabled(true);
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

void MainWindow::on_lwType_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalpos = ui->lwType->mapToGlobal(pos);

    QAction* selectedAction;
    selectedAction = typeMenu->exec(globalpos);

    if(selectedAction == action_openList_typeMenu){
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
}
