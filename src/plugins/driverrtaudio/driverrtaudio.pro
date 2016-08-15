# RtAudio driver support

TARGET = driver
VERSION = 4.1.1

include(../plugins.pri)

HEADERS += \
    driverproviderrtaudio.h

SOURCES += \
    driverproviderrtaudio.cpp \
    plugindriverrtaudio.cpp

LIBS += \
    -lrtaudio
