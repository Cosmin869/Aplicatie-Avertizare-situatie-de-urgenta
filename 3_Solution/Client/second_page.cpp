#include "second_page.h"
#include "ui_second_page.h"

#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/qnetworkreply.h>

Second_Page::Second_Page(QWidget *parent, QTcpSocket* socket) :
    QDialog(parent),
    ui(new Ui::Second_Page)
{
    this->socket = socket;
    ui->setupUi(this);
}

Second_Page::~Second_Page()
{
    delete ui;
}

void Second_Page::on_pushButton_clicked()
{
    //afisare nume User
}


/*void Second_Page::on_textBrowser_historyChanged()
{
    QEventLoop loop;
    QNetworkAccessManager nam;
    QNetworkRequest req(QUrl("https://vremea.stirileprotv.ro/"));
    QNetworkReply *reply = nam.get(req);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray buffer = reply->readAll();

    qDebug() << "what did i get" << buffer;
}
*/
