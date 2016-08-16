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
    /home/android/Develop/WAVExplorer/vendor/libsndfile/build/src/.libs/libsndfile.a

INCLUDEPATH += \
    ../../..
