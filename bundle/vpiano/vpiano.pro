QT += gui widgets

TARGET = vpiano
TEMPLATE = lib
CONFIG += staticlib

VPATH += ../../vendor/vpiano-0.9/plugin

SOURCES += \
        keylabel.cpp \
        pianokey.cpp \
        pianokeybd.cpp \
        pianoscene.cpp \
        rawkeybdapp.cpp

HEADERS += \
        keyboardmap.h \
        keylabel.h \
        pianodefs.h \
        pianokey.h \
        pianokeybd.h \
        pianoscene.h \
        rawkeybdapp.h
