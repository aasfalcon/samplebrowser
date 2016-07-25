#-------------------------------------------------
#
# Project created by QtCreator 2016-07-19T15:33:30
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = widgets
TEMPLATE = lib
CONFIG += staticlib

SOURCES += meter.cpp

HEADERS += meter.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
