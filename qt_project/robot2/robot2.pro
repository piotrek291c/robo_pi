#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T19:15:56
#
#-------------------------------------------------

#QT       += core gui
QT       += core gui network widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robot2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
#INCLUDEPATH    += /usr/local/include
LIBS += -L/usr/local/include -lwiringPi
