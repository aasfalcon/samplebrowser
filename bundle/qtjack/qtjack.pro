TARGET = qtjack
TEMPLATE = lib
CONFIG += staticlib no_keywords

VPATH += ../../vendor/qtjack

SOURCES += \
        audiobuffer.cpp \
        audioport.cpp \
        buffer.cpp \
        client.cpp \
        driver.cpp \
        midibuffer.cpp \
        midievent.cpp \
        midiport.cpp \
        parameter.cpp \
        port.cpp \
        server.cpp \
        system.cpp

HEADERS += \
        audiobuffer.h \
        audioport.h \
        buffer.h \
        client.h \
        driver.h \
        global.h \
        midibuffer.h \
        midievent.h \
        midiport.h \
        parameter.h \
        port.h \
        processor.h \
        ringbuffer.h \
        server.h \
        system.h
