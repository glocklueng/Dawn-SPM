#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include<QTime>
class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void on_sOpenBtn_clicked();

private:
    Ui::Server *ui;
};

#endif // SERVER_H
