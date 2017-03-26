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
    ../tools/Logger.cpp \
    ../module/AudioObject.cpp \
    ../module/AudioSignal.cpp \
    ObjectsConfiguration.cpp \
    Objects.cpp \
    ObjectInput.cpp \
    DeviceChannel.cpp \
    DeviceLevel.cpp \
    Device.cpp \
    AudioChart.cpp \
    MainWindow.cpp

HEADERS  += mainwindow.h \
    ../tools/Logger.h \
    ../module/AudioObject.h \
    ../module/AudioSignal.h \
    ../../lib/QtLibraries/src/qtlibraries.h \
    objects.h \
    device.h \
    audiochart.h

FORMS    += mainwindow.ui

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
