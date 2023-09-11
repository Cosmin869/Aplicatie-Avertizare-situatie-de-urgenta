#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <QDialog>
#include <QtNetwork//qtcpsocket.h>
#include <qtimer.h>

class MainWindow;

namespace Ui {
class Create_Account;
}

class Create_Account : public QDialog
{
    Q_OBJECT

public:
    explicit Create_Account(QWidget *parent = nullptr);
    ~Create_Account();

private slots:
    void on_pushButton_clicked();

    void on_back_button_clicked();

private:
    Ui::Create_Account *ui;
    QTcpSocket *socket;
    QTimer * timer;
    MainWindow * mw2;
};

#endif // CREATE_ACCOUNT_H
