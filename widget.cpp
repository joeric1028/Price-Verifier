#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mJob = new multithread(this);
    connOpen();
    qry = new QSqlQuery(db);
    time = new QTimer(this);
    ui->stackedWidget->setCurrentWidget(ui->page_verify);
    scanScard();
}

Widget::~Widget()
{
    mJob->stop();
    mJob->CardReleaseContext();
    mJob->CardFreeMemory();
    delete ui;
}

void Widget::scanScard()
{
    connect(mJob,SIGNAL(onNumber(QString)),this,SLOT(newNumber(QString)));
    test = QtConcurrent::run(mJob,multithread::start);
    cardUID.clear();
}

void Widget::timeout2()
{
    ui->stackedWidget->setCurrentWidget(ui->page_verify);

    if(!test.isRunning()){
        scanScard();
    }
}

void Widget::newNumber(QString cardUid)
{
    mJob->stop();
    cardUID = cardUid;
    qDebug() << "Received UID: " << cardUID;
    ui->productData->setText("No Data");
    ui->priceData->setText("No Data");

    if(!cardUID.isEmpty())
    {
        ui->stackedWidget->setCurrentWidget(ui->page_product_details);
        qry->prepare("Select * FROM products WHERE uid = '" + cardUID + "';");
        if(qry->exec())
        {
            qDebug() << "Execute:" << qry->lastQuery() << qry->lastError();
            qry->next();
                ui->productData->setText(qry->value("productName").toString());
                ui->priceData->setText("PHP " + qry->value("price").toString());
                qDebug() << "Result: " << qry->value("productName").toString() << qry->value("price").toString();

            time->start(5000);
            if(test.isRunning()){
                mJob->pause();
            }

            connect(time, SIGNAL(timeout()), this, SLOT(timeout2()));
        }
    }
}
