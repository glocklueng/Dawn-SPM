﻿#include "tcpclient.h"
#include <QMessageBox>
#include <QHostInfo>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtMultimedia/QMediaPlayer>
#include <QUrl>
#include <QDateTime>

TcpClient::TcpClient(QString usrID,QString passwd,QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(QString::fromLocal8Bit("黎明苑超市"));

    msgBrowser = new QTextBrowser;
    msgBrowser->setTextColor(Qt::blue);
    msgBrowser->setCurrentFont(QFont("Times New Roman",12));
    contentListWidget = new QListWidget;

    sendLineEdit = new QLineEdit;
    sendBtn = new QPushButton(QString::fromLocal8Bit("发送"));

    userNameLabel = new QLabel(tr("usrname"));
    userNameLineEdit = new QLineEdit;




    fileName_Lab = new QLabel(tr("file name"));

    connectBtn= new QPushButton(QString::fromLocal8Bit("连接"));
    oepnFileBtn= new QPushButton(QString::fromLocal8Bit("打开文件"));
    sendFileBtn = new QPushButton(QString::fromLocal8Bit("发送文件"));

    pushbtntest = new QPushButton(QString::fromLocal8Bit("测试"));


    mainLayout = new QGridLayout(this);
//    mainLayout->addWidget(contentListWidget,0,0);
    mainLayout->addWidget(msgBrowser,0,0);
    mainLayout->addWidget(fileName_Lab,0,1);
    mainLayout->addWidget(sendLineEdit,1,0);
    mainLayout->addWidget(sendFileBtn,1,1);
    mainLayout->addWidget(sendBtn,2,0);
    mainLayout->addWidget(pushbtntest,2,1);
    mainLayout->addWidget(userNameLabel,3,0);
    mainLayout->addWidget(userNameLineEdit,3,1);
    mainLayout->addWidget(connectBtn,6,0);
    mainLayout->addWidget(oepnFileBtn,6,1);

    status = false;

    port = 9999;
    userNameLineEdit->setText(usrID);

    serverIP =new QHostAddress();

    connect(connectBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
    connect(oepnFileBtn,SIGNAL(clicked()),this,SLOT(slotOpen()));
    connect(sendFileBtn,SIGNAL(clicked()),this,SLOT(sendFile_start()));
    connect(pushbtntest,SIGNAL(clicked()),this,SLOT(slotplay()));

    sendBtn->setEnabled(false);
}

TcpClient::~TcpClient()
{

}

void TcpClient::slotEnter()
{
    if(!status)
    {
        QString ip ="115.159.83.140 ";
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
        port = 9999;
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
    connectBtn->setText(QString::fromLocal8Bit("断开"));


    QJsonObject msg_json;
    msg_json.insert("usrname",userName);
    msg_json.insert("type","login");
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));

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

    fileName_Lab->setText(msg_json.take("content").toString());
    tcpSocket->write(msg.toLatin1(),msg.length());
    sendLineEdit->clear();
}

void TcpClient::slotDisconnected()
{
    sendBtn->setEnabled(false);
    connectBtn->setText(tr("connect"));
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
                    QJsonValue type_value = obj.take("type");
                    type_reciv=type_value.toString();
                    fileName_Lab->setText(type_reciv);
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
            fileName_Lab->setText("recive");
//            QString fileName_reciv = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),tr("Files (*.txt);;C++ (*.cpp *.h)"));
            QString fileName_reciv = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),tr("Files *.*"));
            QFile receivedFile(fileName_reciv);
            if (!receivedFile.open(QFile::WriteOnly ))  {    return;}
            QTextStream stream(&receivedFile);
            stream<<datagram_reciv;
            stream.flush();
            receivedFile.close();
        }else if(type_reciv=="msg"){
           QString  time = QDateTime::currentDateTimeUtc().toString();
            msgBrowser->append("[  partner] "+ time);
            msgBrowser->append(datagram_reciv);
        }


        contentListWidget->addItem(datagram_reciv);
        datagram_reciv.resize(0);
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

//void TcpClient::slotplay()
//{

//    QString fileName_reciv = QFileDialog::getOpenFileName(
//                this,
//                "open file dialog Choose a file",
//                "/home",
//                "Images (*.*)" );

//    QMediaPlayer *play = new QMediaPlayer;
//    fileName_Lab->setText(sendLineEdit->text());
//    play->setMedia(QUrl("http://sc.111ttt.com/up/mp3/217385/740F74F616B5F6975F3073A33E6A99CD.mp3"));
//    play->setVolume(50);
//    play->play();


//}
