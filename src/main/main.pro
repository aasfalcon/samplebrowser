QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../wexplorer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    driverdialog.cpp \
    browser.cpp

HEADERS += \
    mainwindow.h \
    driverdialog.h \
    browser.h

FORMS += \
    mainwindow.ui \
    driverdialog.ui \
    browser.ui

LIBS += \
    -L../engine -lengine \
    -L../widgets -lwidgets \
    -L../plugins -lweplsh \
    -L../sound -lsound \
    \
    -L../../bundle/qvumeter -lqvumeter \
    -L../../bundle/WaveformWidget -lWaveformWidget \
    -L../../bundle/audiowaveform -laudiowaveform \
    -L../../bundle/vpiano -lvpiano \
    \
    -L/usr/local/lib -lsndfile \
    -ljack \
    -lpng12 \
    -lltdl

INCLUDEPATH += ../ ../../bundle ../../vendor ../plugins

CONFIG += c++11

PRE_TARGETDEPS += ../../bundle ../engine ../widgets ../plugins

RESOURCES += \
    resources.qrc
