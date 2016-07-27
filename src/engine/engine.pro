#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T19:37:47
#
#-------------------------------------------------

TARGET = engine
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    player.cpp \
    client.cpp \
    scanner.cpp \
    helper.cpp \
    sound/file.cpp \
    sound/format.cpp \
    sound/chunk.cpp \
    sound/error.cpp \
    sound/input.cpp \
    sound/output.cpp \
    sound/resampler.cpp \
    sound/buffer.cpp \
    sound/resamplerbase.cpp \
    sound/realtimeresampler.cpp \
    sound/resamplerbuffer.cpp \
    sound/interface.cpp

HEADERS += \
    player.h \
    client.h \
    scanner.h \
    config.h \
    helper.h \
    sound/buffer.h \
    sound/file.h \
    sound/format.h \
    sound/frame.h \
    sound/chunk.h \
    sound/error.h \
    sound/input.h \
    sound/output.h \
    sound/resampler.h \
    sound/resamplerbase.h \
    sound/realtimeresampler.h \
    sound/resamplerbuffer.h \
    sound/interface.h

INCLUDEPATH += ../../bundle ../../vendor ../
