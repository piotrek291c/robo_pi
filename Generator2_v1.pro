#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T10:07:00
#
#-------------------------------------------------

QT       += core gui network  widgets axcontainer webkitwidgets webkit


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Generator2_v1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    zasoby.qrc \
    zasoby.qrc

DISTFILES += \
    przeszkody1.txt
# install
target.path = $$[QT_INSTALL_EXAMPLES]/activeqt/webbrowser
INSTALLS += target
