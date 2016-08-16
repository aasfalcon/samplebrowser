# Plugins bundle

TEMPLATE = subdirs

include(../shared/shared.pri)

SUBDIRS += \
    driverrtaudio \
    resamplerlsr \
    resamplersoxr \
    audiofilesndfile \
    audiofilempg123
