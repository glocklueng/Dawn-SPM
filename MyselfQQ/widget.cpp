#include "widget.h"
#include "ui_widget.h"
#include<QColorDialog>
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

Widget::Widget(QWidget *parent,QString name,QString usrname) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    userName = usrname;
    name_w = name;
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("chaoshi"));

     msgBrowser = new QTextBrowser;
     msgBrowser->setTextColor(Qt::blue);
     msgBrowser->setCurrentFont(QFont("Times New Roman",12));
     contentListWidget = new QListWidget;


     sendBtn = new QPushButton(QString::fromLocal8Bit("fasong"));

     userNameLabel = new QLabel(tr("usrname"));
     userNameLineEdit = new QLineEdit;






     connectBtn= new QPushButton(QString::fromLocal8Bit("连接"));
     oepnFileBtn= new QPushButton(QString::fromLocal8Bit("打开文件"));
     sendFileBtn = new QPushButton(QString::fromLocal8Bit("sendfile"));

     pushbtntest = new QPushButton(QString::fromLocal8Bit("测试"));

     status = false;

     port = 9999;
     userNameLineEdit->setText("usrID");

     serverIP =new QHostAddress();

     connect(connectBtn,SIGNAL(clicked()),this,SLOT(slotEnter()));
     connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
     connect(oepnFileBtn,SIGNAL(clicked()),this,SLOT(slotOpen()));
     connect(sendFileBtn,SIGNAL(clicked()),this,SLOT(sendFile_start()));

     sendBtn->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTxtEdit->setCurrentFont(f);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_sizeCbx_currentIndexChanged(const QString &arg1)
{
    ui->msgTxtEdit->setFontPointSize(arg1.toDouble());
    ui->msgTxtEdit->setFocus();
}

void Widget::on_holdBtn_clicked(bool checked)
{
    if(checked)
        ui->msgTxtEdit->setFontWeight(QFont::Bold);
    else
        ui->msgTxtEdit->setFontWeight(QFont::Normal);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_italicBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontItalic(checked);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_underlineBtn_clicked(bool checked)
{
    ui->msgTxtEdit->setFontUnderline(checked);
    ui->msgTxtEdit->setFocus();
}

void Widget::on_colorBtn_clicked()
{
    color=QColorDialog::getColor(color,this);
    if(color.isValid()){
        ui->msgTxtEdit->setTextColor(color);
        ui->msgTxtEdit->setFocus();
    }
}

void Widget::slotEnter()
{
}

void Widget::slotConnected()
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

void Widget::slotSend()
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

void Widget::slotDisconnected()
{
    sendBtn->setEnabled(false);
    connectBtn->setText(tr("connect"));
}

void Widget::dataReceived()
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
            datagram_reciv.resize(0);
        }else if(type_reciv=="msg"){
           QString  time = QDateTime::currentDateTimeUtc().toString();

            ui->textBrowser->append("[  partner] "+ time);
            ui->textBrowser->append(datagram_reciv);
        }



    }

}

void Widget::slotOpen(){
    fileName= QFileDialog::getOpenFileName(
                this,
                "open file dialog Choose a file",
                "/home",
                "Images (*.*)" );
    fileName_Lab->setText(fileName);
}

void Widget::sendFile_start(){
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

void Widget::on_pushButton_clicked()
{

    if(!status)
    {
        QString ip ="115.159.83.140 ";
        if(!serverIP->setAddress(ip))
        {
            QMessageBox::information(this,tr("error"),tr("server ip address error!"));
            return;
        }

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
        QJsonObject msg_json;
        msg_json.insert("usrname",userName);
        msg_json.insert("type","msg");
        msg_json.insert("content","Leave");
        QJsonDocument msg_doc(msg_json);
        QString msg(msg_doc.toJson(QJsonDocument::Compact));

        tcpSocket->write(msg.toLatin1(),msg.length());

        tcpSocket->disconnectFromHost();
        status=false;
    }
}

void Widget::on_sendBtn_clicked()
{
    QJsonObject msg_json;
    msg_json.insert("usrname",userName);
    msg_json.insert("type","msg");
    msg_json.insert("toname",name_w);
    msg_json.insert("content",ui->msgTxtEdit->toHtml());
    QJsonDocument msg_doc(msg_json);
    QString msg(msg_doc.toJson(QJsonDocument::Compact));
    ui->msgTxtEdit->clear();
    QString  time = QDateTime::currentDateTimeUtc().toString();
     ui->textBrowser->append("["+userName+"] "+ time);
     ui->textBrowser->append(msg_json.take("content").toString());

    tcpSocket->write(msg.toLatin1(),msg.length());
}
