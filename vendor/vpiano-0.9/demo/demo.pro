TEMPLATE = app
TARGET = vpianodemo
QT += core \
    gui
HEADERS += vpiano.h
SOURCES += main.cpp \
    vpiano.cpp
INCLUDEPATH += ../plugin
LIBS += -L../plugin \
    -lvpianokeybd
CONFIG += release
linux* {
    CONFIG += x11
}
win32 { 
    CONFIG -= debug
    CONFIG -= debug_and_release
    CONFIG += console
}
macx {
    LIBS += -framework Carbon
}
FORMS += demo.ui
