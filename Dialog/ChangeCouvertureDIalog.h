#ifndef CHANGECOUVERTUREDIALOG_H
#define CHANGECOUVERTUREDIALOG_H

#include <QWidget>

namespace Ui {
class ChangeCouvertureDIalog;
}

class ChangeCouvertureDIalog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeCouvertureDIalog(QWidget *parent = nullptr);
    ~ChangeCouvertureDIalog();

    QString path;

private slots:
    void on_pbOpen_clicked();

    void on_pushButton_2_clicked();
public slots:
    QString on_pushButton_clicked();

private:
    Ui::ChangeCouvertureDIalog *ui;
};

#endif // CHANGECOUVERTUREDIALOG_H
