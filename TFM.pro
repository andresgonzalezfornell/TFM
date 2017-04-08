QT       += widgets charts qml quick network opengl sql script scripttools svg xml xmlpatterns multimedia testlib dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TFM
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
    src/interface/Decoder.cpp \
    src/interface/Coder.cpp \
    src/interface/ChannelsList.cpp \
    src/interface/ChannelsConfiguration.cpp \
    src/interface/AudioSignal.cpp \
    src/interface/AudioFile.cpp \
    src/interface/AudioInfo.cpp \
    src/interface/AudioChart.cpp \
    src/interface/Volumeter.cpp \
    src/coder/bitstream.c \
    src/coder/sac_bd_embedder.c \
    src/coder/sac_enc.c \
    src/coder/sac_encoder.c \
    src/coder/sac_huff_cld.c \
    src/coder/sac_huff_cpc.c \
    src/coder/sac_huff_icc.c \
    src/coder/sac_hybfilter.c \
    src/coder/sac_nlc_enc.c \
    src/coder/sac_polyphase.c \
    src/coder/sac_stream.c \
    src/tools/AudioStream.cpp \
    src/tools/Logger.cpp

HEADERS  += \
    src/interface/Decoder.h \
    src/interface/Coder.h \
    src/interface/ChannelsList.h \
    src/interface/ChannelsConfiguration.h \
    src/interface/AudioSignal.h \
    src/interface/AudioFile.h \
    src/interface/AudioInfo.h \
    src/interface/AudioChart.h \
    src/interface/Volumeter.h \
    src/coder/bitstream.h \
    src/coder/defines.h \
    src/coder/sac_bd_embedder.h \
    src/coder/sac_enc.h \
    src/coder/sac_huff_tab.h \
    src/coder/sac_hybfilter.h \
    src/coder/sac_nlc_enc.h \
    src/coder/sac_polyphase.h \
    src/coder/sac_sbrconst.h \
    src/coder/sac_stream.h \
    src/coder/sac_types.h \
    src/tools/AudioStream.h \
    src/tools/Logger.h \
    lib/QtLibraries/src/qtlibraries.h

FORMS    += \
    src/interface/AudioInfo.ui \
    src/interface/Coder.ui \
    src/interface/Decoder.ui

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

RESOURCES += \
    src/interface/media.qrc

unix|win32: LIBS += -L$$PWD/lib/AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/lib/AFsp/src
DEPENDPATH += $$PWD/lib/AFsp/src

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/libtsp.a
