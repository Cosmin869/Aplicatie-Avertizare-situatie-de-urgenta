
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <second_page.h>
#include <create_account.h>
#include <QtNetwork//qtcpsocket.h>
#include <qtimer.h>

#include <administrator.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void read_from_server();

private:
    Ui::MainWindow *ui;
    Second_Page * secPage;
    Create_Account * accPage;
    Administrator * adm;
    QTcpSocket *socket;
    QTimer * timer;
};

#endif // MAINWINDOW_H
