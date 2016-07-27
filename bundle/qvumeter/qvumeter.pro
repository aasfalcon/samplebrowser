QT += widgets

TARGET = qvumeter
TEMPLATE = lib
CONFIG += staticlib

VPATH += ../../vendor/radit-essential/qvumeter
INCLUDEPATH += ../../vendor/radit-essential/qvumeter

SOURCES += \
    	qvumeter.cpp

HEADERS += \
	qvumeter.h
