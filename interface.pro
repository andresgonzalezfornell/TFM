QT       += widgets charts qml quick network opengl sql script scripttools svg xml xmlpatterns multimedia testlib dbus

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
    src/effects/Reverb.cpp

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
    src/effects/Reverb.h

FORMS    += \
    src/interface/Encoder.ui \
    src/interface/AudioTest.ui \
    src/interface/AudioInfo.ui \
    src/interface/SACEffects.ui \
    src/interface/ChannelsCharts.ui

RESOURCES += \
    src/interface/media.qrc

# Libraries

# fftw3

win32:CONFIG(release, debug|release): LIBS += -L/usr/local/lib/release/ -lfftw3
else:win32:CONFIG(debug, debug|release): LIBS += -L/usr/local/lib/debug/ -lfftw3
else:unix: LIBS += -L/usr/local/lib/ -lfftw3

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/local/lib/release/libfftw3.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/local/lib/debug/libfftw3.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /usr/local/lib/release/fftw3.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /usr/local/lib/debug/fftw3.lib
else:unix: PRE_TARGETDEPS += /usr/local/lib/libfftw3.a

# libtsp

unix: LIBS += -L$$PWD/lib/AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/lib/AFsp/include
DEPENDPATH += $$PWD/lib/AFsp/include

unix: PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/libtsp.a

# sac_enc

unix: LIBS += -L$$PWD/lib/SAC/sac_enc/lib/ -lsac_enc

INCLUDEPATH += $$PWD/lib/SAC/sac_enc/include
DEPENDPATH += $$PWD/lib/SAC/sac_enc/include

unix: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_enc/lib/libsac_enc.a

# sac_dec

unix: LIBS += -L$$PWD/lib/SAC/sac_dec/lib/ -lsac_dec

INCLUDEPATH += $$PWD/lib/SAC/sac_dec/include
DEPENDPATH += $$PWD/lib/SAC/sac_dec/include

unix: PRE_TARGETDEPS += $$PWD/lib/SAC/sac_dec/lib/libsac_dec.a
