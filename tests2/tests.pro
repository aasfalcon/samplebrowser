TARGET = ../samplebrowser-test

TEMPLATE = app
CONFIG += console c++11 precompile_header
CONFIG -= app_bundle
CONFIG -= qt

PRECOMPILED_HEADER = stable.h

SOURCES += main.cpp
LIBS += -lgtest
LIBS += -lgtest_main
