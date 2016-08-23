# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += \
    basicstream.cpp \
    buffer.cpp \
    chunk.cpp \
    constframe.cpp \
    driver.cpp \
    frame.cpp \
    processor/leveler.cpp \
    processor/meter.cpp \
    object.cpp \
    processor/base.cpp \
    processor/player.cpp \
    processor/processor.cpp \
    processor/resampler.cpp \
    ringbuffer.cpp \
    ringfeeder.tcc \
    runtime.cpp \
    sample.cpp \
    processor/silence.cpp \
    processor/test.cpp

HEADERS += \
    basicstream.h \
    buffer.h \
    chunk.h \
    constframe.h \
    driver.h \
    frame.h \
    inputstream.h \
    iostream.h \
    processor/leveler.h \
    processor/meter.h \
    object.h \
    outputstream.h \
    processor/base.h \
    processor/player.h \
    processor/processor.h \
    processor/resampler.h \
    ringbuffer.h \
    ringfeeder.h \
    runtime.h \
    sample.h \
    soundmacros.h \
    processor/register.h \
    processor/silence.h \
    id.h

INCLUDEPATH += .. ../../vendor
