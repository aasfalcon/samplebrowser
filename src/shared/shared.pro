# Plugins shared library

TARGET = weplsh
VERSION = 1.0.0

QT =
TEMPLATE = lib
CONFIG += staticlib c++11
INCLUDEPATH += ../

SOURCES +=\
    version.cpp \
    server.cpp \
    plugin.cpp \
    allocator.cpp \
    interface.cpp

HEADERS +=\
    version.h \
    server.h \
    plugin.h \
    allocator.h \
    \
    iaudiofile.h \
    interface.h \
    idriver.h \
    iplugin.h \
    iresampler.h \
    iserver.h \
    pluginmacros.h
