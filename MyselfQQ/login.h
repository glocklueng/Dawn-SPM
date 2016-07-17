#ifndef LOGIN_H
#define LOGIN_H

#include "drawer.h"
#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QString>

class Login : public QWidget
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);

private:
    QPushButton *loginBtn;
    QLineEdit *usrID;
    QLineEdit *passwd;
    QLabel *usrID_Lab;
    QLabel *passwd_Lab;
    QGridLayout *mainLayout;
    Drawer *client;

public slots:
    void loginAttempt();

};

#endif // LOGIN_H
