#include "create_account.h"
#include "ui_create_account.h"
#include "QMessageBox"


Create_Account::Create_Account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Create_Account)
{

    ui->setupUi(this);
    this->socket = new QTcpSocket();
    socket->connectToHost("192.168.43.7", 54001);
    socket->open(QIODevice::ReadWrite);
    this->timer = new QTimer(this);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(read_from_server()));
    timer->setInterval(1000);
    timer->start();
}

Create_Account::~Create_Account()
{
    delete ui;
}

void Create_Account::on_pushButton_clicked()
{

    QString username = ui -> userName_2 ->text();
    QString password = ui -> pass_1 ->text();
    QString password_c = ui -> pass_2 ->text();

    if(password == password_c /* and Username not taken */ )
    {
        QMessageBox::information(this,"Login","Account succesfully created");
        hide();

        QString to_send = "1";
        to_send += "*";
        to_send += username;
        to_send += "*";
        to_send += password;

        socket->write(to_send.toStdString().c_str());
        this->socket->flush();
    }
    else
    {
        QMessageBox::warning(this,"Login","Passwords dont match ");
    }
}

