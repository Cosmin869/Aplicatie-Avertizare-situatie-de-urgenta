#ifndef SECOND_PAGE_H
#define SECOND_PAGE_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkReply>
#include <qtimer.h>

class MainWindow;

struct CountryInfo {
    int id;
    QString country_name;
};

struct DisasterInfo {
    int id;
    QString disaster_name;
};

namespace Ui {
class Second_Page;
}

class Second_Page : public QDialog
{
    Q_OBJECT

public:
    explicit Second_Page(QWidget *parent = nullptr, QTcpSocket* socket = nullptr);
    ~Second_Page();
    QVector<CountryInfo> countryVector;
    QVector<DisasterInfo> disasterVector;

private slots:
    void on_pushButton_clicked();

    void onNetworkReply(QNetworkReply *reply);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void listen_for_alerts();

    void on_pushButton_4_clicked();

private:
    Ui::Second_Page *ui;
    QTcpSocket *socket;
    MainWindow * mw;
    QTimer * timer;
    QString initialApiUrl = "https://api.reliefweb.int/v1/disasters?appname=rwint-user-0&profile=list&preset=latest&slim=1";
};

#endif // SECOND_PAGE_H
