#ifndef COLLECTION_H
#define COLLECTION_H

#include <QList>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStyle>
#include <QDesktopWidget>
#include <QScreen>
#include <QWidget>
#include <QGuiApplication>
#include <QDebug>
#include <QMessageBox>

class Collection
{
public:
    Collection(QString name);

    QString getName();
    void setName(QString name);

    void createCollection();

    void addToCollection();

    void deleteCollection();

    void seeCollections();

    void seeCollection();

    void addItem();

    void deleteItem();

public slots:

    void fromJson();

    QJsonObject toJson();

    void save(Collection *collection);

    void load();

private :
    QString _name;
    Collection *col;


};

#endif // COLLECTION_H
