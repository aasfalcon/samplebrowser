QT =
TARGET = ../server
TEMPLATE = lib
CONFIG += c++11
DEFINES += SERVER_LIBRARY

SOURCES +=\
    ../common/plugin.cpp \
    pluginserver.cpp \
    serverprovider.cpp

HEADERS +=\
    ../iplugin.h \
    ../iserverprovider.h \
    ../common/plugin.h \
    serverprovider.h

INCLUDEPATH += ../
