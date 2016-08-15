# Libsamplerate support

TARGET = resamplerlsr
VERSION = 0.1.8

include(../plugins.pri)

HEADERS += \
    resamplerlsr.h

SOURCES += \
    pluginresamplerlsr.cpp \
    resamplerlsr.cpp

LIBS += \
    -lsamplerate
