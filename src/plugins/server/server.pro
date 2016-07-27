TARGET = ../server
VERSION=1.0.0

include(../plugins.pri)

SOURCES +=\
    ../common/plugin.cpp \
    pluginserver.cpp \
    serverprovider.cpp

HEADERS +=\
    ../iplugin.h \
    ../iserverprovider.h \
    ../common/plugin.h \
    serverprovider.h
