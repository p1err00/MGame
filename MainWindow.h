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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    Ui::MainWindow *ui;

    QList<Game> listGame;

    ~MainWindow();

    void saveGame(Game *game);

    void loadGame();

private slots:

    void on_pbAdd_clicked();

    void on_pbDel_clicked();

    void closeEvent(QCloseEvent *event);

    void on_pbLoad_clicked();

    void on_pbLaunch_clicked(Game game);

private:

    QString f;
    QString path;
    QString dir;
    QPushButton *pbOpen;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *launch;
    QJsonObject m_currentJsonObject;
    QString selectGame;

public slots:

};
#endif // MAINWINDOW_H
