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
    include/sac_bitdec.h \
    include/sac_bitinput.h \
    include/sac_bitstream.h \
    include/sac_blind.h \
    include/sac_calcM1andM2.h \
    include/sac_chandefs.h \
    include/sac_config.h \
    include/sac_dec.h \
    include/sac_dec_interface.h \
    include/sac_decor.h \
    include/sac_hrtf.h \
    include/sac_huff_nodes.h \
    include/sac_hybfilter.h \
    include/sac_interface.h \
    include/sac_intrins.h \
    include/sac_mdct2qmf.h \
    include/sac_mdct2qmf_wf_tables.h \
    include/sac_nlc_dec.h \
    include/sac_parallelReverb.h \
    include/sac_polyphase.h \
    include/sac_process.h \
    include/sac_resdecode.h \
    include/sac_resdefs.h \
    include/sac_reshapeBBEnv.h \
    include/sac_reshuffdec.h \
    include/sac_reshuffdec_intern.h \
    include/sac_reshuffinit.h \
    include/sac_reshufftables.h \
    include/sac_resintrins.h \
    include/sac_restns.h \
    include/sac_sbrconst.h \
    include/sac_smoothing.h \
    include/sac_stream.h \
    include/sac_tonality.h \
    include/sac_TPprocess.h \
    include/sac_types.h \
    include/spatial_bdreader.h \
    include/spatial_filereader.h \
    include/spatial_hrtfreader.h

INCLUDEPATH += $$PWD/include

unix|win32: LIBS += -L$$PWD/../../AFsp/lib/ -ltsp

INCLUDEPATH += $$PWD/../../AFsp/include
DEPENDPATH += $$PWD/../../AFsp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/tsp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../AFsp/lib/libtsp.a
