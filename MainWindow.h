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

    void processFinished(int code , QProcess::ExitStatus status, QDateTime dateTimeStart);

    void closeEvent(QCloseEvent *event);

    void launchGame(Game *game);

    void gameStop();

    void on_pbSave_clicked();

    void on_pbLoad_clicked();


private:

    QString f;
    QString path;
    QString dir;
    QPushButton *pbOpen;
    QVBoxLayout *layout = new QVBoxLayout;
    QPushButton *launch;
    QJsonObject m_currentJsonObject;
    Game *selectGame;


public slots:

    void list(Game *game);

};
#endif // MAINWINDOW_H
