#include "login.h"

Login::Login(QWidget *parent) : QWidget(parent)
{
    loginBtn = new QPushButton(QString::fromLocal8Bit("connect"));
    usrID = new QLineEdit();
    passwd = new QLineEdit();
    usrID_Lab = new QLabel(QString::fromLocal8Bit("usr"));
    passwd_Lab = new QLabel(QString::fromLocal8Bit("passwd"));

    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(usrID_Lab,0,0);
    mainLayout->addWidget(usrID,0,1);
    mainLayout->addWidget(passwd_Lab,1,0);
    mainLayout->addWidget(passwd,1,1);
    mainLayout->addWidget(loginBtn,2,0);


    connect(loginBtn,SIGNAL(clicked()),this,SLOT(loginAttempt()));




}

void Login::loginAttempt()
{
    client = new Drawer(usrID->text(),passwd->text(),0,0);
    client->show();
    this->hide();
}
