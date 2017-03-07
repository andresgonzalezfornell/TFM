#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T15:30:21
#
#-------------------------------------------------

QT       += core gui

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
        mainwindow.cpp \
    ../module/Rectangle.cpp \
    ../tools/Logger.cpp \
    audioinput.cpp

HEADERS  += mainwindow.h \
    ../module/Rectangle.h \
    ../tools/Logger.h \
    ../../lib/QtLibraries/src/qtlibraries.h \
    audioinput.h

FORMS    += mainwindow.ui

INCLUDEPATH += "../../lib/QtLibraries/src/"

LIBS += "../lib/QtLibraries/lib/libQtLibraries.a"


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/QtLibraries/lib/release/ -lQtLibraries
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/QtLibraries/lib/debug/ -lQtLibraries
else:unix: LIBS += -L$$PWD/../../lib/QtLibraries/lib/ -lQtLibraries

INCLUDEPATH += $$PWD/../../lib/QtLibraries
DEPENDPATH += $$PWD/../../lib/QtLibraries

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtLibraries/lib/release/libQtLibraries.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtLibraries/lib/debug/libQtLibraries.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtLibraries/lib/release/QtLibraries.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/QtLibraries/lib/debug/QtLibraries.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../lib/QtLibraries/lib/libQtLibraries.a
