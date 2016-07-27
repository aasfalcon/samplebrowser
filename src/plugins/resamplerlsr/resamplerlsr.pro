QT =
TARGET = ../resamplerlsr
TEMPLATE = lib
CONFIG += c++11
DEFINES += SERVER_LIBRARY

HEADERS += \
    ../common/resamplerprovider.h \
    resamplerproviderlsr.h

SOURCES += \
    ../common/resamplerprovider.cpp \
    resamplerproviderlsr.cpp \
    pluginresamplerlsr.cpp

INCLUDEPATH += ../
