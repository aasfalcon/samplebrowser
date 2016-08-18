# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += \
    chunk.cpp \
    driver.cpp \
    basicstream.cpp

HEADERS += \
    buffer.h \
    buffer.tcc \
    chain.h \
    chain.tcc \
    chunk.h \
    config.h \
    constframe.h \
    constframe.tcc \
    driver.h \
    frame.h \
    frame.tcc \
    inputstream.h \
    inputstream.tcc \
    leveler.h \
    leveler.tcc \
    meter.h \
    meter.tcc \
    player.h \
    player.tcc \
    processor.h \
    processor.tcc \
    resampler.h \
    resampler.tcc \
    sample.h \
    sample.tcc \
    sound.h \
    sound.tcc \
    basicstream.h \
    outputstream.h \
    outputstream.tcc \
    iostream.h \
    iostream.tcc \
    ringbuffer.h \
    ringbuffer.tcc

INCLUDEPATH += .. ../../vendor
