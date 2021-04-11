#include "AddGameInstallDialog.h"
#include "ui_AddGameInstallDialog.h"
#include "GameInstall.h"
#include "MainWindow.h"
#include "GameInstallManager.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>

AddGameInstallDialog::AddGameInstallDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddGameInstallDialog)
{
    ui->setupUi(this);
}

AddGameInstallDialog::~AddGameInstallDialog()
{
    delete ui;
}

void AddGameInstallDialog::on_pbOpen_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Open","*.exe");

    folder = file.absoluteFilePath();
    QString size = formatSize(file.size());

    ui->leName->setText(name);
    ui->lFolder->setText(folder);
    ui->lSize->setText(size);
}
qint64 AddGameInstallDialog::dirSize(QString dirPath) {
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
QString AddGameInstallDialog::formatSize(qint64 size) {
    QStringList units = {"Bytes", "KB", "MB", "GB", "TB", "PB"};
    int i;
    double outputSize = size;
    for(i=0; i<units.size()-1; i++) {
        if(outputSize<1024) break;
        outputSize= outputSize/1024;
    }
    return QString("%0 %1").arg(outputSize, 0, 'f', 2).arg(units[i]);
}

void AddGameInstallDialog::on_pushButton_2_clicked()
{
    close();
}

void AddGameInstallDialog::on_pushButton_clicked()
{
    bool install = false;
    GameInstallManager m_gameInstall;

    if(ui->leName->text().isEmpty())
        QMessageBox::warning(this, "Warning", "Name can't be empty");
    name = ui->leName->text();

    GameInstall *gameInstall = new GameInstall(name, folder, size, install);

    MainWindow mw;
    mw.listGameInstall.append(gameInstall);
    qDebug() << "Game Install Save";
    for(auto item : mw.listGameInstall)
        qDebug() << item->name();

    m_gameInstall.save(gameInstall);
    close();
}
