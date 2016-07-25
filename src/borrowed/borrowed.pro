#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T02:34:32
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = borrowed
TEMPLATE = lib
CONFIG += staticlib

SOURCES += borrowed.cpp

HEADERS += borrowed.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
