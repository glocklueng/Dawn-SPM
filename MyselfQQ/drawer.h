#ifndef DRAWER_H
#define DRAWER_H

#include<QToolBox>
#include<QToolButton>
#include "widget.h"

class Drawer : public QToolBox
{
            Q_OBJECT
public:
    Drawer(QString usrname,QString pswd,QWidget* parent=0,Qt::WindowFlags f=0);
private:
    QString usrname_d;
     QToolButton *toolBtn1;
     QToolButton *toolBtn2;
     QToolButton *toolBtn3;
     QToolButton *toolBtn4;
     Widget *chatWidget1;
     Widget *chatWidget2;
     Widget *chatWidget3;
     Widget *chatWidget4;
private slots:
     void showChatWidget1();
     void showChatWidget2();
     void showChatWidget3();
     void showChatWidget4();
};

#endif // DRAWER_H
