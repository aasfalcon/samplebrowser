# RtAudio driver support

TARGET = driver
VERSION = 4.1.1

include(../plugins.pri)

HEADERS += \
    driverrtaudio.h

SOURCES += \
    plugindriverrtaudio.cpp \
    driverrtaudio.cpp

LIBS += \
    -lrtaudio
