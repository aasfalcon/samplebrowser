TARGET = audiowaveform

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
VPATH += ../../vendor/audiowaveform/src

SOURCES += \
        AudioFileReader.cpp \
        AudioProcessor.cpp \
        BStdFile.cpp \
        Error.cpp \
        GdImageRenderer.cpp \
        MathUtil.cpp \
        Mp3AudioFileReader.cpp \
        Rgba.cpp \
        SndFileAudioFileReader.cpp \
        TimeUtil.cpp \
        WaveformBuffer.cpp \
        WaveformColors.cpp \
        WaveformGenerator.cpp \
        WaveformRescaler.cpp \
        WavFileWriter.cpp \
        madlld-1.1p1/bstdfile.c \
        madlld-1.1p1/madlld.c

HEADERS += \
        madlld-1.1p1/bstdfile.h \
        Array.h \
        AudioFileReader.h \
        AudioProcessor.h \
        BStdFile.h \
        Error.h \
        GdImageRenderer.h \
        MathUtil.h \
        Mp3AudioFileReader.h \
        Rgba.h \
        SndFileAudioFileReader.h \
        Streams.h \
        TimeUtil.h \
        WaveformBuffer.h \
        WaveformColors.h \
        WaveformGenerator.h \
        WaveformRescaler.h \
        WavFileWriter.h
