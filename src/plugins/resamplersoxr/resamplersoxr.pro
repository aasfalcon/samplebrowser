# SoXR-LSR resampler support

VPATH += ../resamplerlsr
include(../resamplerlsr/resamplerlsr.pro)

TARGET = ../resamplersoxr
VERSION = 0.2.1

SOURCES -= pluginresamplerlsr.cpp
SOURCES += pluginresamplersoxr.cpp
LIBS -= -lsamplerate
LIBS += -lsoxr-lsr

INCLUDEPATH += ../resamplerlsr
