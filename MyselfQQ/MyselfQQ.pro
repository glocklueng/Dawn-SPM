#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T15:23:55
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyselfQQ
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    drawer.cpp \
    server.cpp

HEADERS  += widget.h \
    drawer.h \
    server.h

FORMS    += widget.ui \
    server.ui

RESOURCES += \
    myimage.qrc
