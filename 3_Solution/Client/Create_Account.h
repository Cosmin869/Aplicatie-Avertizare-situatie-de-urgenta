
#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <QDialog>
#include <QtNetwork//qtcpsocket.h>
#include <qtimer.h>

namespace Ui {
class Create_Account;
}

class Create_Account : public QDialog
{
    Q_OBJECT

public:
    explicit Create_Account(QWidget *parent = nullptr);
    ~Create_Account();

    //metoda pentru creare de cont

private slots:
    void on_pushButton_clicked();

private:
    Ui::Create_Account *ui;
    QTcpSocket *socket;
    QTimer * timer;
};

#endif // CREATE_ACCOUNT_H
