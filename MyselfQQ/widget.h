#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTextBrowser>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString name,QString usrname);
    ~Widget();

private slots:
    void on_fontCbx_currentFontChanged(const QFont &f);

    void on_sizeCbx_currentIndexChanged(const QString &arg1);

    void on_holdBtn_clicked(bool checked);

    void on_italicBtn_clicked(bool checked);

    void on_underlineBtn_clicked(bool checked);

    void on_colorBtn_clicked();
    void slotEnter();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();
    void slotSend();
    void slotOpen();
    void sendFile_start();

    void on_pushButton_clicked();

    void on_sendBtn_clicked();

private:
    QString userName;
    QString name_w;
    Ui::Widget *ui;
    QColor color;
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
    QTcpSocket *tcpSocket;
    QPushButton *oepnFileBtn;
    QString fileName;
    QLabel *fileName_Lab;
    QPushButton *sendFileBtn;
    QPushButton *pushbtntest;
};

#endif // WIDGET_H
