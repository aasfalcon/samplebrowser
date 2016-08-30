# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11 precompile_header
PRECOMPILED_HEADER = stable.h

SOURCES += \
    buffer.cpp \
    constframe.cpp \
    driver.cpp \
    frame.cpp \
    object.cpp \
    processor/base.cpp \
    processor/leveler.cpp \
    processor/messagebus.cpp \
    processor/meter.cpp \
    processor/player.cpp \
    processor/processor.cpp \
    processor/resampler.cpp \
    processor/ringbuffer.cpp \
    processor/ringfeeder.tcc \
    processor/root.cpp \
    processor/silence.cpp \
    sample.cpp \
    stream/basicstream.cpp \
    stream/chunk.cpp \
    processor/event.cpp \
    processor/model.cpp \
    object.tcc \
    dither.cpp \
    sample.tcc

HEADERS += \
    buffer.h \
    config.h \
    constframe.h \
    driver.h \
    frame.h \
    object.h \
    processor/base.h \
    processor/leveler.h \
    processor/messagebus.h \
    processor/meter.h \
    processor/player.h \
    processor/processor.h \
    processor/processormacros.h \
    processor/resampler.h \
    processor/ringbuffer.h \
    processor/ringfeeder.h \
    processor/root.h \
    processor/silence.h \
    sample.h \
    soundmacros.h \
    stream/basicstream.h \
    stream/chunk.h \
    stream/inputstream.h \
    stream/iostream.h \
    stream/outputstream.h \
    processor/shortcuts.h \
    processor/event.h \
    processor/model.h \
    processor/watcher.h \
    dither.h

INCLUDEPATH += .. ../../vendor
