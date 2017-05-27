#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T15:14:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "saceffects"
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/process/ProcessManager.cpp \
    src/process/File.cpp \
    src/process/AudioStream.cpp \
    src/process/AudioSignal.cpp \
    src/sac/sac_decoder.c \
    src/sac/sac_encoder.c \
    src/effects/Effect.cpp \
    src/tools/Logger.cpp \
    src/effects/Compressor.cpp \
    src/effects/Equalizer.cpp \
    src/effects/Reverb.cpp \
    src/sac/SACBitstream.cpp

HEADERS  += \
    src/process/ProcessManager.h \
    src/process/AudioStream.h \
    src/process/File.h \
    src/process/AudioSignal.h \
    src/sac/sac_decoder.h \
    src/sac/sac_encoder.h \
    src/tools/Logger.h \
    src/effects/Compressor.h \
    src/effects/EffectBase.h \
    src/effects/Effect.h \
    src/effects/Equalizer.h \
    src/effects/Reverb.h \
    src/sac/SACBitstream.h

unix|win32: LIBS += -L$$PWD/lib/SAC/sac_enc/lib/ -lsac_enc

INCLUDEPATH += $$PWD/lib/SAC/sac_enc/include
DEPENDPATH += $$PWD/lib/SAC/sac_enc/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_enc/lib/sac_enc.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_enc/lib/libsac_enc.a

unix|win32: LIBS += -L$$PWD/lib/SAC/sac_dec/lib/ -lsac_dec

INCLUDEPATH += $$PWD/lib/SAC/sac_dec/include
DEPENDPATH += $$PWD/lib/SAC/sac_dec/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_dec/lib/sac_dec.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_dec/lib/libsac_dec.a

unix|win32: LIBS += -L/usr/local/lib/ -lfftw3

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

win32:!win32-g++: PRE_TARGETDEPS += /usr/local/lib/fftw3.lib
else:unix|win32-g++: PRE_TARGETDEPS += /usr/local/lib/libfftw3.a

unix|win32: LIBS += -L$$PWD/lib/AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/lib/AFsp/include
DEPENDPATH += $$PWD/lib/AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/libtsp.a
