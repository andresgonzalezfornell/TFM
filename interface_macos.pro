#----------------------------------------------------------
# SAC EFFECTS PLATFORM
# Audio effects platform over Spatial Audio Coding decoder
# Created by: Andrés González Fornell
# Escuela Ténica Superior de Ingenieros de Telecomunicación
# Universidad Politécnica de Madrid
#----------------------------------------------------------

QT       += widgets charts multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "SAC Effects"
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

SOURCES += src/interface/main.cpp\
    src/interface/SACEffects.cpp \
    src/interface/Encoder.cpp \
    src/interface/EffectsMonitor.cpp \
    src/interface/ChannelsList.cpp \
    src/interface/AudioOutput.cpp \
    src/interface/AudioInfo.cpp \
    src/interface/Chart2D.cpp \
    src/process/ProcessManager.cpp \
    src/process/File.cpp \
    src/process/AudioSignal.cpp \
    src/sac/SACBitstream.cpp \
    src/sac/sac_encoder.c \
    src/sac/sac_decoder.c \
    src/tools/Logger.cpp \
    src/effects/Effect.cpp \
    src/effects/Compressor.cpp \
    src/effects/Equalizer.cpp \
    src/effects/Reverb.cpp \
    src/effects/Panning.cpp

HEADERS  += \
    src/interface/SACEffects.h \
    src/interface/Encoder.h \
    src/interface/EffectsMonitor.h \
    src/interface/ChannelsList.h \
    src/interface/AudioOutput.h \
    src/interface/AudioInfo.h \
    src/interface/Chart2D.h \
    src/process/ProcessManager.h \
    src/process/File.h \
    src/process/AudioSignal.h \
    src/sac/SACBitstream.h \
    src/sac/sac_encoder.h \
    src/sac/sac_decoder.h \
    src/tools/Logger.h \
    lib/QtLibraries/src/qtlibraries.h \
    src/effects/Effect.h \
    src/effects/EffectBase.h \
    src/effects/Compressor.h \
    src/effects/Equalizer.h \
    src/effects/Reverb.h \
    src/effects/Panning.h

FORMS    += \
    src/interface/Encoder.ui \
    src/interface/AudioTest.ui \
    src/interface/AudioInfo.ui \
    src/interface/SACEffects.ui \
    src/interface/ChannelsCharts.ui

RESOURCES += \
    src/interface/media.qrc

# FFTw3
LIBS += -L/usr/local/lib/ -lfftw3
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
PRE_TARGETDEPS += /usr/local/lib/libfftw3.a

# AFsp
LIBS += -L$$PWD/lib/AFsp/lib/ -ltsp
INCLUDEPATH += $$PWD/lib/AFsp/include
DEPENDPATH += $$PWD/lib/AFsp/include
PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/libtsp.a

# SAC decoder
LIBS += -L$$PWD/lib/SAC/sac_dec/lib/ -lsac_dec
INCLUDEPATH += $$PWD/lib/SAC/sac_dec/include
DEPENDPATH += $$PWD/lib/SAC/sac_dec/include
PRE_TARGETDEPS += $$PWD/lib/SAC/sac_dec/lib/libsac_dec.a

# SAC encoder
LIBS += -L$$PWD/lib/SAC/sac_enc/lib/ -lsac_enc
INCLUDEPATH += $$PWD/lib/SAC/sac_enc/include
DEPENDPATH += $$PWD/lib/SAC/sac_enc/include
PRE_TARGETDEPS += $$PWD/lib/SAC/sac_enc/lib/libsac_enc.a
