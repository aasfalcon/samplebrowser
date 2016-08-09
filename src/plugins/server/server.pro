# Plugin server

TARGET = server
VERSION = 1.0.0

include(../plugins.pri)

SOURCES +=\
    pluginserver.cpp \
    serverprovider.cpp

HEADERS +=\
    serverprovider.h
