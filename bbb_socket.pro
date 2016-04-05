QT += core
QT -= gui

CONFIG += c++11

TARGET = bbb_socket
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11
LIBS += -pthread


TEMPLATE = app

SOURCES += main.cpp \
    Adafruit_Python_DHT/source/Beaglebone_Black/bbb_dht_read.cpp \
    Adafruit_Python_DHT/source/Beaglebone_Black/bbb_mmio.cpp \
    Adafruit_Python_DHT/source/common_dht_read.cpp \
    BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp \
    BlackLib/v3_0/BlackCore.cpp \

HEADERS += \
    Adafruit_Python_DHT/source/Beaglebone_Black/bbb_dht_read.h \
    Adafruit_Python_DHT/source/Beaglebone_Black/bbb_mmio.h \
    Adafruit_Python_DHT/source/common_dht_read.h \
    BlackLib/v3_0/BlackGPIO/BlackGPIO.h \
    BlackLib/v3_0/BlackCore.h \
    BlackLib/v3_0/BlackDef.h \
    BlackLib/v3_0/BlackErr.h
