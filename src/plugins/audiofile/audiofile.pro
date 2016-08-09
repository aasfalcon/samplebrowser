# Audio file I/O

TARGET = audiofile
VERSION = 1.0.0

include(../plugins.pri)

HEADERS += \
    audiofileprovider.h \
    audiofileprovidermpg123.h

SOURCES += \
    audiofileprovider.cpp \
    pluginaudiofile.cpp \
    audiofileprovidermpg123.cpp

LIBS += \
    -L../../vendor/libsndfile -lsndfile

INCLUDEPATH += \
    ../../../vendor
