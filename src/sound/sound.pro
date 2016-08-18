# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += \
    chunk.cpp \
    driver.cpp \
    basicstream.cpp \
    sound.cpp \
    runtime.cpp \
    ringbuffer.cpp \
    resampler.cpp \
    processor.cpp \
    player.cpp \
    outputstream.cpp \
    meter.cpp \
    leveler.cpp \
    iostream.cpp \
    inputstream.cpp \
    frame.cpp \
    constframe.cpp \
    chain.cpp \
    buffer.cpp \
    sample.cpp

HEADERS += \
    basicstream.h \
    buffer.h \
    chain.h \
    chunk.h \
    config.h \
    constframe.h \
    driver.h \
    frame.h \
    inputstream.h \
    iostream.h \
    leveler.h \
    meter.h \
    outputstream.h \
    player.h \
    processor.h \
    resampler.h \
    ringbuffer.h \
    runtime.h \
    sample.h \
    sound.h

INCLUDEPATH += .. ../../vendor
