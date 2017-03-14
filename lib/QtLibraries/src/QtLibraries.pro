#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T00:22:35
#
#-------------------------------------------------

QT       += widgets qml quick network opengl sql script scripttools svg xml xmlpatterns multimedia testlib dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = QtLibraries
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += qtlibraries.cpp

HEADERS += qtlibraries.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Library - Qt5 Ui Tools

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/ -lQt5UiTools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/ -lQt5UiTools
else:unix: LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -lQt5UiTools

INCLUDEPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
DEPENDPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/libQt5UiTools.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/libQt5UiTools.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/Qt5UiTools.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/Qt5UiTools.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/libQt5UiTools.a

# Framework - Qt GUI

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/ -lQtGui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/ -lQtGui
else:mac: LIBS += -F$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -framework QtGui
else:unix: LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -lQtGui

INCLUDEPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
DEPENDPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include

# Framework - Qt Core

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/ -lQtCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/ -lQtCore
else:mac: LIBS += -F$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -framework QtCore
else:unix: LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -lQtCore

INCLUDEPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
DEPENDPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include

# Framework - Qt Widgets

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/ -lQtWidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/ -lQtWidgets
else:mac: LIBS += -F$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -framework QtWidgets
else:unix: LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -lQtWidgets

INCLUDEPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
DEPENDPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include

# Framework - Qt Multimedia

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/release/ -lQtMultimedia
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/debug/ -lQtMultimedia
else:mac: LIBS += -F$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -framework QtMultimedia
else:unix: LIBS += -L$$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/lib/ -lQtMultimedia

INCLUDEPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
DEPENDPATH += $$PWD/../../../../../../../../Applications/Qt/5.8/clang_64/include
