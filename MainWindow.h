#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game.h"
#include "AddDialog.h"
#include "Settings/Settings.h"
#include "Collection/Collection.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFile>
#include <QDialog>
#include <QProcess>
#include <QJsonObject>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QAction>
#include <QMenu>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    Ui::MainWindow *ui;

    QList<Game*> listGame;

    QList<Collection*> listCollection;

    ~MainWindow();

    void saveGame(Game *game);

    void loadgameFromFile();

    Game *selectGame;

    QDateTime oStartTime;

    QTime stop;
    QSettings settings;
    QFile file;
    QList<QString> favList;

    void loadTransmission();


private slots:

    void on_pbAdd_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pbLaunch_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_pbAddType_clicked();

    void on_comboBox_activated(const QString &arg1);

private:

    QString f;
    QString path;
    QString dir;
    QPushButton *pbOpen;
    QPushButton *launch;
    QJsonObject m_currentJsonObject; 
    QWidget *wid;
    QString descChange;

    QMenu *listMenu = new QMenu;
    QMenu *itemMenu = new QMenu;
    QMenu *appMenu = new QMenu;
    QMenu *collectionMenu = new QMenu;

    QAction *action_itemAdd_listMenu = new QAction("Add");
    QAction *action_itemReload_listMenu = new QAction("Reload");

    QAction *action_itemDel_itemMenu = new QAction("Delete");
    QAction *action_itemFavori_itemMenu = new QAction("Favori");
    QAction *action_itemRemoveFromCollection_itemMenu = new QAction("Supprimer de la collection");
    QAction *action_itemProperty_itemMenu = new QAction("Property");

    QAction *action_createCollection_collectionMenu = new QAction("New collection");
    QAction *action_addToCollection_collectionMenu = new QAction("Add to collection");
    QAction *action_displayCollection_collectionMenu = new QAction("Afficher cette collection");
    QAction *action_renameCollection_colectionMenu = new QAction("Renommer collection");
    QAction *action_deleteCollection_collectionMenu = new QAction("Supprimer collection");
    QAction *action_reduceCollection_collectionMenu = new QAction("Reduire");
    QAction *action_developAllCollections_collectionMenu = new QAction("Afficher toutes les collections");
    QAction *action_reduceAllCollections_collectionMenu = new QAction("Reduire toutes les collections");

    qint64 dirSize(QString dirPath);

    QString formatSize(qint64 size);

    void displayCollectionInLayout(Collection *collection);

    void reloadGame();

    quint64 calculateTime(QDateTime startProcess, QDateTime stopProcess);


public slots:

    void loadList();

    //void displayGame(Game *game);

    //Find idea to return that in private slot
    void on_pbDel_clicked();

    void saveDescGame();

    void loadFavList();
};
#endif // MAINWINDOW_H
