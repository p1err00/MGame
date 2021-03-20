#include "Collection.h"

Collection::Collection(QString name)
{
    _name = name;
}

QString Collection::getName(){
    return _name;
}
void Collection::setName(QString name){
    _name = name;
}

void Collection::createCollection(){

    QDialog *createDialog = new QDialog;
    QVBoxLayout *layoutMain = new QVBoxLayout;
    QHBoxLayout *layoutButton = new QHBoxLayout;
    QLineEdit *leName = new QLineEdit;
    QPushButton *pbAdd = new QPushButton;
    QPushButton *pbCancel = new QPushButton;


    leName->setPlaceholderText("Name of Collection :");
    pbAdd->setText("Add");
    pbCancel->setText("Cancel");

    layoutButton->addWidget(pbAdd);
    layoutButton->addWidget(pbCancel);

    layoutMain->addWidget(leName);
    layoutMain->addLayout(layoutButton);

    createDialog->setLayout(layoutMain);
    createDialog->setGeometry(750,350,250,150);
    createDialog->show();

    QObject::connect(pbAdd, &QPushButton::clicked, [=](){

        if(leName->text().isEmpty())
            QMessageBox::warning(createDialog, "Warning", "Le champ \"Nom\" doit etre remplis");

        Collection *col = new Collection(leName->text());
        createDialog->close();

        qDebug() << col->getName();
        return col;

    });

    QObject::connect(pbCancel, &QPushButton::clicked, [=](){
        createDialog->close();
    });

}

void Collection::addToCollection(){

}

void Collection::deleteCollection(){

}

void Collection::seeCollections(){

}

void Collection::seeCollection(){

}
void Collection::addItem(){

}

void Collection::deleteItem(){

}
