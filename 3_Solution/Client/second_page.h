#ifndef SECOND_PAGE_H
#define SECOND_PAGE_H

#include <QDialog>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class Second_Page;
}

class Second_Page : public QDialog
{
    Q_OBJECT

public:
    explicit Second_Page(QWidget *parent = nullptr, QTcpSocket* socket = nullptr);
    ~Second_Page();

private slots:
    void on_pushButton_clicked();

    //void on_textBrowser_historyChanged();

private:
    Ui::Second_Page *ui;
    QTcpSocket *socket;
};

#endif // SECOND_PAGE_H
