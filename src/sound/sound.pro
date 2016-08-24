# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11 precompile_header
PRECOMPILED_HEADER = stable.h

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
    sample.cpp \
    processor/silence.cpp \
    processor/root.cpp \
    processor/realtimeany.tcc \
    processor/messagebus.cpp

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
    sample.h \
    soundmacros.h \
    processor/silence.h \
    processor/processormacros.h \
    processor/root.h \
    processor/realtimeany.h \
    processor/messagebus.h \
    config.h

INCLUDEPATH += .. ../../vendor
