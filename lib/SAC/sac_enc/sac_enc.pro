TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

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

unix|win32: LIBS += -L$$PWD/../../AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a

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
