#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T17:05:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = sac_enc
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

SOURCES += \
    src/bitstream.c \
    src/sac_bd_embedder.c \
    src/sac_enc.c \
    src/sac_encoder.c \
    src/sac_huff_cld.c \
    src/sac_huff_cpc.c \
    src/sac_huff_icc.c \
    src/sac_hybfilter.c \
    src/sac_nlc_enc.c \
    src/sac_polyphase.c \
    src/sac_stream.c

HEADERS += \
    src/bitstream.h \
    src/defines.h \
    src/sac_bd_embedder.h \
    src/sac_enc.h \
    src/sac_huff_tab.h \
    src/sac_hybfilter.h \
    src/sac_nlc_enc.h \
    src/sac_polyphase.h \
    src/sac_sbrconst.h \
    src/sac_stream.h \
    src/sac_types.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../../AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a
