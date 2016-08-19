QT =
TARGET = ../$$TARGET
TEMPLATE = lib
CONFIG += c++11
DEFINES += SERVER_LIBRARY PLUGIN_VERSION=\\\"$$VERSION\\\"
INCLUDEPATH += ../.. ../../../vendor
LIBS += \
    -L../../shared -lweplsh
