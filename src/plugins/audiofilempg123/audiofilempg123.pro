# Audio file I/O with MP3 support

TARGET = audiofilempg123
VERSION = 1.0.0

include(../plugins.pri)

HEADERS += \
    audiofileprovidermpg123.h

SOURCES += \
    pluginaudiofilempg123.cpp \
    audiofileprovidermpg123.cpp

LIBS += \
    -L../../vendor/libsndfile -lsndfile

INCLUDEPATH += \
    ../../../vendor
