#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <QDebug>
#include <QSqlError>
#include <QtSql/QSqlDatabase>
#include <QSqlDriverCreator>
#include <QSqlQuery>
#include <QPixmap>
#include <QPainter>
#include <QString>
#include <QTimer>
#include "multithread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void scanScard();
    QSqlQuery *qry;
    QTimer * time;
    QSqlDatabase db;
    QString cardUID, UID;
    QString image;
    QPixmap maskImage;



    void connClose()
    {
        db.close();
        db.removeDatabase(QSqlDatabase::defaultConnection);
    }

    bool connOpen()
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setDatabaseName("priceverifier");
        db.setHostName("localhost");
        db.setUserName("root");
        db.setPassword("z1x2c3v4a1s2");
        db.setPort(3306);

        if(!db.open())
        {
            qDebug()<< "Failed to Open" << db.lastError();
            return false;
        }else{
            qDebug()<<("Connected");
            return true;
        }
    }
public slots:
    void newNumber(QString cardUid);
    void timeout2();
private:
    Ui::Widget *ui;
    multithread *mJob;
    QFuture<void> test;
};

#endif // WIDGET_H
