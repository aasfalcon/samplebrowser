#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T19:37:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = engine
TEMPLATE = lib
CONFIG += staticlib

SOURCES += player.cpp \
    soundfile.cpp \
    client.cpp \
    scanner.cpp

HEADERS += player.h \
    soundfile.h \
    client.h \
    scanner.h \
    config.h

INCLUDEPATH += ../../vendor
