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

SOURCES += main.cpp\
    MainWindow.cpp \
    Objects.cpp \
    ObjectsConfiguration.cpp \
    ObjectInput.cpp \
    Device.cpp \
    DeviceChannel.cpp \
    AudioSignal.cpp \
    AudioFile.cpp \
    AudioInfo.cpp \
    AudioChart.cpp \
    Volumeter.cpp \
    ../coder/bitstream.c \
    ../coder/sac_bd_embedder.c \
    ../coder/sac_enc.c \
    ../coder/sac_encoder.c \
    ../coder/sac_huff_cld.c \
    ../coder/sac_huff_cpc.c \
    ../coder/sac_huff_icc.c \
    ../coder/sac_hybfilter.c \
    ../coder/sac_nlc_enc.c \
    ../coder/sac_polyphase.c \
    ../coder/sac_stream.c \
    ../tools/AudioStream.cpp \
    ../tools/Logger.cpp

HEADERS  += mainwindow.h \
    objects.h \
    device.h \
    AudioSignal.h \
    AudioFile.h \
    AudioInfo.h \
    AudioChart.h \
    Volumeter.h \
    ../coder/bitstream.h \
    ../coder/defines.h \
    ../coder/sac_bd_embedder.h \
    ../coder/sac_enc.h \
    ../coder/sac_huff_tab.h \
    ../coder/sac_hybfilter.h \
    ../coder/sac_nlc_enc.h \
    ../coder/sac_polyphase.h \
    ../coder/sac_sbrconst.h \
    ../coder/sac_stream.h \
    ../coder/sac_types.h \
    ../tools/AudioStream.h \
    ../tools/Logger.h \
    ../../lib/QtLibraries/src/qtlibraries.h

FORMS    += mainwindow.ui \
    AudioInfo.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/lib/release/ -lfftw3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/lib/debug/ -lfftw3
else:unix: LIBS += -L$$PWD/../../../../../../../usr/local/lib/ -lfftw3

INCLUDEPATH += $$PWD/../../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/release/libfftw3.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/debug/libfftw3.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/release/fftw3.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/debug/fftw3.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../../../usr/local/lib/libfftw3.a

RESOURCES += \
    media.qrc

unix|win32: LIBS += -L$$PWD/../../lib/AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../lib/AFsp/src
DEPENDPATH += $$PWD/../../lib/AFsp/src

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../lib/AFsp/lib/libtsp.a
