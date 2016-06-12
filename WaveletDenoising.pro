#-------------------------------------------------
#
# Project created by QtCreator 2016-06-12T14:00:35
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = WaveletDenoising
TEMPLATE = app

CONFIG   += C++11

SOURCES  += $$PWD/src/main.cpp\
            $$PWD/src/MainWindow.cpp

HEADERS  += $$PWD/src/MainWindow.h

FORMS    += $$PWD/src/MainWindow.ui
