#include "drawer.h"
#include<QGroupBox>
#include<QVBoxLayout>
#include <QDebug>

Drawer::Drawer(QString usrname,QString pswd,QWidget *parent,Qt::WindowFlags f)
    :QToolBox(parent,f)
{
    usrname_d = usrname;
    qDebug()<<usrname_d;
     setWindowTitle(tr("MyselfQQ 2016"));
     setWindowIcon(QPixmap(":/images/qq.png"));

     toolBtn1=new QToolButton;
     toolBtn1->setText(tr("asdas"));
     toolBtn1->setIcon(QPixmap(":/images/qmnn.png"));
     toolBtn1->setIconSize(QPixmap(":/images/qmnn.png").size());
     toolBtn1->setAutoRaise(true);
     toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
     connect(toolBtn1,SIGNAL(clicked()),this,SLOT(showChatWidget1()));

     toolBtn2=new QToolButton;
     toolBtn2->setText(tr("dsa"));
     toolBtn2->setIcon(QPixmap(":/images/cg.png"));
     toolBtn2->setIconSize(QPixmap(":/images/mimi.png").size());
     toolBtn2->setAutoRaise(true);
     toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
     connect(toolBtn2,SIGNAL(clicked()),this,SLOT(showChatWidget2()));

     toolBtn3=new QToolButton;
     toolBtn3->setText(tr("海棠"));
     toolBtn3->setIcon(QPixmap(":/images/ht.png"));
     toolBtn3->setIconSize(QPixmap(":/images/mimi.png").size());
     toolBtn3->setAutoRaise(true);
     toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
     connect(toolBtn3,SIGNAL(clicked()),this,SLOT(showChatWidget3()));

     toolBtn4=new QToolButton;
     toolBtn4->setText(tr("群聊"));
     toolBtn4->setIcon(QPixmap(":/images/ht.png"));
     toolBtn4->setIconSize(QPixmap(":/images/mimi.png").size());
     toolBtn4->setAutoRaise(true);
     toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
     connect(toolBtn4,SIGNAL(clicked()),this,SLOT(showChatWidget4()));

     QGroupBox*groupBox=new QGroupBox;
     QVBoxLayout*layout=new QVBoxLayout(groupBox);

     layout->setMargin(20);
     layout->setAlignment(Qt::AlignLeft);
     layout->addWidget(toolBtn1);
     layout->addWidget(toolBtn2);
     layout->addWidget(toolBtn3);
     layout->addWidget(toolBtn4);
     layout->addStretch();
     this->addItem((QWidget*)groupBox,tr(""));


}
void Drawer::showChatWidget1()
{
    chatWidget1 = new Widget(0,toolBtn1->text(),usrname_d);
    chatWidget1->setWindowTitle(toolBtn1->text());
    chatWidget1->setWindowIcon(toolBtn1->icon());
    chatWidget1->show();
}
void Drawer::showChatWidget2()
{
    chatWidget2=new Widget(0,toolBtn2->text(),usrname_d);
    chatWidget2->setWindowTitle(toolBtn2->text());
    chatWidget2->setWindowIcon(toolBtn2->icon());
    chatWidget2->show();
}
void Drawer::showChatWidget3()
{
    chatWidget3=new Widget(0,toolBtn3->text(),usrname_d);
    chatWidget3->setWindowTitle(toolBtn3->text());
    chatWidget3->setWindowIcon(toolBtn3->icon());
    chatWidget3->show();
}
void Drawer::showChatWidget4()
{
    chatWidget4=new Widget(0,toolBtn4->text(),usrname_d);
    chatWidget4->setWindowTitle(toolBtn4->text());
    chatWidget4->setWindowIcon(toolBtn4->icon());
    chatWidget4->show();
}
