#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T19:30:10
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robo3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
LIBS += -lwiringPi
INCLUDEPATH    += /usr/local/include
LIBS += -L/usr/local/include -lwiringPi
