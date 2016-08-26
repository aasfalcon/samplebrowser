# Plugins shared library

TARGET = weplsh
VERSION = 1.0.0

QT =
TEMPLATE = lib
CONFIG += staticlib c++11 precompile_header
PRECOMPILED_HEADER = stable.h

INCLUDEPATH += .. ../../vendor

SOURCES +=\
    version.cpp \
    server.cpp \
    plugin.cpp \
    interface.cpp \
    value.tcc

HEADERS +=\
    version.h \
    server.h \
    plugin.h \
    allocator.h \
    iaudiofile.h \
    interface.h \
    idriver.h \
    iplugin.h \
    iresampler.h \
    iserver.h \
    pluginmacros.h \
    log.h \
    any.h \
    ring.h \
    id.h \
    value.h \
    bus.h
