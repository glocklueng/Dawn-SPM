#ifndef DRAWER_H
#define DRAWER_H

#include<QToolBox>
#include<QToolButton>
#include "widget.h"

class Drawer : public QToolBox
{
            Q_OBJECT
public:
    Drawer(QWidget* parent=0,Qt::WindowFlags f=0);
private:
     QToolButton *toolBtn1;
     QToolButton *toolBtn2;
     QToolButton *toolBtn3;
     Widget *chatWidget1;
     Widget *chatWidget2;
     Widget *chatWidget3;
private slots:
     void showChatWidget1();
     void showChatWidget2();
     void showChatWidget3();
};

#endif // DRAWER_H
