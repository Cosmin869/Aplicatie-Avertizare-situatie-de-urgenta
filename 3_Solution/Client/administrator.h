#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QTimer>

class MainWindow;

namespace Ui {
class Administrator;
}

class Administrator : public QDialog
{
    Q_OBJECT

public:
    explicit Administrator(QWidget *parent = nullptr, QTcpSocket* socket = nullptr);
    ~Administrator();

private slots:

    void on_sendButton_clicked();

    void on_pushButton_clicked();

    void listen_for_sos();

private:
    Ui::Administrator *ui;
    QTcpSocket *socket;
    QTimer * timer;
    MainWindow * mw4;
};

#endif // ADMINISTRATOR_H
