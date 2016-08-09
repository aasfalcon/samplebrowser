# Libsamplerate support

TARGET = resamplerlsr
VERSION = 0.1.8

include(../plugins.pri)

HEADERS += \
    resamplerproviderlsr.h

SOURCES += \
    resamplerproviderlsr.cpp \
    pluginresamplerlsr.cpp

LIBS += \
    -lsamplerate
