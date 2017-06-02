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
    src/sac_enc_hybfilter.c \
    src/sac_enc_polyphase.c \
    src/sac_encoder.c \
    src/sac_huff_cld.c \
    src/sac_huff_cpc.c \
    src/sac_huff_icc.c \
    src/sac_nlc_enc.c \
    src/sac_stream.c

HEADERS += \
    include/bitstream.h \
    include/defines.h \
    include/sac_bd_embedder.h \
    include/sac_enc.h \
    include/sac_enc_hybfilter.h \
    include/sac_enc_polyphase.h \
    include/sac_huff_tab.h \
    include/sac_nlc_enc.h \
    include/sac_sbrconst.h \
    include/sac_stream.h \
    include/sac_types.h

INCLUDEPATH += $$PWD/include

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# AFsp
LIBS += -L$$PWD/../../AFsp/lib/ -ltsp
INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include
PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a
