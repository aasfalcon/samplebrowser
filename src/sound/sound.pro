# Sound engine

TARGET = sound
TEMPLATE = lib
CONFIG += staticlib c++11

SOURCES += \
    sample.cpp \
    buffer.cpp \
    frame.cpp \
    converter.cpp \
    processor.cpp \
    leveler.cpp \
    constframe.cpp \
    resampler.cpp \
    sound.cpp \
    chain.cpp \
    driver.cpp \
    meter.cpp \
    player.cpp \
    inputstream.cpp \
    chunk.cpp

HEADERS += \
    sample.h \
    buffer.h \
    frame.h \
    sound.h \
    converter.h \
    processor.h \
    leveler.h \
    constframe.h \
    resampler.h \
    config.h \
    chain.h \
    driver.h \
    meter.h \
    player.h \
    inputstream.h \
    chunk.h

INCLUDEPATH += ..
