#-------------------------------------------------
#
# Project created by QtCreator 2016-04-02T16:28:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport designer

TARGET = Servidor
TEMPLATE = app

CONFIG += plugin release
CONFIG += c++11


LIBS += -pthread

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    LEDPlugin.cpp \
    LED.cpp \
    updatevalores.cpp \
    portablesleep.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    LEDPlugin.h \
    LED.h \
    updatevalores.h \
    portablesleep.h

FORMS    += mainwindow.ui
