#-------------------------------------------------
#
# Project created by QtCreator 2016-12-13T18:02:12
#
#-------------------------------------------------

#QT       += core gui
QT       += core gui network widgets


TARGET = robot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
INCLUDEPATH    += /usr/local/include
LIBS += -L/usr/local/include -lwiringPi
