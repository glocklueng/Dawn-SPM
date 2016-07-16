#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>

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
    fileName_Lab = new QLabel(tr("file name"));
    portLineEdit = new QLineEdit;

    enterBtn= new QPushButton(tr("connect"));
    testBtn= new QPushButton(tr("open"));
    sendFile = new QPushButton(tr("sendFile"));


    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget,0,0);
    mainLayout->addWidget(fileName_Lab,0,1);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendFile,1,1);
    mainLayout->addWidget(sendBtn,2,0);
    mainLayout->addWidget(userNameLabel,3,0);
    mainLayout->addWidget(userNameLineEdit,3,1);
    mainLayout->addWidget(serverIPLabel,4,0);
    mainLayout->addWidget(serverIPLineEdit,4,1);
    mainLayout->addWidget(portLabel,5,0);
    mainLayout->addWidget(portLineEdit,5,1);
    mainLayout->addWidget(enterBtn,6,0);
    mainLayout->addWidget(testBtn,6,1);

    status = false;

    port = 9999;
    userNameLineEdit->setText("123");
    portLineEdit->setText(QString::number(port));
   serverIPLineEdit->setText("115.159.83.140 ");

    serverIP =new QHostAddress();

    connect(enterBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
    connect(testBtn,SIGNAL(clicked()),this,SLOT(slotOpen()));
    connect(sendFile,SIGNAL(clicked()),this,SLOT(sendFile_start()));

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
    msg_json.insert("usrname",userName);
    msg_json.insert("type","msg");
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));
    contentListWidget->addItem("sended");

    tcpSocket->write(msg.toLatin1(),msg.length());
}

void TcpClient::slotSend()
{
    if(sendLineEdit->text()=="")
    {
        return ;
    }

    QJsonObject msg_json;
    msg_json.insert("usrname",userName);
    msg_json.insert("type","msg");
    msg_json.insert("content",sendLineEdit->text());
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));

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
        QString type_reciv;
        QString usrname_reciv;
        QByteArray datagram_reciv;
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(),datagram.size());

        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(datagram, &json_error);
        if(json_error.error == QJsonParseError::NoError)
        {
            if(parse_doucment.isObject())
            {
                QJsonObject obj = parse_doucment.object();
                if(obj.contains("usrname"))
                 {
                    QJsonValue name_value = obj.take("usrname");
                    usrname_reciv = name_value.toString();
                }
                if(obj.contains("type"))
                {
                    QJsonValue type_value = obj.take("usrname");
                    type_reciv=type_value.toString();
                }
                if(obj.contains("content"))
                {
                    QJsonValue content_value = obj.take("content");
                    datagram_reciv = content_value.toVariant().toByteArray();
                }
            }
        }
        if(type_reciv == "file" )
        {
            QFile *receivedFile = new QFile("D:\new.cpp");
            if (!receivedFile->open(QFile::WriteOnly ))  {    return;}
            receivedFile->write(datagram_reciv);
            datagram_reciv.resize(0);
        }


        QString msg=datagram.data();
        contentListWidget->addItem(msg.left(datagram.size()));
    }

}

void TcpClient::slotOpen(){
    fileName= QFileDialog::getOpenFileName(
                this,
                "open file dialog Choose a file",
                "/home",
                "Images (*.*)" );
    fileName_Lab->setText(fileName);
}

void TcpClient::sendFile_start(){
    QFile *sendFile = new QFile(fileName);
    if (!sendFile->open(QFile::ReadOnly ))  //读取发送文件
    {    return;}
    QByteArray outBlock;
    outBlock = sendFile->read(sendFile->size());


    QJsonObject msg_json;
    msg_json.insert("usrname",userName);
    msg_json.insert("type","file");
    msg_json.insert("content",outBlock.data());
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));

    tcpSocket->write(msg.toLatin1(),msg.size());
}
