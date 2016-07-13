#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QJsonObject>
#include<QJsonDocument>


TcpClient::TcpClient(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("TCP Client"));

    contentListWidget = new QListWidget;

    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(tr("send"));

    userNameLabel = new QLabel(tr("usrname"));
    userNameLineEdit = new QLineEdit;

    serverIPLabel = new QLabel(tr("address"));
    serverIPLineEdit = new QLineEdit;

    portLabel = new QLabel(tr("port"));
    portLineEdit = new QLineEdit;

    enterBtn= new QPushButton(tr("connect"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget,0,0);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendBtn,2,0);
    mainLayout->addWidget(userNameLabel,3,0);
    mainLayout->addWidget(userNameLineEdit,3,1);
    mainLayout->addWidget(serverIPLabel,4,0);
    mainLayout->addWidget(serverIPLineEdit,4,1);
    mainLayout->addWidget(portLabel,5,0);
    mainLayout->addWidget(portLineEdit,5,1);
    mainLayout->addWidget(enterBtn,6,0);

    status = false;

    port = 9999;
    userNameLineEdit->setText("123");
    portLineEdit->setText(QString::number(port));
   serverIPLineEdit->setText("115.159.83.140 ");

    serverIP =new QHostAddress();

    connect(enterBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));

    sendBtn->setEnabled(false);
}

TcpClient::~TcpClient()
{

}

void TcpClient::slotEnter()
{
    if(!status)
    {
        QString ip = serverIPLineEdit->text();
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }

        if(userNameLineEdit->text()=="")
        {
            QMessageBox::information(this,tr("error"),tr("User name error!"));
            return;
        }

        userName=userNameLineEdit->text();

        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        port = (portLineEdit->text()).toInt();
        tcpSocket->connectToHost(*serverIP,port);
        contentListWidget->addItem("Connecting to");
        contentListWidget->addItem(serverIP->toString()+":"+QString::number(port));
        status=true;
    }
    else
    {
        int length=0;
        QString msg=userName+tr(":Leave Room");

        if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg. length())
        {
            return;
        }

        tcpSocket->disconnectFromHost();
        status=false;
    }
}

void TcpClient::slotConnected()
{
    sendBtn->setEnabled(true);
    enterBtn->setText(tr("Leave"));

    int length=0;
    QJsonObject msg_json;
//    QString msg=userName+tr(":Enter Chat Room");
    msg_json.insert("usrname",userName);
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));

    if((length=tcpSocket->write(msg.toLatin1(),msg.length()))!=msg.length())
    {
        contentListWidget->addItem("sended");
        return;
    }
}

void TcpClient::slotSend()
{
    if(sendLineEdit->text()=="")
    {
        return ;
    }

    QJsonObject msg_json;
//    QString msg=userName+tr(":Enter Chat Room");
    msg_json.insert("usrname",userName);
    msg_json.insert("msg",sendLineEdit->text());
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));
//    QString msg=userName+":"+sendLineEdit->text();
//    QString msg =" {\"+aa\":[\"sdddssd\"],\"bb\":[\"892394829342394792399\",\"23894723984729374932874\"],\"cc\":[\"11111111111111\"]}";

    tcpSocket->write(msg.toLatin1(),msg.length());
    sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
    sendBtn->setEnabled(false);
    enterBtn->setText(tr("connect"));
}

void TcpClient::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(),datagram.size());

        QString msg=datagram.data();
        contentListWidget->addItem(msg.left(datagram.size()));
    }
}
