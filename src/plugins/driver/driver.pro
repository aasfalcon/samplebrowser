# RtAudio driver support

TARGET = driver
VERSION = 4.1.1

include(../plugins.pri)

HEADERS += \
    driverprovider.h

SOURCES += \
    driverprovider.cpp \
    plugindriver.cpp

LIBS += \
    -lrtaudio
