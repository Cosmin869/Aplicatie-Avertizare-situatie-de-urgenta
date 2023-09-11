#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->socket = new QTcpSocket();
    socket->connectToHost("192.168.43.74", 54001);
    socket->open(QIODevice::ReadWrite);
    this->timer = new QTimer(this);
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(read_from_server()));
    timer->setInterval(1000);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_from_server()
{
    if(this->socket != nullptr && this->socket->isOpen())
    {
        QByteArray response = this->socket->readAll();
        qDebug() << response;
        QString response_str = QString::fromStdString(response.toStdString());
        if (response_str == "You're logged in!")
        {
            this->hide();
            secPage = new Second_Page(this, this->socket);
            secPage->setModal(true);
            secPage->exec();
        }
        else if(response_str == "You're Admin!")
        {
            this->hide();
            Administrator adm(this, this->socket);
            adm.setModal(true);
            adm.exec();
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    //inregistrare: protocol 1
    //autentificare: protocol 0
    QString username = ui -> userName ->text();
    QString password = ui -> userPass ->text();

    //QString verif = xorEncrypt(username,'x');

    QString to_send;
    to_send = "0";
    to_send += "*";
    to_send += username;
    to_send += "*";
    to_send += password;

    socket->write(to_send.toStdString().c_str());
    this->socket->flush();
   // QMessageBox::information(this, "login", this->read_from_server());

}
void MainWindow::on_pushButton_2_clicked()
{
    hide();
    //QString protocol = "1";
    //socket->write(protocol.toStdString().c_str());
    //this->socket->flush();
    accPage = new Create_Account(this);
    accPage -> show();
}


