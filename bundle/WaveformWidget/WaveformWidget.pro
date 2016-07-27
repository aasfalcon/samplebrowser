QT += widgets

TARGET = WaveformWidget
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
	AudioUtil.cpp \
	WaveformWidget.cpp

HEADERS += \
	AudioUtil.h \
	MathUtil.h \
	WaveformWidget.h
