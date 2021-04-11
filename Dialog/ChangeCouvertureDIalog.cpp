#include "ChangeCouvertureDIalog.h"
#include "ui_ChangeCouvertureDIalog.h"
#include "MainWindow.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

ChangeCouvertureDIalog::ChangeCouvertureDIalog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeCouvertureDIalog)
{
    ui->setupUi(this);
}

ChangeCouvertureDIalog::~ChangeCouvertureDIalog()
{
    delete ui;
}

void ChangeCouvertureDIalog::on_pbOpen_clicked()
{
    QFileInfo file = QFileDialog::getOpenFileName(this, "Open", QApplication::applicationDirPath()+"\\img","*");
    QPixmap pix;
    pix.load(file.absoluteFilePath());
    path = file.absoluteFilePath();
    if(pix.isNull())
        QMessageBox::warning(this, "Warning", "Picture is null");
}

void ChangeCouvertureDIalog::on_pushButton_2_clicked()
{
    close();
}

QString ChangeCouvertureDIalog::on_pushButton_clicked()
{
    if(path.isEmpty()){
        QMessageBox::warning(this, "Warning", "Path file is empty");
    } else {
        return path;
    }
    return path;
}
