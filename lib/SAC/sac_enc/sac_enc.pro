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

HEADERS += \
    include/bitstream.h \
    include/defines.h \
    include/sac_bd_embedder.h \
    include/sac_enc.h \
    include/sac_huff_tab.h \
    include/sac_hybfilter.h \
    include/sac_nlc_enc.h \
    include/sac_polyphase.h \
    include/sac_sbrconst.h \
    include/sac_stream.h \
    include/sac_types.h

INCLUDEPATH += $$PWD/include

unix|win32: LIBS += -L$$PWD/../../AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a
