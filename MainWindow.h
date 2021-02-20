#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Game.h"
#include "AddDialog.h"

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

    void loadGame();

    Game *selectGame;

    QDateTime oStartTime;

    QTime stop;

private slots:

    void on_pbAdd_clicked();

    void on_pbDel_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pbLaunch_clicked();

    quint64 calculateTime(QDateTime oStartTime, QDateTime oEndtTime);

private:

    QString f;
    QString path;
    QString dir;
    QPushButton *pbOpen;
    QPushButton *launch;
    QJsonObject m_currentJsonObject; 

    void displayGame(Game *game);

public slots:

    void saveTimePlayed(Game *game);

    void startProgram();

    void on_pushButton_clicked();

    void changeDateLastUse(Game *game);

    void loadList();

};
#endif // MAINWINDOW_H
