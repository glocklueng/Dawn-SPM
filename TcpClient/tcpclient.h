#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTextBrowser>

class TcpClient : public QDialog
{
    Q_OBJECT
    
public:
    TcpClient(QString usrID,QString passwd,QWidget *parent = 0,Qt::WindowFlags f=0);
    ~TcpClient();
private:
    QTextBrowser *msgBrowser;
    QListWidget *contentListWidget;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;
    QPushButton *connectBtn;
    QGridLayout *mainLayout;
    bool status;
    int port;
    QHostAddress *serverIP;
    QString userName;
    QTcpSocket *tcpSocket;
    QPushButton *oepnFileBtn;
    QString fileName;
    QLabel *fileName_Lab;
    QPushButton *sendFileBtn;
    QPushButton *pushbtntest;
public slots:
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();
    void slotOpen();
    void sendFile_start();
//    void slotplay();
};

#endif // TCPCLIENT_H
