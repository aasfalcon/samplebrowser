# Audio file I/O (libsndfile wrapper)

TARGET = audiofilesndfile
VERSION = 1.0.0

include(../plugins.pri)

HEADERS += \
    audiofilesndfile.h

SOURCES += \
    pluginaudiofilesndfile.cpp \
    audiofilesndfile.cpp

LIBS += \
    -L../../vendor/libsndfile -lsndfile

INCLUDEPATH += \
    ../../../vendor
