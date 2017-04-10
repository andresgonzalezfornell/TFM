TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/sac_arbitraryTree.c \
    src/sac_bitdec.c \
    src/sac_bitinput.c \
    src/sac_blind.c \
    src/sac_calcM1andM2.c \
    src/sac_config.c \
    src/sac_dec.c \
    src/sac_decor.c \
    src/sac_hrtf.c \
    src/sac_hrtffilter.c \
    src/sac_hybfilter.c \
    src/sac_mdct2qmf.c \
    src/sac_mdct2qmf_wf_tables.c \
    src/sac_nlc_dec.c \
    src/sac_nodes_cld.c \
    src/sac_nodes_cpc.c \
    src/sac_nodes_icc.c \
    src/sac_nodes_pt0.c \
    src/sac_nodes_reshape.c \
    src/sac_parallelReverb.c \
    src/sac_partiallyComplex.c \
    src/sac_polyphase.c \
    src/sac_process.c \
    src/sac_resdecode.c \
    src/sac_reshapeBBEnv.c \
    src/sac_reshuffdec1.c \
    src/sac_reshuffdec2.c \
    src/sac_reshuffdec3.c \
    src/sac_reshuffinit.c \
    src/sac_reshufftables.c \
    src/sac_resintrins.c \
    src/sac_restns.c \
    src/sac_smoothing.c \
    src/sac_tonality.c \
    src/sac_TPprocess.c \
    src/spatial_bdreader.c \
    src/spatial_filereader.c \
    src/spatial_frontend.c \
    src/spatial_hrtfreader.c

HEADERS += \
    src/sac_bitdec.h \
    src/sac_bitinput.h \
    src/sac_bitstream.h \
    src/sac_blind.h \
    src/sac_calcM1andM2.h \
    src/sac_chandefs.h \
    src/sac_config.h \
    src/sac_dec.h \
    src/sac_decor.h \
    src/sac_hrtf.h \
    src/sac_huff_nodes.h \
    src/sac_hybfilter.h \
    src/sac_interface.h \
    src/sac_intrins.h \
    src/sac_mdct2qmf.h \
    src/sac_mdct2qmf_wf_tables.h \
    src/sac_nlc_dec.h \
    src/sac_parallelReverb.h \
    src/sac_polyphase.h \
    src/sac_process.h \
    src/sac_resdecode.h \
    src/sac_resdefs.h \
    src/sac_reshapeBBEnv.h \
    src/sac_reshuffdec.h \
    src/sac_reshuffdec_intern.h \
    src/sac_reshuffinit.h \
    src/sac_reshufftables.h \
    src/sac_resintrins.h \
    src/sac_restns.h \
    src/sac_sbrconst.h \
    src/sac_smoothing.h \
    src/sac_stream.h \
    src/sac_tonality.h \
    src/sac_TPprocess.h \
    src/sac_types.h \
    src/spatial_bdreader.h \
    src/spatial_filereader.h \
    src/spatial_hrtfreader.h \
    src/sac_dec_interface.h

unix|win32: LIBS += -L$$PWD/../../AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a
