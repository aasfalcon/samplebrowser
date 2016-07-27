TARGET = ../resamplerlsr
VERSION=1.0.0

include(../plugins.pri)

HEADERS += \
    ../common/resamplerprovider.h \
    resamplerproviderlsr.h

SOURCES += \
    ../common/resamplerprovider.cpp \
    resamplerproviderlsr.cpp \
    pluginresamplerlsr.cpp
