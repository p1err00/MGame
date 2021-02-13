#include "MyThread.h"
#include "MainWindow.h"

#include <QDebug>
#include <QThread>

MyThread::MyThread()
{

}
void MyThread::run(){
    qDebug() << "Enter the thread";
}
