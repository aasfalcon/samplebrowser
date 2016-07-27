QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../wexplorer
TEMPLATE = app

SOURCES += \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	mainwindow.h

FORMS += \
	mainwindow.ui

LIBS += \
	-L../engine -lengine \
	-L../widgets -lwidgets \
        \
	-L../../bundle/qtjack -lqtjack \
        -L../../bundle/qvumeter -lqvumeter \
        -L../../bundle/WaveformWidget -lWaveformWidget \
        -L../../bundle/audiowaveform -laudiowaveform \
        -L../../bundle/vpiano -lvpiano \
        \
        -L/usr/local/lib -lsndfile \
	-ljack \
        -lsamplerate \
        #-lsoxr-lsr \
        -lpng12 \
        -lltdl

INCLUDEPATH += ../ ../../bundle ../../vendor

CONFIG += c++11

PRE_TARGETDEPS += ../../bundle ../engine ../widgets ../plugins

RESOURCES += \
	resources.qrc
