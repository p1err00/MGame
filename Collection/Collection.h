#ifndef COLLECTION_H
#define COLLECTION_H

#include <QList>

class Collection
{
public:
    Collection(QString name);

    Collection addCollection();

    void deleteCollection();

    void seeCollections();

    void seeCollection();

    void addItem();

    void deleteItem();

public slots:

    Collection fromJson();

    QJsonObject toJson();

    void save(Collection *collection);

    void load();

private :
    QString _name;



};

#endif // COLLECTION_H
