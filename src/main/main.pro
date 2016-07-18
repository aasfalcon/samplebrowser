#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T19:00:26
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS         +=  mainwindow.h

FORMS           +=  mainwindow.ui

LIBS            +=  -L../engine -lengine \
                    -L../../vendor/qtjack -lqtjack \
                    -ljack -lsoxr \
                    -L/usr/local/lib -lsndfile

INCLUDEPATH     +=  ../ ../../vendor

PRE_TARGETDEPS  += ../engine ../../vendor/qtjack

RESOURCES += \
    resources.qrc
