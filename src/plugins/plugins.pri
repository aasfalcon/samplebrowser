QT =
TARGET = ../$$TARGET
TEMPLATE = lib
CONFIG += c++11
QMAKE_CXXFLAGS_RELEASE += -fno-rtti
DEFINES += SERVER_LIBRARY PLUGIN_VERSION=\\\"$$VERSION\\\"
INCLUDEPATH += ../.. ../../../vendor
LIBS += \
    -L../../shared -lweplsh
