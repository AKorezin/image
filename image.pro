#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T19:47:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = image
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    images.cpp \
    scene.cpp

HEADERS  += mainwindow.h \
    images.h \
    scene.h

FORMS    += mainwindow.ui
INCLUDEPATH += /usr/include/opencv
LIBS += `pkg-config opencv --cflags --libs`
