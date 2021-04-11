#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game.h"
#include "AddDialog.h"
#include "Settings/Settings.h"
#include "GameInstall/GameInstall.h"


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

    ~MainWindow();

    void saveGame(Game *game);

    void loadgameFromFile();

    void loadGameInstallFromFile();

    Game *selectGame;

    QDateTime oStartTime;

    QTime stop;
    QSettings settings;
    QFile file;
    QList<QString> favList;

    QList<GameInstall*> listGameInstall;

    void loadTransmission();


private slots:

    void on_pbAdd_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pbLaunch_clicked();

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void on_pbAddType_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_actionParametre_triggered();

    void on_lwGameInstall_itemClicked(QListWidgetItem *item);

    void on_lwGameInstall_customContextMenuRequested(const QPoint &pos);

    void on_pbChangeImage_clicked();

    void on_lCouverturel_customContextMenuRequested(const QPoint &pos);

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
    QMenu *itemInstallMenu = new QMenu;
    QMenu *couvertureMenu = new QMenu;

    QAction *action_itemAdd_listMenu = new QAction("Add");
    QAction *action_itemReload_listMenu = new QAction("Reload");

    QAction *action_itemDel_itemMenu = new QAction("Delete");
    QAction *action_itemFavori_itemMenu = new QAction("Favori");
    QAction *action_itemProperty_itemMenu = new QAction("Property");

    QAction *action_itemAdd_itemInstallMenu = new QAction("Add");
    QAction *action_itemDel_itemInstallMenu = new QAction("Delete");
    QAction *action_itemReload_itemInstallMenu = new QAction("Reload");
    QAction *action_itemAddList_itemInstallMenu = new QAction("Ajouté à \"Installer rapidement\"");

    QAction *action_itemChange_couvertureMenu = new QAction("Changer");

    qint64 dirSize(QString dirPath);

    QString formatSize(qint64 size);

    void reloadGame();

    quint64 calculateTime(QDateTime startProcess, QDateTime stopProcess);

    void closeEvent (QCloseEvent *event);

    void saveFavori();

public slots:

    void loadList();

    void loadListGameInstall();

    //void displayGame(Game *game);

    //Find idea to return that in private slot
    void on_pbDel_clicked();

    void saveDescGame();

    void loadFavList();
};
#endif // MAINWINDOW_H
