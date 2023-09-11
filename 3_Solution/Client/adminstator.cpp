#include "administrator.h"
#include "mainwindow.h"
#include "ui_administrator.h"

Administrator::Administrator(QWidget *parent, QTcpSocket* socket) :
    QDialog(parent),
    ui(new Ui::Administrator)
{
    ui->setupUi(this);
    this->socket = socket;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(listen_for_sos()));
    timer->start(1000);
}

Administrator::~Administrator()
{
    delete ui;
}

void Administrator::on_sendButton_clicked()
{
    QString regiune = ui -> textRegiune ->text();
    QString dezastru = ui -> textDezastru ->text();
    QString status = ui -> textStatus ->text();

    QString to_send = "2";
    to_send += "*";
    to_send += regiune;
    to_send += "*";
    to_send += dezastru;
    to_send += "*";
    to_send += status;

    socket->write(to_send.toStdString().c_str());
    this->socket->flush();
}


void Administrator::on_pushButton_clicked()
{
    this->close();
    mw4=new MainWindow;
    mw4->show();
}

void Administrator::listen_for_sos()
{
    if (this->socket != nullptr && this->socket->isOpen())
    {
        QByteArray notification = this->socket->readAll();
        QString response_str = QString::fromStdString(notification.toStdString());
        if(response_str.length() > 0 && response_str == "S.O.S")
        {
            ui->sosBox->append(response_str);
            this->socket->readAll();
        }
    }
}
