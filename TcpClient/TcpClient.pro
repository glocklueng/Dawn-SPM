#-------------------------------------------------
#
# Project created by QtCreator 2013-06-06T16:12:55
#
#-------------------------------------------------

QT       += multimedia
QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
TEMPLATE = app


SOURCES += main.cpp\
        tcpclient.cpp \
    login.cpp

HEADERS  += tcpclient.h \
    login.h
