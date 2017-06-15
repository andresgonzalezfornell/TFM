# Output

DIR		= bin/commandline/
TARGET		= $(DIR)saceffects

# Compiler, tools and options

CC		= gcc
CXX		= g++ -std=c++11
CFLAGS		= -pipe -g -Wall -W -fPIC $(DEFINES)
CXXFLAGS	= -pipe -g -std=gnu++11 -Wall -W -fPIC $(DEFINES)
DEFINES		= -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH		= -I. -I/usr/local/include -Ilib/AFsp/include -Ilib/SAC/sac_dec/include
DEL_FILE	= rm -f
LIBS		= $(SUBLIBS) -L/usr/local/lib/ -lfftw3 -Llib/AFsp/lib/ -ltsp -Llib/SAC/sac_dec/lib/ -lsac_dec

# Effects

DIR_EFFECTS = src/effects/

SOURCES_EFFECTS = \
		$(DIR_EFFECTS)Compressor.cpp \
		$(DIR_EFFECTS)Equalizer.cpp \
		$(DIR_EFFECTS)Panning.cpp \
		$(DIR_EFFECTS)Reverb.cpp

OBJECTS_EFFECTS = $(SOURCES_EFFECTS:%.cpp=%.o)

HEADERS_EFFECTS = $(SOURCES_EFFECTS:%.cpp=%.h)

COMPILE_EFFECTS = \
		src/process/File.h \
		src/sac/SACBitstream.h \
		src/effects/EffectBase.h \
		src/tools/Logger.h

# Files

SOURCES		= src/main.cpp \
		src/process/ProcessManager.cpp \
		src/process/File.cpp \
		src/process/AudioSignal.cpp \
		src/effects/Effect.cpp \
		src/sac/SACBitstream.cpp \
		src/sac/sac_decoder.c \
		src/tools/Logger.cpp

OBJECTS		= $(DIR)main.o \
		$(DIR)ProcessManager.o \
		$(DIR)File.o \
		$(DIR)AudioSignal.o \
		$(DIR)Effect.o \
		$(DIR)SACBitstream.o \
		$(DIR)sac_decoder.o \
		$(DIR)Logger.o \

SOURCES_LIBS	= \
		lib/AFsp/libtsp/AF/dataio/AFdRdAlaw.c \
		lib/AFsp/libtsp/AF/dataio/AFdRdFx.c \
		lib/AFsp/libtsp/AF/dataio/AFdRdIx.c \
		lib/AFsp/libtsp/AF/dataio/AFdRdMulaw.c \
		lib/AFsp/libtsp/AF/dataio/AFdRdTA.c \
		lib/AFsp/libtsp/AF/dataio/AFdRdU1.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrAlaw.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrFx.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrIx.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrMulaw.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrTA.c \
		lib/AFsp/libtsp/AF/dataio/AFdWrU1.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdAlaw.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdFx.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdIx.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdMulaw.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdTA.c \
		lib/AFsp/libtsp/AF/dataio/AFfRdU1.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrAlaw.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrFx.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrIx.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrMulaw.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrTA.c \
		lib/AFsp/libtsp/AF/dataio/AFfWrU1.c \
		lib/AFsp/libtsp/AF/dataio/AFgetLine.c \
		lib/AFsp/libtsp/AF/dataio/AFposition.c \
		lib/AFsp/libtsp/AF/dataio/AFseek.c \
		lib/AFsp/libtsp/AF/dataio/AFtell.c \
		lib/AFsp/libtsp/AF/header/AFdecSpeaker.c \
		lib/AFsp/libtsp/AF/header/AFfindType.c \
		lib/AFsp/libtsp/AF/header/AFgetInfoRec.c \
		lib/AFsp/libtsp/AF/header/AFoptions.c \
		lib/AFsp/libtsp/AF/header/AFprAFpar.c \
		lib/AFsp/libtsp/AF/header/AFpreSetWPar.c \
		lib/AFsp/libtsp/AF/header/AFrdAIhead.c \
		lib/AFsp/libtsp/AF/header/AFrdAUhead.c \
		lib/AFsp/libtsp/AF/header/AFrdBLhead.c \
		lib/AFsp/libtsp/AF/header/AFrdEShead.c \
		lib/AFsp/libtsp/AF/header/AFrdINhead.c \
		lib/AFsp/libtsp/AF/header/AFrdNShead.c \
		lib/AFsp/libtsp/AF/header/AFrdSFhead.c \
		lib/AFsp/libtsp/AF/header/AFrdSPhead.c \
		lib/AFsp/libtsp/AF/header/AFrdSWhead.c \
		lib/AFsp/libtsp/AF/header/AFrdTAhead.c \
		lib/AFsp/libtsp/AF/header/AFrdTextAFsp.c \
		lib/AFsp/libtsp/AF/header/AFrdWVhead.c \
		lib/AFsp/libtsp/AF/header/AFreadHead.c \
		lib/AFsp/libtsp/AF/header/AFsetNHread.c \
		lib/AFsp/libtsp/AF/header/AFsetNHwrite.c \
		lib/AFsp/libtsp/AF/header/AFsetRead.c \
		lib/AFsp/libtsp/AF/header/AFsetWrite.c \
		lib/AFsp/libtsp/AF/header/AFupdAIhead.c \
		lib/AFsp/libtsp/AF/header/AFupdAUhead.c \
		lib/AFsp/libtsp/AF/header/AFupdHead.c \
		lib/AFsp/libtsp/AF/header/AFupdWVhead.c \
		lib/AFsp/libtsp/AF/header/AFwrAIhead.c \
		lib/AFsp/libtsp/AF/header/AFwrAUhead.c \
		lib/AFsp/libtsp/AF/header/AFwriteHead.c \
		lib/AFsp/libtsp/AF/header/AFwrWVhead.c \
		lib/AFsp/libtsp/AF/AFclose.c \
		lib/AFsp/libtsp/AF/AFdReadData.c \
		lib/AFsp/libtsp/AF/AFdWriteData.c \
		lib/AFsp/libtsp/AF/AFfReadData.c \
		lib/AFsp/libtsp/AF/AFfWriteData.c \
		lib/AFsp/libtsp/AF/AFopenRead.c \
		lib/AFsp/libtsp/AF/AFopenWrite.c \
		lib/AFsp/libtsp/AF/AFopnRead.c \
		lib/AFsp/libtsp/AF/AFopnWrite.c \
		lib/AFsp/libtsp/AF/AFreadData.c \
		lib/AFsp/libtsp/AF/AFsetFileType.c \
		lib/AFsp/libtsp/AF/AFsetInfo.c \
		lib/AFsp/libtsp/AF/AFsetNHpar.c \
		lib/AFsp/libtsp/AF/AFsetSpeaker.c \
		lib/AFsp/libtsp/AF/AFwriteData.c \
		lib/AFsp/libtsp/FI/nucleus/FIrCosF.c \
		lib/AFsp/libtsp/FI/nucleus/FIxKaiser.c \
		lib/AFsp/libtsp/FI/FIdBiquad.c \
		lib/AFsp/libtsp/FI/FIdConvol.c \
		lib/AFsp/libtsp/FI/FIdConvSI.c \
		lib/AFsp/libtsp/FI/FIdFiltAP.c \
		lib/AFsp/libtsp/FI/FIdFiltIIR.c \
		lib/AFsp/libtsp/FI/FIdKaiserLPF.c \
		lib/AFsp/libtsp/FI/FIdReadFilt.c \
		lib/AFsp/libtsp/FI/FIdWinHamm.c \
		lib/AFsp/libtsp/FI/FIdWinKaiser.c \
		lib/AFsp/libtsp/FI/FIfBiquad.c \
		lib/AFsp/libtsp/FI/FIfConvol.c \
		lib/AFsp/libtsp/FI/FIfConvSI.c \
		lib/AFsp/libtsp/FI/FIfDeem.c \
		lib/AFsp/libtsp/FI/FIfFiltAP.c \
		lib/AFsp/libtsp/FI/FIfFiltIIR.c \
		lib/AFsp/libtsp/FI/FIfKaiserLPF.c \
		lib/AFsp/libtsp/FI/FIfPreem.c \
		lib/AFsp/libtsp/FI/FIfReadFilt.c \
		lib/AFsp/libtsp/FI/FIfWinHamm.c \
		lib/AFsp/libtsp/FI/FIfWinHCos.c \
		lib/AFsp/libtsp/FI/FIfWinKaiser.c \
		lib/AFsp/libtsp/FI/FIfWinRCos.c \
		lib/AFsp/libtsp/FI/FIgdelFIR.c \
		lib/AFsp/libtsp/FI/FIgdelIIR.c \
		lib/AFsp/libtsp/FL/nucleus/FLdReadTF.c \
		lib/AFsp/libtsp/FL/nucleus/FLexist.c \
		lib/AFsp/libtsp/FL/nucleus/FLfileSize.c \
		lib/AFsp/libtsp/FL/nucleus/FLfReadTF.c \
		lib/AFsp/libtsp/FL/nucleus/FLjoinNames.c \
		lib/AFsp/libtsp/FL/nucleus/FLseekable.c \
		lib/AFsp/libtsp/FL/nucleus/FLterm.c \
		lib/AFsp/libtsp/FL/FLbackup.c \
		lib/AFsp/libtsp/FL/FLbaseName.c \
		lib/AFsp/libtsp/FL/FLdate.c \
		lib/AFsp/libtsp/FL/FLdefName.c \
		lib/AFsp/libtsp/FL/FLdirName.c \
		lib/AFsp/libtsp/FL/FLdReadData.c \
		lib/AFsp/libtsp/FL/FLdWriteData.c \
		lib/AFsp/libtsp/FL/FLexpHome.c \
		lib/AFsp/libtsp/FL/FLextName.c \
		lib/AFsp/libtsp/FL/FLfileDate.c \
		lib/AFsp/libtsp/FL/FLfReadData.c \
		lib/AFsp/libtsp/FL/FLfullName.c \
		lib/AFsp/libtsp/FL/FLfWriteData.c \
		lib/AFsp/libtsp/FL/FLgetLine.c \
		lib/AFsp/libtsp/FL/FLgetRec.c \
		lib/AFsp/libtsp/FL/FLhomeDir.c \
		lib/AFsp/libtsp/FL/FLpathList.c \
		lib/AFsp/libtsp/FL/FLpreName.c \
		lib/AFsp/libtsp/FL/FLprompt.c \
		lib/AFsp/libtsp/FL/FLreadLine.c \
		lib/AFsp/libtsp/FN/FNbessI0.c \
		lib/AFsp/libtsp/FN/FNevChebP.c \
		lib/AFsp/libtsp/FN/FNgcd.c \
		lib/AFsp/libtsp/FN/FNiLog2.c \
		lib/AFsp/libtsp/FN/FNsinc.c \
		lib/AFsp/libtsp/MA/nucleus/MAfChFactor.c \
		lib/AFsp/libtsp/MA/nucleus/MAfLTSolve.c \
		lib/AFsp/libtsp/MA/nucleus/MAfTTSolve.c \
		lib/AFsp/libtsp/MA/MAdAllocMat.c \
		lib/AFsp/libtsp/MA/MAdFreeMat.c \
		lib/AFsp/libtsp/MA/MAdPrint.c \
		lib/AFsp/libtsp/MA/MAfAllocMat.c \
		lib/AFsp/libtsp/MA/MAfChSolve.c \
		lib/AFsp/libtsp/MA/MAfFreeMat.c \
		lib/AFsp/libtsp/MA/MAfPrint.c \
		lib/AFsp/libtsp/MA/MAfSubMat.c \
		lib/AFsp/libtsp/MA/MAfSyBilin.c \
		lib/AFsp/libtsp/MA/MAfSyQuad.c \
		lib/AFsp/libtsp/MA/MAfTpQuad.c \
		lib/AFsp/libtsp/MA/MAfTpSolve.c \
		lib/AFsp/libtsp/MA/MAiPrint.c \
		lib/AFsp/libtsp/MA/MAlPrint.c \
		lib/AFsp/libtsp/MS/nucleus/MScoefMC.c \
		lib/AFsp/libtsp/MS/nucleus/MSdSlopeMC.c \
		lib/AFsp/libtsp/MS/nucleus/MSevalMC.c \
		lib/AFsp/libtsp/MS/nucleus/MSfSlopeMC.c \
		lib/AFsp/libtsp/MS/MSdConvCof.c \
		lib/AFsp/libtsp/MS/MSdDeconvCof.c \
		lib/AFsp/libtsp/MS/MSdIntLin.c \
		lib/AFsp/libtsp/MS/MSdIntMC.c \
		lib/AFsp/libtsp/MS/MSdLocate.c \
		lib/AFsp/libtsp/MS/MSdNint.c \
		lib/AFsp/libtsp/MS/MSdPolyInt.c \
		lib/AFsp/libtsp/MS/MSfConvCof.c \
		lib/AFsp/libtsp/MS/MSfGaussRand.c \
		lib/AFsp/libtsp/MS/MSfIntLin.c \
		lib/AFsp/libtsp/MS/MSfIntMC.c \
		lib/AFsp/libtsp/MS/MSfUnifRand.c \
		lib/AFsp/libtsp/MS/MSiCeil.c \
		lib/AFsp/libtsp/MS/MSiFloor.c \
		lib/AFsp/libtsp/MS/MSiPower.c \
		lib/AFsp/libtsp/MS/MSlCeil.c \
		lib/AFsp/libtsp/MS/MSlFloor.c \
		lib/AFsp/libtsp/MS/MSratio.c \
		lib/AFsp/libtsp/SP/nucleus/SPdPreFFT.c \
		lib/AFsp/libtsp/SP/nucleus/SPdTrMat2.c \
		lib/AFsp/libtsp/SP/nucleus/SPfPreFFT.c \
		lib/AFsp/libtsp/SP/nucleus/SPfTrMat2.c \
		lib/AFsp/libtsp/SP/SPautoc.c \
		lib/AFsp/libtsp/SP/SPcepXpc.c \
		lib/AFsp/libtsp/SP/SPcorBWexp.c \
		lib/AFsp/libtsp/SP/SPcorFilt.c \
		lib/AFsp/libtsp/SP/SPcorFmse.c \
		lib/AFsp/libtsp/SP/SPcorPmse.c \
		lib/AFsp/libtsp/SP/SPcorXpc.c \
		lib/AFsp/libtsp/SP/SPcovar.c \
		lib/AFsp/libtsp/SP/SPcovCXpc.c \
		lib/AFsp/libtsp/SP/SPcovFilt.c \
		lib/AFsp/libtsp/SP/SPcovFmse.c \
		lib/AFsp/libtsp/SP/SPcovLXpc.c \
		lib/AFsp/libtsp/SP/SPcovMXpc.c \
		lib/AFsp/libtsp/SP/SPcovPmse.c \
		lib/AFsp/libtsp/SP/SPcovXpc.c \
		lib/AFsp/libtsp/SP/SPdCFFT.c \
		lib/AFsp/libtsp/SP/SPdQuantL.c \
		lib/AFsp/libtsp/SP/SPdRFFT.c \
		lib/AFsp/libtsp/SP/SPecXpc.c \
		lib/AFsp/libtsp/SP/SPfCFFT.c \
		lib/AFsp/libtsp/SP/SPfDCT.c \
		lib/AFsp/libtsp/SP/SPfQuantL.c \
		lib/AFsp/libtsp/SP/SPfQuantU.c \
		lib/AFsp/libtsp/SP/SPfRFFT.c \
		lib/AFsp/libtsp/SP/SPlpcISdist.c \
		lib/AFsp/libtsp/SP/SPlpcLSdist.c \
		lib/AFsp/libtsp/SP/SPlsfXpc.c \
		lib/AFsp/libtsp/SP/SPpcBWexp.c \
		lib/AFsp/libtsp/SP/SPpcXcep.c \
		lib/AFsp/libtsp/SP/SPpcXcor.c \
		lib/AFsp/libtsp/SP/SPpcXec.c \
		lib/AFsp/libtsp/SP/SPpcXlsf.c \
		lib/AFsp/libtsp/SP/SPpcXrc.c \
		lib/AFsp/libtsp/SP/SPrcXpc.c \
		lib/AFsp/libtsp/ST/nucleus/STdec1val.c \
		lib/AFsp/libtsp/ST/nucleus/STdecNval.c \
		lib/AFsp/libtsp/ST/nucleus/STdecPair.c \
		lib/AFsp/libtsp/ST/nucleus/STstrDots.c \
		lib/AFsp/libtsp/ST/nucleus/STstrstrNM.c \
		lib/AFsp/libtsp/ST/nucleus/STtrimIws.c \
		lib/AFsp/libtsp/ST/nucleus/STtrimNMax.c \
		lib/AFsp/libtsp/ST/STcatMax.c \
		lib/AFsp/libtsp/ST/STcopyMax.c \
		lib/AFsp/libtsp/ST/STcopyNMax.c \
		lib/AFsp/libtsp/ST/STdec1double.c \
		lib/AFsp/libtsp/ST/STdec1float.c \
		lib/AFsp/libtsp/ST/STdec1int.c \
		lib/AFsp/libtsp/ST/STdec1long.c \
		lib/AFsp/libtsp/ST/STdecDfrac.c \
		lib/AFsp/libtsp/ST/STdecIfrac.c \
		lib/AFsp/libtsp/ST/STdecIrange.c \
		lib/AFsp/libtsp/ST/STdecLrange.c \
		lib/AFsp/libtsp/ST/STdecNdouble.c \
		lib/AFsp/libtsp/ST/STdecNfloat.c \
		lib/AFsp/libtsp/ST/STdecNint.c \
		lib/AFsp/libtsp/ST/STdecNlong.c \
		lib/AFsp/libtsp/ST/STfindToken.c \
		lib/AFsp/libtsp/ST/STkeyMatch.c \
		lib/AFsp/libtsp/ST/STkeyXpar.c \
		lib/AFsp/libtsp/ST/STstrLC.c \
		lib/AFsp/libtsp/ST/STtrim.c \
		lib/AFsp/libtsp/ST/STunQuote.c \
		lib/AFsp/libtsp/UT/nucleus/UTbyteCode.c \
		lib/AFsp/libtsp/UT/nucleus/UTbyteOrder.c \
		lib/AFsp/libtsp/UT/nucleus/UTcheckIEEE.c \
		lib/AFsp/libtsp/UT/nucleus/UTctime.c \
		lib/AFsp/libtsp/UT/nucleus/UTdecOption.c \
		lib/AFsp/libtsp/UT/nucleus/UTdIEEE80.c \
		lib/AFsp/libtsp/UT/nucleus/UTeIEEE80.c \
		lib/AFsp/libtsp/UT/nucleus/UTgetHost.c \
		lib/AFsp/libtsp/UT/nucleus/UTgetUser.c \
		lib/AFsp/libtsp/UT/nucleus/UTswapCode.c \
		lib/AFsp/libtsp/UT/UTdate.c \
		lib/AFsp/libtsp/UT/UTerror.c \
		lib/AFsp/libtsp/UT/UTfree.c \
		lib/AFsp/libtsp/UT/UTgetOption.c \
		lib/AFsp/libtsp/UT/UThalt.c \
		lib/AFsp/libtsp/UT/UTmalloc.c \
		lib/AFsp/libtsp/UT/UTrealloc.c \
		lib/AFsp/libtsp/UT/UTsetProg.c \
		lib/AFsp/libtsp/UT/UTsysMsg.c \
		lib/AFsp/libtsp/UT/UTuserName.c \
		lib/AFsp/libtsp/UT/UTwarn.c \
		lib/AFsp/libtsp/VR/nucleus/VRswapBytes.c \
		lib/AFsp/libtsp/VR/VRdAdd.c \
		lib/AFsp/libtsp/VR/VRdCopy.c \
		lib/AFsp/libtsp/VR/VRdCorSym.c \
		lib/AFsp/libtsp/VR/VRdDotProd.c \
		lib/AFsp/libtsp/VR/VRdLinInc.c \
		lib/AFsp/libtsp/VR/VRdMult.c \
		lib/AFsp/libtsp/VR/VRdPrint.c \
		lib/AFsp/libtsp/VR/VRdRFFTMSq.c \
		lib/AFsp/libtsp/VR/VRdScale.c \
		lib/AFsp/libtsp/VR/VRdSet.c \
		lib/AFsp/libtsp/VR/VRdShift.c \
		lib/AFsp/libtsp/VR/VRdSymPart.c \
		lib/AFsp/libtsp/VR/VRdZero.c \
		lib/AFsp/libtsp/VR/VRfAdd.c \
		lib/AFsp/libtsp/VR/VRfAmpldB.c \
		lib/AFsp/libtsp/VR/VRfCopy.c \
		lib/AFsp/libtsp/VR/VRfCorSym.c \
		lib/AFsp/libtsp/VR/VRfDiff.c \
		lib/AFsp/libtsp/VR/VRfDiffSq.c \
		lib/AFsp/libtsp/VR/VRfDotProd.c \
		lib/AFsp/libtsp/VR/VRfLog10.c \
		lib/AFsp/libtsp/VR/VRfMax.c \
		lib/AFsp/libtsp/VR/VRfMin.c \
		lib/AFsp/libtsp/VR/VRfMult.c \
		lib/AFsp/libtsp/VR/VRfPow10.c \
		lib/AFsp/libtsp/VR/VRfPrint.c \
		lib/AFsp/libtsp/VR/VRfRev.c \
		lib/AFsp/libtsp/VR/VRfScale.c \
		lib/AFsp/libtsp/VR/VRfSet.c \
		lib/AFsp/libtsp/VR/VRfShift.c \
		lib/AFsp/libtsp/VR/VRfSum.c \
		lib/AFsp/libtsp/VR/VRfZero.c \
		lib/AFsp/libtsp/VR/VRiPrint.c \
		lib/AFsp/libtsp/VR/VRiZero.c \
		lib/AFsp/libtsp/VR/VRlPrint.c \
		lib/SAC/sac_dec/src/sac_arbitraryTree.c \
		lib/SAC/sac_dec/src/sac_bitdec.c \
		lib/SAC/sac_dec/src/sac_bitinput.c \
		lib/SAC/sac_dec/src/sac_blind.c \
		lib/SAC/sac_dec/src/sac_calcM1andM2.c \
		lib/SAC/sac_dec/src/sac_config.c \
		lib/SAC/sac_dec/src/sac_dec.c \
		lib/SAC/sac_dec/src/sac_decor.c \
		lib/SAC/sac_dec/src/sac_hrtf.c \
		lib/SAC/sac_dec/src/sac_hrtffilter.c \
		lib/SAC/sac_dec/src/sac_dec_hybfilter.c \
		lib/SAC/sac_dec/src/sac_mdct2qmf.c \
		lib/SAC/sac_dec/src/sac_mdct2qmf_wf_tables.c \
		lib/SAC/sac_dec/src/sac_nlc_dec.c \
		lib/SAC/sac_dec/src/sac_nodes_cld.c \
		lib/SAC/sac_dec/src/sac_nodes_cpc.c \
		lib/SAC/sac_dec/src/sac_nodes_icc.c \
		lib/SAC/sac_dec/src/sac_nodes_pt0.c \
		lib/SAC/sac_dec/src/sac_nodes_reshape.c \
		lib/SAC/sac_dec/src/sac_parallelReverb.c \
		lib/SAC/sac_dec/src/sac_partiallyComplex.c \
		lib/SAC/sac_dec/src/sac_dec_polyphase.c \
		lib/SAC/sac_dec/src/sac_process.c \
		lib/SAC/sac_dec/src/sac_resdecode.c \
		lib/SAC/sac_dec/src/sac_reshapeBBEnv.c \
		lib/SAC/sac_dec/src/sac_reshuffdec1.c \
		lib/SAC/sac_dec/src/sac_reshuffdec2.c \
		lib/SAC/sac_dec/src/sac_reshuffdec3.c \
		lib/SAC/sac_dec/src/sac_reshuffinit.c \
		lib/SAC/sac_dec/src/sac_reshufftables.c \
		lib/SAC/sac_dec/src/sac_resintrins.c \
		lib/SAC/sac_dec/src/sac_restns.c \
		lib/SAC/sac_dec/src/sac_smoothing.c \
		lib/SAC/sac_dec/src/sac_tonality.c \
		lib/SAC/sac_dec/src/sac_TPprocess.c \
		lib/SAC/sac_dec/src/spatial_bdreader.c \
		lib/SAC/sac_dec/src/spatial_filereader.c \
		lib/SAC/sac_dec/src/spatial_hrtfreader.c 

OBJECTS_LIBS	= \
		$(DIR)AFdRdAlaw.o \
		$(DIR)AFdRdFx.o \
		$(DIR)AFdRdIx.o \
		$(DIR)AFdRdMulaw.o \
		$(DIR)AFdRdTA.o \
		$(DIR)AFdRdU1.o \
		$(DIR)AFdWrAlaw.o \
		$(DIR)AFdWrFx.o \
		$(DIR)AFdWrIx.o \
		$(DIR)AFdWrMulaw.o \
		$(DIR)AFdWrTA.o \
		$(DIR)AFdWrU1.o \
		$(DIR)AFfRdAlaw.o \
		$(DIR)AFfRdFx.o \
		$(DIR)AFfRdIx.o \
		$(DIR)AFfRdMulaw.o \
		$(DIR)AFfRdTA.o \
		$(DIR)AFfRdU1.o \
		$(DIR)AFfWrAlaw.o \
		$(DIR)AFfWrFx.o \
		$(DIR)AFfWrIx.o \
		$(DIR)AFfWrMulaw.o \
		$(DIR)AFfWrTA.o \
		$(DIR)AFfWrU1.o \
		$(DIR)AFgetLine.o \
		$(DIR)AFposition.o \
		$(DIR)AFseek.o \
		$(DIR)AFtell.o \
		$(DIR)AFdecSpeaker.o \
		$(DIR)AFfindType.o \
		$(DIR)AFgetInfoRec.o \
		$(DIR)AFoptions.o \
		$(DIR)AFprAFpar.o \
		$(DIR)AFpreSetWPar.o \
		$(DIR)AFrdAIhead.o \
		$(DIR)AFrdAUhead.o \
		$(DIR)AFrdBLhead.o \
		$(DIR)AFrdEShead.o \
		$(DIR)AFrdINhead.o \
		$(DIR)AFrdNShead.o \
		$(DIR)AFrdSFhead.o \
		$(DIR)AFrdSPhead.o \
		$(DIR)AFrdSWhead.o \
		$(DIR)AFrdTAhead.o \
		$(DIR)AFrdTextAFsp.o \
		$(DIR)AFrdWVhead.o \
		$(DIR)AFreadHead.o \
		$(DIR)AFsetNHread.o \
		$(DIR)AFsetNHwrite.o \
		$(DIR)AFsetRead.o \
		$(DIR)AFsetWrite.o \
		$(DIR)AFupdAIhead.o \
		$(DIR)AFupdAUhead.o \
		$(DIR)AFupdHead.o \
		$(DIR)AFupdWVhead.o \
		$(DIR)AFwrAIhead.o \
		$(DIR)AFwrAUhead.o \
		$(DIR)AFwriteHead.o \
		$(DIR)AFwrWVhead.o \
		$(DIR)AFclose.o \
		$(DIR)AFdReadData.o \
		$(DIR)AFdWriteData.o \
		$(DIR)AFfReadData.o \
		$(DIR)AFfWriteData.o \
		$(DIR)AFopenRead.o \
		$(DIR)AFopenWrite.o \
		$(DIR)AFopnRead.o \
		$(DIR)AFopnWrite.o \
		$(DIR)AFreadData.o \
		$(DIR)AFsetFileType.o \
		$(DIR)AFsetInfo.o \
		$(DIR)AFsetNHpar.o \
		$(DIR)AFsetSpeaker.o \
		$(DIR)AFwriteData.o \
		$(DIR)FIrCosF.o \
		$(DIR)FIxKaiser.o \
		$(DIR)FIdBiquad.o \
		$(DIR)FIdConvol.o \
		$(DIR)FIdConvSI.o \
		$(DIR)FIdFiltAP.o \
		$(DIR)FIdFiltIIR.o \
		$(DIR)FIdKaiserLPF.o \
		$(DIR)FIdReadFilt.o \
		$(DIR)FIdWinHamm.o \
		$(DIR)FIdWinKaiser.o \
		$(DIR)FIfBiquad.o \
		$(DIR)FIfConvol.o \
		$(DIR)FIfConvSI.o \
		$(DIR)FIfDeem.o \
		$(DIR)FIfFiltAP.o \
		$(DIR)FIfFiltIIR.o \
		$(DIR)FIfKaiserLPF.o \
		$(DIR)FIfPreem.o \
		$(DIR)FIfReadFilt.o \
		$(DIR)FIfWinHamm.o \
		$(DIR)FIfWinHCos.o \
		$(DIR)FIfWinKaiser.o \
		$(DIR)FIfWinRCos.o \
		$(DIR)FIgdelFIR.o \
		$(DIR)FIgdelIIR.o \
		$(DIR)FLdReadTF.o \
		$(DIR)FLexist.o \
		$(DIR)FLfileSize.o \
		$(DIR)FLfReadTF.o \
		$(DIR)FLjoinNames.o \
		$(DIR)FLseekable.o \
		$(DIR)FLterm.o \
		$(DIR)FLbackup.o \
		$(DIR)FLbaseName.o \
		$(DIR)FLdate.o \
		$(DIR)FLdefName.o \
		$(DIR)FLdirName.o \
		$(DIR)FLdReadData.o \
		$(DIR)FLdWriteData.o \
		$(DIR)FLexpHome.o \
		$(DIR)FLextName.o \
		$(DIR)FLfileDate.o \
		$(DIR)FLfReadData.o \
		$(DIR)FLfullName.o \
		$(DIR)FLfWriteData.o \
		$(DIR)FLgetLine.o \
		$(DIR)FLgetRec.o \
		$(DIR)FLhomeDir.o \
		$(DIR)FLpathList.o \
		$(DIR)FLpreName.o \
		$(DIR)FLprompt.o \
		$(DIR)FLreadLine.o \
		$(DIR)FNbessI0.o \
		$(DIR)FNevChebP.o \
		$(DIR)FNgcd.o \
		$(DIR)FNiLog2.o \
		$(DIR)FNsinc.o \
		$(DIR)MAfChFactor.o \
		$(DIR)MAfLTSolve.o \
		$(DIR)MAfTTSolve.o \
		$(DIR)MAdAllocMat.o \
		$(DIR)MAdFreeMat.o \
		$(DIR)MAdPrint.o \
		$(DIR)MAfAllocMat.o \
		$(DIR)MAfChSolve.o \
		$(DIR)MAfFreeMat.o \
		$(DIR)MAfPrint.o \
		$(DIR)MAfSubMat.o \
		$(DIR)MAfSyBilin.o \
		$(DIR)MAfSyQuad.o \
		$(DIR)MAfTpQuad.o \
		$(DIR)MAfTpSolve.o \
		$(DIR)MAiPrint.o \
		$(DIR)MAlPrint.o \
		$(DIR)MScoefMC.o \
		$(DIR)MSdSlopeMC.o \
		$(DIR)MSevalMC.o \
		$(DIR)MSfSlopeMC.o \
		$(DIR)MSdConvCof.o \
		$(DIR)MSdDeconvCof.o \
		$(DIR)MSdIntLin.o \
		$(DIR)MSdIntMC.o \
		$(DIR)MSdLocate.o \
		$(DIR)MSdNint.o \
		$(DIR)MSdPolyInt.o \
		$(DIR)MSfConvCof.o \
		$(DIR)MSfGaussRand.o \
		$(DIR)MSfIntLin.o \
		$(DIR)MSfIntMC.o \
		$(DIR)MSfUnifRand.o \
		$(DIR)MSiCeil.o \
		$(DIR)MSiFloor.o \
		$(DIR)MSiPower.o \
		$(DIR)MSlCeil.o \
		$(DIR)MSlFloor.o \
		$(DIR)MSratio.o \
		$(DIR)SPdPreFFT.o \
		$(DIR)SPdTrMat2.o \
		$(DIR)SPfPreFFT.o \
		$(DIR)SPfTrMat2.o \
		$(DIR)SPautoc.o \
		$(DIR)SPcepXpc.o \
		$(DIR)SPcorBWexp.o \
		$(DIR)SPcorFilt.o \
		$(DIR)SPcorFmse.o \
		$(DIR)SPcorPmse.o \
		$(DIR)SPcorXpc.o \
		$(DIR)SPcovar.o \
		$(DIR)SPcovCXpc.o \
		$(DIR)SPcovFilt.o \
		$(DIR)SPcovFmse.o \
		$(DIR)SPcovLXpc.o \
		$(DIR)SPcovMXpc.o \
		$(DIR)SPcovPmse.o \
		$(DIR)SPcovXpc.o \
		$(DIR)SPdCFFT.o \
		$(DIR)SPdQuantL.o \
		$(DIR)SPdRFFT.o \
		$(DIR)SPecXpc.o \
		$(DIR)SPfCFFT.o \
		$(DIR)SPfDCT.o \
		$(DIR)SPfQuantL.o \
		$(DIR)SPfQuantU.o \
		$(DIR)SPfRFFT.o \
		$(DIR)SPlpcISdist.o \
		$(DIR)SPlpcLSdist.o \
		$(DIR)SPlsfXpc.o \
		$(DIR)SPpcBWexp.o \
		$(DIR)SPpcXcep.o \
		$(DIR)SPpcXcor.o \
		$(DIR)SPpcXec.o \
		$(DIR)SPpcXlsf.o \
		$(DIR)SPpcXrc.o \
		$(DIR)SPrcXpc.o \
		$(DIR)STdec1val.o \
		$(DIR)STdecNval.o \
		$(DIR)STdecPair.o \
		$(DIR)STstrDots.o \
		$(DIR)STstrstrNM.o \
		$(DIR)STtrimIws.o \
		$(DIR)STtrimNMax.o \
		$(DIR)STcatMax.o \
		$(DIR)STcopyMax.o \
		$(DIR)STcopyNMax.o \
		$(DIR)STdec1double.o \
		$(DIR)STdec1float.o \
		$(DIR)STdec1int.o \
		$(DIR)STdec1long.o \
		$(DIR)STdecDfrac.o \
		$(DIR)STdecIfrac.o \
		$(DIR)STdecIrange.o \
		$(DIR)STdecLrange.o \
		$(DIR)STdecNdouble.o \
		$(DIR)STdecNfloat.o \
		$(DIR)STdecNint.o \
		$(DIR)STdecNlong.o \
		$(DIR)STfindToken.o \
		$(DIR)STkeyMatch.o \
		$(DIR)STkeyXpar.o \
		$(DIR)STstrLC.o \
		$(DIR)STtrim.o \
		$(DIR)STunQuote.o \
		$(DIR)UTbyteCode.o \
		$(DIR)UTbyteOrder.o \
		$(DIR)UTcheckIEEE.o \
		$(DIR)UTctime.o \
		$(DIR)UTdecOption.o \
		$(DIR)UTdIEEE80.o \
		$(DIR)UTeIEEE80.o \
		$(DIR)UTgetHost.o \
		$(DIR)UTgetUser.o \
		$(DIR)UTswapCode.o \
		$(DIR)UTdate.o \
		$(DIR)UTerror.o \
		$(DIR)UTfree.o \
		$(DIR)UTgetOption.o \
		$(DIR)UThalt.o \
		$(DIR)UTmalloc.o \
		$(DIR)UTrealloc.o \
		$(DIR)UTsetProg.o \
		$(DIR)UTsysMsg.o \
		$(DIR)UTuserName.o \
		$(DIR)UTwarn.o \
		$(DIR)VRswapBytes.o \
		$(DIR)VRdAdd.o \
		$(DIR)VRdCopy.o \
		$(DIR)VRdCorSym.o \
		$(DIR)VRdDotProd.o \
		$(DIR)VRdLinInc.o \
		$(DIR)VRdMult.o \
		$(DIR)VRdPrint.o \
		$(DIR)VRdRFFTMSq.o \
		$(DIR)VRdScale.o \
		$(DIR)VRdSet.o \
		$(DIR)VRdShift.o \
		$(DIR)VRdSymPart.o \
		$(DIR)VRdZero.o \
		$(DIR)VRfAdd.o \
		$(DIR)VRfAmpldB.o \
		$(DIR)VRfCopy.o \
		$(DIR)VRfCorSym.o \
		$(DIR)VRfDiff.o \
		$(DIR)VRfDiffSq.o \
		$(DIR)VRfDotProd.o \
		$(DIR)VRfLog10.o \
		$(DIR)VRfMax.o \
		$(DIR)VRfMin.o \
		$(DIR)VRfMult.o \
		$(DIR)VRfPow10.o \
		$(DIR)VRfPrint.o \
		$(DIR)VRfRev.o \
		$(DIR)VRfScale.o \
		$(DIR)VRfSet.o \
		$(DIR)VRfShift.o \
		$(DIR)VRfSum.o \
		$(DIR)VRfZero.o \
		$(DIR)VRiPrint.o \
		$(DIR)VRiZero.o \
		$(DIR)VRlPrint.o \
		$(DIR)sac_arbitraryTree.o \
		$(DIR)sac_bitdec.o \
		$(DIR)sac_bitinput.o \
		$(DIR)sac_blind.o \
		$(DIR)sac_calcM1andM2.o \
		$(DIR)sac_config.o \
		$(DIR)sac_dec.o \
		$(DIR)sac_decor.o \
		$(DIR)sac_hrtf.o \
		$(DIR)sac_hrtffilter.o \
		$(DIR)sac_dec_hybfilter.o \
		$(DIR)sac_mdct2qmf.o \
		$(DIR)sac_mdct2qmf_wf_tables.o \
		$(DIR)sac_nlc_dec.o \
		$(DIR)sac_nodes_cld.o \
		$(DIR)sac_nodes_cpc.o \
		$(DIR)sac_nodes_icc.o \
		$(DIR)sac_nodes_pt0.o \
		$(DIR)sac_nodes_reshape.o \
		$(DIR)sac_parallelReverb.o \
		$(DIR)sac_partiallyComplex.o \
		$(DIR)sac_dec_polyphase.o \
		$(DIR)sac_process.o \
		$(DIR)sac_resdecode.o \
		$(DIR)sac_reshapeBBEnv.o \
		$(DIR)sac_reshuffdec1.o \
		$(DIR)sac_reshuffdec2.o \
		$(DIR)sac_reshuffdec3.o \
		$(DIR)sac_reshuffinit.o \
		$(DIR)sac_reshufftables.o \
		$(DIR)sac_resintrins.o \
		$(DIR)sac_restns.o \
		$(DIR)sac_smoothing.o \
		$(DIR)sac_tonality.o \
		$(DIR)sac_TPprocess.o \
		$(DIR)spatial_bdreader.o \
		$(DIR)spatial_filereader.o \
		$(DIR)spatial_hrtfreader.o

# Custom Variables

EXPORT_VALID_ARCHS = x86_64
EXPORT_ARCH_ARGS = $(foreach arch, $(if $(EXPORT_ACTIVE_ARCHS), $(EXPORT_ACTIVE_ARCHS), $(EXPORT_VALID_ARCHS)), -arch $(arch))
EXPORT_ACTIVE_ARCHS = $(filter $(EXPORT_VALID_ARCHS), $(ARCHS))

# Build rules

$(TARGET): folder /usr/local/lib/libfftw3.a lib/AFsp/lib/libtsp.a lib/SAC/sac_dec/lib/libsac_dec.a $(OBJECTS) $(OBJECTS_EFFECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(OBJECTS_EFFECTS) $(LIBS)

force: folder /usr/local/lib/libfftw3.a $(OBJECTS) $(OBJECTS_EFFECTS) $(OBJECTS_LIBS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)$(OBJECTS_EFFECTS) $(OBJECTS_LIBS) $(LIBS)

clean:
	-$(DEL_FILE) moc_predefs.h
	-$(DEL_FILE) $(OBJECTS) $(OBJECTS_EFFECTS) $(OBJECTS_LIBS)
	-$(DEL_FILE) *~ core *.core

folder:
	@mkdir -p $(DIR)
	
# Install

configure: configure_src/main.o
	$(CXX) $(CXXFLAGS) -o configure_OS configure_src/main.o
	
clean_configure:
	-$(DEL_FILE) configure_src/main.o
	-$(DEL_FILE) *~ core *.core
	
configure_src/main.o: configure_src/main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o configure_src/main.o configure_src/main.cpp

install:

uninstall:

# Compile

$(DIR)main.o: src/main.cpp src/process/ProcessManager.h \
		src/process/File.h \
		src/sac/SACBitstream.h \
		src/sac/sac_decoder.h \
		src/effects/Effect.h \
		src/effects/EffectBase.h \
		$(HEADERS_EFFECTS) \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)main.o src/main.cpp

$(DIR)ProcessManager.o: src/process/ProcessManager.cpp src/process/ProcessManager.h \
		src/process/File.h \
		src/sac/SACBitstream.h \
		src/sac/sac_decoder.h \
		src/effects/Effect.h \
		src/effects/EffectBase.h \
		$(HEADERS_EFFECTS) \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)ProcessManager.o src/process/ProcessManager.cpp

$(DIR)File.o: src/process/File.cpp src/process/File.h \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)File.o src/process/File.cpp

$(DIR)AudioSignal.o: src/process/AudioSignal.cpp src/process/AudioSignal.h \
		/usr/local/include/fftw3.h \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)AudioSignal.o src/process/AudioSignal.cpp

$(DIR)sac_decoder.o: src/sac/sac_decoder.c src/sac/sac_decoder.h \
		lib/SAC/sac_dec/include/spatial_frontend.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/SAC/sac_dec/include/spatial_filereader.h \
		lib/SAC/sac_dec/include/spatial_bdreader.h \
		lib/SAC/sac_dec/include/spatial_hrtfreader.h \
		lib/SAC/sac_dec/include/sac_parallelReverb.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_decoder.o src/sac/sac_decoder.c

$(DIR)SACBitstream.o: src/sac/SACBitstream.cpp src/sac/SACBitstream.h \
		src/process/File.h \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)SACBitstream.o src/sac/SACBitstream.cpp

$(DIR)Effect.o: src/effects/Effect.cpp src/effects/Effect.h \
		src/process/File.h \
		src/sac/SACBitstream.h \
		src/effects/EffectBase.h \
		$(HEADERS_EFFECTS) \
		src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Effect.o src/effects/Effect.cpp

$(DIR)Logger.o: src/tools/Logger.cpp src/tools/Logger.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Logger.o src/tools/Logger.cpp

# Compile libraries

$(DIR)AFdRdAlaw.o: lib/AFsp/libtsp/AF/dataio/AFdRdAlaw.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdAlaw.o lib/AFsp/libtsp/AF/dataio/AFdRdAlaw.c

$(DIR)AFdRdFx.o: lib/AFsp/libtsp/AF/dataio/AFdRdFx.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdFx.o lib/AFsp/libtsp/AF/dataio/AFdRdFx.c

$(DIR)AFdRdIx.o: lib/AFsp/libtsp/AF/dataio/AFdRdIx.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdIx.o lib/AFsp/libtsp/AF/dataio/AFdRdIx.c

$(DIR)AFdRdMulaw.o: lib/AFsp/libtsp/AF/dataio/AFdRdMulaw.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdMulaw.o lib/AFsp/libtsp/AF/dataio/AFdRdMulaw.c

$(DIR)AFdRdTA.o: lib/AFsp/libtsp/AF/dataio/AFdRdTA.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdTA.o lib/AFsp/libtsp/AF/dataio/AFdRdTA.c

$(DIR)AFdRdU1.o: lib/AFsp/libtsp/AF/dataio/AFdRdU1.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdRdU1.o lib/AFsp/libtsp/AF/dataio/AFdRdU1.c

$(DIR)AFdWrAlaw.o: lib/AFsp/libtsp/AF/dataio/AFdWrAlaw.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrAlaw.o lib/AFsp/libtsp/AF/dataio/AFdWrAlaw.c

$(DIR)AFdWrFx.o: lib/AFsp/libtsp/AF/dataio/AFdWrFx.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrFx.o lib/AFsp/libtsp/AF/dataio/AFdWrFx.c

$(DIR)AFdWrIx.o: lib/AFsp/libtsp/AF/dataio/AFdWrIx.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrIx.o lib/AFsp/libtsp/AF/dataio/AFdWrIx.c

$(DIR)AFdWrMulaw.o: lib/AFsp/libtsp/AF/dataio/AFdWrMulaw.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrMulaw.o lib/AFsp/libtsp/AF/dataio/AFdWrMulaw.c

$(DIR)AFdWrTA.o: lib/AFsp/libtsp/AF/dataio/AFdWrTA.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrTA.o lib/AFsp/libtsp/AF/dataio/AFdWrTA.c

$(DIR)AFdWrU1.o: lib/AFsp/libtsp/AF/dataio/AFdWrU1.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWrU1.o lib/AFsp/libtsp/AF/dataio/AFdWrU1.c

$(DIR)AFfRdAlaw.o: lib/AFsp/libtsp/AF/dataio/AFfRdAlaw.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdAlaw.o lib/AFsp/libtsp/AF/dataio/AFfRdAlaw.c

$(DIR)AFfRdFx.o: lib/AFsp/libtsp/AF/dataio/AFfRdFx.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdFx.o lib/AFsp/libtsp/AF/dataio/AFfRdFx.c

$(DIR)AFfRdIx.o: lib/AFsp/libtsp/AF/dataio/AFfRdIx.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdIx.o lib/AFsp/libtsp/AF/dataio/AFfRdIx.c

$(DIR)AFfRdMulaw.o: lib/AFsp/libtsp/AF/dataio/AFfRdMulaw.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdMulaw.o lib/AFsp/libtsp/AF/dataio/AFfRdMulaw.c

$(DIR)AFfRdTA.o: lib/AFsp/libtsp/AF/dataio/AFfRdTA.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdTA.o lib/AFsp/libtsp/AF/dataio/AFfRdTA.c

$(DIR)AFfRdU1.o: lib/AFsp/libtsp/AF/dataio/AFfRdU1.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfRdU1.o lib/AFsp/libtsp/AF/dataio/AFfRdU1.c

$(DIR)AFfWrAlaw.o: lib/AFsp/libtsp/AF/dataio/AFfWrAlaw.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrAlaw.o lib/AFsp/libtsp/AF/dataio/AFfWrAlaw.c

$(DIR)AFfWrFx.o: lib/AFsp/libtsp/AF/dataio/AFfWrFx.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrFx.o lib/AFsp/libtsp/AF/dataio/AFfWrFx.c

$(DIR)AFfWrIx.o: lib/AFsp/libtsp/AF/dataio/AFfWrIx.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrIx.o lib/AFsp/libtsp/AF/dataio/AFfWrIx.c

$(DIR)AFfWrMulaw.o: lib/AFsp/libtsp/AF/dataio/AFfWrMulaw.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrMulaw.o lib/AFsp/libtsp/AF/dataio/AFfWrMulaw.c

$(DIR)AFfWrTA.o: lib/AFsp/libtsp/AF/dataio/AFfWrTA.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrTA.o lib/AFsp/libtsp/AF/dataio/AFfWrTA.c

$(DIR)AFfWrU1.o: lib/AFsp/libtsp/AF/dataio/AFfWrU1.c lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWrU1.o lib/AFsp/libtsp/AF/dataio/AFfWrU1.c

$(DIR)AFgetLine.o: lib/AFsp/libtsp/AF/dataio/AFgetLine.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFgetLine.o lib/AFsp/libtsp/AF/dataio/AFgetLine.c

$(DIR)AFposition.o: lib/AFsp/libtsp/AF/dataio/AFposition.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFposition.o lib/AFsp/libtsp/AF/dataio/AFposition.c

$(DIR)AFseek.o: lib/AFsp/libtsp/AF/dataio/AFseek.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFseek.o lib/AFsp/libtsp/AF/dataio/AFseek.c

$(DIR)AFtell.o: lib/AFsp/libtsp/AF/dataio/AFtell.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFtell.o lib/AFsp/libtsp/AF/dataio/AFtell.c

$(DIR)AFdecSpeaker.o: lib/AFsp/libtsp/AF/header/AFdecSpeaker.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdecSpeaker.o lib/AFsp/libtsp/AF/header/AFdecSpeaker.c

$(DIR)AFfindType.o: lib/AFsp/libtsp/AF/header/AFfindType.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfindType.o lib/AFsp/libtsp/AF/header/AFfindType.c

$(DIR)AFgetInfoRec.o: lib/AFsp/libtsp/AF/header/AFgetInfoRec.c lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFgetInfoRec.o lib/AFsp/libtsp/AF/header/AFgetInfoRec.c

$(DIR)AFoptions.o: lib/AFsp/libtsp/AF/header/AFoptions.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFoptions.o lib/AFsp/libtsp/AF/header/AFoptions.c

$(DIR)AFprAFpar.o: lib/AFsp/libtsp/AF/header/AFprAFpar.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFprAFpar.o lib/AFsp/libtsp/AF/header/AFprAFpar.c

$(DIR)AFpreSetWPar.o: lib/AFsp/libtsp/AF/header/AFpreSetWPar.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFpreSetWPar.o lib/AFsp/libtsp/AF/header/AFpreSetWPar.c

$(DIR)AFrdAIhead.o: lib/AFsp/libtsp/AF/header/AFrdAIhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AIpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdAIhead.o lib/AFsp/libtsp/AF/header/AFrdAIhead.c

$(DIR)AFrdAUhead.o: lib/AFsp/libtsp/AF/header/AFrdAUhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AUpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdAUhead.o lib/AFsp/libtsp/AF/header/AFrdAUhead.c

$(DIR)AFrdBLhead.o: lib/AFsp/libtsp/AF/header/AFrdBLhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdBLhead.o lib/AFsp/libtsp/AF/header/AFrdBLhead.c

$(DIR)AFrdEShead.o: lib/AFsp/libtsp/AF/header/AFrdEShead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/ESpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdEShead.o lib/AFsp/libtsp/AF/header/AFrdEShead.c

$(DIR)AFrdINhead.o: lib/AFsp/libtsp/AF/header/AFrdINhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdINhead.o lib/AFsp/libtsp/AF/header/AFrdINhead.c

$(DIR)AFrdNShead.o: lib/AFsp/libtsp/AF/header/AFrdNShead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdNShead.o lib/AFsp/libtsp/AF/header/AFrdNShead.c

$(DIR)AFrdSFhead.o: lib/AFsp/libtsp/AF/header/AFrdSFhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdSFhead.o lib/AFsp/libtsp/AF/header/AFrdSFhead.c

$(DIR)AFrdSPhead.o: lib/AFsp/libtsp/AF/header/AFrdSPhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdSPhead.o lib/AFsp/libtsp/AF/header/AFrdSPhead.c

$(DIR)AFrdSWhead.o: lib/AFsp/libtsp/AF/header/AFrdSWhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdSWhead.o lib/AFsp/libtsp/AF/header/AFrdSWhead.c

$(DIR)AFrdTAhead.o: lib/AFsp/libtsp/AF/header/AFrdTAhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdTAhead.o lib/AFsp/libtsp/AF/header/AFrdTAhead.c

$(DIR)AFrdTextAFsp.o: lib/AFsp/libtsp/AF/header/AFrdTextAFsp.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdTextAFsp.o lib/AFsp/libtsp/AF/header/AFrdTextAFsp.c

$(DIR)AFrdWVhead.o: lib/AFsp/libtsp/AF/header/AFrdWVhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/WVpar.h \
		lib/AFsp/include/libtsp/UTtypes.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFrdWVhead.o lib/AFsp/libtsp/AF/header/AFrdWVhead.c

$(DIR)AFreadHead.o: lib/AFsp/libtsp/AF/header/AFreadHead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFreadHead.o lib/AFsp/libtsp/AF/header/AFreadHead.c

$(DIR)AFsetNHread.o: lib/AFsp/libtsp/AF/header/AFsetNHread.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetNHread.o lib/AFsp/libtsp/AF/header/AFsetNHread.c

$(DIR)AFsetNHwrite.o: lib/AFsp/libtsp/AF/header/AFsetNHwrite.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetNHwrite.o lib/AFsp/libtsp/AF/header/AFsetNHwrite.c

$(DIR)AFsetRead.o: lib/AFsp/libtsp/AF/header/AFsetRead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetRead.o lib/AFsp/libtsp/AF/header/AFsetRead.c

$(DIR)AFsetWrite.o: lib/AFsp/libtsp/AF/header/AFsetWrite.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetWrite.o lib/AFsp/libtsp/AF/header/AFsetWrite.c

$(DIR)AFupdAIhead.o: lib/AFsp/libtsp/AF/header/AFupdAIhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AIpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFupdAIhead.o lib/AFsp/libtsp/AF/header/AFupdAIhead.c

$(DIR)AFupdAUhead.o: lib/AFsp/libtsp/AF/header/AFupdAUhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AUpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFupdAUhead.o lib/AFsp/libtsp/AF/header/AFupdAUhead.c

$(DIR)AFupdHead.o: lib/AFsp/libtsp/AF/header/AFupdHead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFupdHead.o lib/AFsp/libtsp/AF/header/AFupdHead.c

$(DIR)AFupdWVhead.o: lib/AFsp/libtsp/AF/header/AFupdWVhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/WVpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFupdWVhead.o lib/AFsp/libtsp/AF/header/AFupdWVhead.c

$(DIR)AFwrAIhead.o: lib/AFsp/libtsp/AF/header/AFwrAIhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AIpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFwrAIhead.o lib/AFsp/libtsp/AF/header/AFwrAIhead.c

$(DIR)AFwrAUhead.o: lib/AFsp/libtsp/AF/header/AFwrAUhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AUpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFwrAUhead.o lib/AFsp/libtsp/AF/header/AFwrAUhead.c

$(DIR)AFwriteHead.o: lib/AFsp/libtsp/AF/header/AFwriteHead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFwriteHead.o lib/AFsp/libtsp/AF/header/AFwriteHead.c

$(DIR)AFwrWVhead.o: lib/AFsp/libtsp/AF/header/AFwrWVhead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/WVpar.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFwrWVhead.o lib/AFsp/libtsp/AF/header/AFwrWVhead.c

$(DIR)AFclose.o: lib/AFsp/libtsp/AF/AFclose.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFclose.o lib/AFsp/libtsp/AF/AFclose.c

$(DIR)AFdReadData.o: lib/AFsp/libtsp/AF/AFdReadData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdReadData.o lib/AFsp/libtsp/AF/AFdReadData.c

$(DIR)AFdWriteData.o: lib/AFsp/libtsp/AF/AFdWriteData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFdWriteData.o lib/AFsp/libtsp/AF/AFdWriteData.c

$(DIR)AFfReadData.o: lib/AFsp/libtsp/AF/AFfReadData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfReadData.o lib/AFsp/libtsp/AF/AFfReadData.c

$(DIR)AFfWriteData.o: lib/AFsp/libtsp/AF/AFfWriteData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFdataio.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFfWriteData.o lib/AFsp/libtsp/AF/AFfWriteData.c

$(DIR)AFopenRead.o: lib/AFsp/libtsp/AF/AFopenRead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFopenRead.o lib/AFsp/libtsp/AF/AFopenRead.c

$(DIR)AFopenWrite.o: lib/AFsp/libtsp/AF/AFopenWrite.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFopenWrite.o lib/AFsp/libtsp/AF/AFopenWrite.c

$(DIR)AFopnRead.o: lib/AFsp/libtsp/AF/AFopnRead.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFopnRead.o lib/AFsp/libtsp/AF/AFopnRead.c

$(DIR)AFopnWrite.o: lib/AFsp/libtsp/AF/AFopnWrite.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFopnWrite.o lib/AFsp/libtsp/AF/AFopnWrite.c

$(DIR)AFreadData.o: lib/AFsp/libtsp/AF/AFreadData.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFreadData.o lib/AFsp/libtsp/AF/AFreadData.c

$(DIR)AFsetFileType.o: lib/AFsp/libtsp/AF/AFsetFileType.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/AFmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetFileType.o lib/AFsp/libtsp/AF/AFsetFileType.c

$(DIR)AFsetInfo.o: lib/AFsp/libtsp/AF/AFsetInfo.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetInfo.o lib/AFsp/libtsp/AF/AFsetInfo.c

$(DIR)AFsetNHpar.o: lib/AFsp/libtsp/AF/AFsetNHpar.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFmsg.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetNHpar.o lib/AFsp/libtsp/AF/AFsetNHpar.c

$(DIR)AFsetSpeaker.o: lib/AFsp/libtsp/AF/AFsetSpeaker.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/AFheader.h \
		lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFsetSpeaker.o lib/AFsp/libtsp/AF/AFsetSpeaker.c

$(DIR)AFwriteData.o: lib/AFsp/libtsp/AF/AFwriteData.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)AFwriteData.o lib/AFsp/libtsp/AF/AFwriteData.c

$(DIR)FIrCosF.o: lib/AFsp/libtsp/FI/nucleus/FIrCosF.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIrCosF.o lib/AFsp/libtsp/FI/nucleus/FIrCosF.c

$(DIR)FIxKaiser.o: lib/AFsp/libtsp/FI/nucleus/FIxKaiser.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIxKaiser.o lib/AFsp/libtsp/FI/nucleus/FIxKaiser.c

$(DIR)FIdBiquad.o: lib/AFsp/libtsp/FI/FIdBiquad.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdBiquad.o lib/AFsp/libtsp/FI/FIdBiquad.c

$(DIR)FIdConvol.o: lib/AFsp/libtsp/FI/FIdConvol.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdConvol.o lib/AFsp/libtsp/FI/FIdConvol.c

$(DIR)FIdConvSI.o: lib/AFsp/libtsp/FI/FIdConvSI.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdConvSI.o lib/AFsp/libtsp/FI/FIdConvSI.c

$(DIR)FIdFiltAP.o: lib/AFsp/libtsp/FI/FIdFiltAP.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdFiltAP.o lib/AFsp/libtsp/FI/FIdFiltAP.c

$(DIR)FIdFiltIIR.o: lib/AFsp/libtsp/FI/FIdFiltIIR.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdFiltIIR.o lib/AFsp/libtsp/FI/FIdFiltIIR.c

$(DIR)FIdKaiserLPF.o: lib/AFsp/libtsp/FI/FIdKaiserLPF.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdKaiserLPF.o lib/AFsp/libtsp/FI/FIdKaiserLPF.c

$(DIR)FIdReadFilt.o: lib/AFsp/libtsp/FI/FIdReadFilt.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FIpar.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdReadFilt.o lib/AFsp/libtsp/FI/FIdReadFilt.c

$(DIR)FIdWinHamm.o: lib/AFsp/libtsp/FI/FIdWinHamm.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdWinHamm.o lib/AFsp/libtsp/FI/FIdWinHamm.c

$(DIR)FIdWinKaiser.o: lib/AFsp/libtsp/FI/FIdWinKaiser.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIdWinKaiser.o lib/AFsp/libtsp/FI/FIdWinKaiser.c

$(DIR)FIfBiquad.o: lib/AFsp/libtsp/FI/FIfBiquad.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfBiquad.o lib/AFsp/libtsp/FI/FIfBiquad.c

$(DIR)FIfConvol.o: lib/AFsp/libtsp/FI/FIfConvol.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfConvol.o lib/AFsp/libtsp/FI/FIfConvol.c

$(DIR)FIfConvSI.o: lib/AFsp/libtsp/FI/FIfConvSI.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfConvSI.o lib/AFsp/libtsp/FI/FIfConvSI.c

$(DIR)FIfDeem.o: lib/AFsp/libtsp/FI/FIfDeem.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfDeem.o lib/AFsp/libtsp/FI/FIfDeem.c

$(DIR)FIfFiltAP.o: lib/AFsp/libtsp/FI/FIfFiltAP.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfFiltAP.o lib/AFsp/libtsp/FI/FIfFiltAP.c

$(DIR)FIfFiltIIR.o: lib/AFsp/libtsp/FI/FIfFiltIIR.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfFiltIIR.o lib/AFsp/libtsp/FI/FIfFiltIIR.c

$(DIR)FIfKaiserLPF.o: lib/AFsp/libtsp/FI/FIfKaiserLPF.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfKaiserLPF.o lib/AFsp/libtsp/FI/FIfKaiserLPF.c

$(DIR)FIfPreem.o: lib/AFsp/libtsp/FI/FIfPreem.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfPreem.o lib/AFsp/libtsp/FI/FIfPreem.c

$(DIR)FIfReadFilt.o: lib/AFsp/libtsp/FI/FIfReadFilt.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FIpar.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfReadFilt.o lib/AFsp/libtsp/FI/FIfReadFilt.c

$(DIR)FIfWinHamm.o: lib/AFsp/libtsp/FI/FIfWinHamm.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfWinHamm.o lib/AFsp/libtsp/FI/FIfWinHamm.c

$(DIR)FIfWinHCos.o: lib/AFsp/libtsp/FI/FIfWinHCos.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfWinHCos.o lib/AFsp/libtsp/FI/FIfWinHCos.c

$(DIR)FIfWinKaiser.o: lib/AFsp/libtsp/FI/FIfWinKaiser.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfWinKaiser.o lib/AFsp/libtsp/FI/FIfWinKaiser.c

$(DIR)FIfWinRCos.o: lib/AFsp/libtsp/FI/FIfWinRCos.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FImsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIfWinRCos.o lib/AFsp/libtsp/FI/FIfWinRCos.c

$(DIR)FIgdelFIR.o: lib/AFsp/libtsp/FI/FIgdelFIR.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIgdelFIR.o lib/AFsp/libtsp/FI/FIgdelFIR.c

$(DIR)FIgdelIIR.o: lib/AFsp/libtsp/FI/FIgdelIIR.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FIgdelIIR.o lib/AFsp/libtsp/FI/FIgdelIIR.c

$(DIR)FLdReadTF.o: lib/AFsp/libtsp/FL/nucleus/FLdReadTF.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdReadTF.o lib/AFsp/libtsp/FL/nucleus/FLdReadTF.c

$(DIR)FLexist.o: lib/AFsp/libtsp/FL/nucleus/FLexist.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLexist.o lib/AFsp/libtsp/FL/nucleus/FLexist.c

$(DIR)FLfileSize.o: lib/AFsp/libtsp/FL/nucleus/FLfileSize.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfileSize.o lib/AFsp/libtsp/FL/nucleus/FLfileSize.c

$(DIR)FLfReadTF.o: lib/AFsp/libtsp/FL/nucleus/FLfReadTF.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfReadTF.o lib/AFsp/libtsp/FL/nucleus/FLfReadTF.c

$(DIR)FLjoinNames.o: lib/AFsp/libtsp/FL/nucleus/FLjoinNames.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLjoinNames.o lib/AFsp/libtsp/FL/nucleus/FLjoinNames.c

$(DIR)FLseekable.o: lib/AFsp/libtsp/FL/nucleus/FLseekable.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLseekable.o lib/AFsp/libtsp/FL/nucleus/FLseekable.c

$(DIR)FLterm.o: lib/AFsp/libtsp/FL/nucleus/FLterm.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLterm.o lib/AFsp/libtsp/FL/nucleus/FLterm.c

$(DIR)FLbackup.o: lib/AFsp/libtsp/FL/FLbackup.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLbackup.o lib/AFsp/libtsp/FL/FLbackup.c

$(DIR)FLbaseName.o: lib/AFsp/libtsp/FL/FLbaseName.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLbaseName.o lib/AFsp/libtsp/FL/FLbaseName.c

$(DIR)FLdate.o: lib/AFsp/libtsp/FL/FLdate.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdate.o lib/AFsp/libtsp/FL/FLdate.c

$(DIR)FLdefName.o: lib/AFsp/libtsp/FL/FLdefName.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdefName.o lib/AFsp/libtsp/FL/FLdefName.c

$(DIR)FLdirName.o: lib/AFsp/libtsp/FL/FLdirName.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdirName.o lib/AFsp/libtsp/FL/FLdirName.c

$(DIR)FLdReadData.o: lib/AFsp/libtsp/FL/FLdReadData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdReadData.o lib/AFsp/libtsp/FL/FLdReadData.c

$(DIR)FLdWriteData.o: lib/AFsp/libtsp/FL/FLdWriteData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLdWriteData.o lib/AFsp/libtsp/FL/FLdWriteData.c

$(DIR)FLexpHome.o: lib/AFsp/libtsp/FL/FLexpHome.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLexpHome.o lib/AFsp/libtsp/FL/FLexpHome.c

$(DIR)FLextName.o: lib/AFsp/libtsp/FL/FLextName.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLextName.o lib/AFsp/libtsp/FL/FLextName.c

$(DIR)FLfileDate.o: lib/AFsp/libtsp/FL/FLfileDate.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfileDate.o lib/AFsp/libtsp/FL/FLfileDate.c

$(DIR)FLfReadData.o: lib/AFsp/libtsp/FL/FLfReadData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfReadData.o lib/AFsp/libtsp/FL/FLfReadData.c

$(DIR)FLfullName.o: lib/AFsp/libtsp/FL/FLfullName.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfullName.o lib/AFsp/libtsp/FL/FLfullName.c

$(DIR)FLfWriteData.o: lib/AFsp/libtsp/FL/FLfWriteData.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLfWriteData.o lib/AFsp/libtsp/FL/FLfWriteData.c

$(DIR)FLgetLine.o: lib/AFsp/libtsp/FL/FLgetLine.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FLmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLgetLine.o lib/AFsp/libtsp/FL/FLgetLine.c

$(DIR)FLgetRec.o: lib/AFsp/libtsp/FL/FLgetRec.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLgetRec.o lib/AFsp/libtsp/FL/FLgetRec.c

$(DIR)FLhomeDir.o: lib/AFsp/libtsp/FL/FLhomeDir.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLhomeDir.o lib/AFsp/libtsp/FL/FLhomeDir.c

$(DIR)FLpathList.o: lib/AFsp/libtsp/FL/FLpathList.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/sysOS.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLpathList.o lib/AFsp/libtsp/FL/FLpathList.c

$(DIR)FLpreName.o: lib/AFsp/libtsp/FL/FLpreName.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLpreName.o lib/AFsp/libtsp/FL/FLpreName.c

$(DIR)FLprompt.o: lib/AFsp/libtsp/FL/FLprompt.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLprompt.o lib/AFsp/libtsp/FL/FLprompt.c

$(DIR)FLreadLine.o: lib/AFsp/libtsp/FL/FLreadLine.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FLreadLine.o lib/AFsp/libtsp/FL/FLreadLine.c

$(DIR)FNbessI0.o: lib/AFsp/libtsp/FN/FNbessI0.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FNbessI0.o lib/AFsp/libtsp/FN/FNbessI0.c

$(DIR)FNevChebP.o: lib/AFsp/libtsp/FN/FNevChebP.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FNevChebP.o lib/AFsp/libtsp/FN/FNevChebP.c

$(DIR)FNgcd.o: lib/AFsp/libtsp/FN/FNgcd.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FNgcd.o lib/AFsp/libtsp/FN/FNgcd.c

$(DIR)FNiLog2.o: lib/AFsp/libtsp/FN/FNiLog2.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/FNmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FNiLog2.o lib/AFsp/libtsp/FN/FNiLog2.c

$(DIR)FNsinc.o: lib/AFsp/libtsp/FN/FNsinc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)FNsinc.o lib/AFsp/libtsp/FN/FNsinc.c

$(DIR)MAfChFactor.o: lib/AFsp/libtsp/MA/nucleus/MAfChFactor.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfChFactor.o lib/AFsp/libtsp/MA/nucleus/MAfChFactor.c

$(DIR)MAfLTSolve.o: lib/AFsp/libtsp/MA/nucleus/MAfLTSolve.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfLTSolve.o lib/AFsp/libtsp/MA/nucleus/MAfLTSolve.c

$(DIR)MAfTTSolve.o: lib/AFsp/libtsp/MA/nucleus/MAfTTSolve.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfTTSolve.o lib/AFsp/libtsp/MA/nucleus/MAfTTSolve.c

$(DIR)MAdAllocMat.o: lib/AFsp/libtsp/MA/MAdAllocMat.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAdAllocMat.o lib/AFsp/libtsp/MA/MAdAllocMat.c

$(DIR)MAdFreeMat.o: lib/AFsp/libtsp/MA/MAdFreeMat.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAdFreeMat.o lib/AFsp/libtsp/MA/MAdFreeMat.c

$(DIR)MAdPrint.o: lib/AFsp/libtsp/MA/MAdPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAdPrint.o lib/AFsp/libtsp/MA/MAdPrint.c

$(DIR)MAfAllocMat.o: lib/AFsp/libtsp/MA/MAfAllocMat.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfAllocMat.o lib/AFsp/libtsp/MA/MAfAllocMat.c

$(DIR)MAfChSolve.o: lib/AFsp/libtsp/MA/MAfChSolve.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfChSolve.o lib/AFsp/libtsp/MA/MAfChSolve.c

$(DIR)MAfFreeMat.o: lib/AFsp/libtsp/MA/MAfFreeMat.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfFreeMat.o lib/AFsp/libtsp/MA/MAfFreeMat.c

$(DIR)MAfPrint.o: lib/AFsp/libtsp/MA/MAfPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfPrint.o lib/AFsp/libtsp/MA/MAfPrint.c

$(DIR)MAfSubMat.o: lib/AFsp/libtsp/MA/MAfSubMat.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfSubMat.o lib/AFsp/libtsp/MA/MAfSubMat.c

$(DIR)MAfSyBilin.o: lib/AFsp/libtsp/MA/MAfSyBilin.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfSyBilin.o lib/AFsp/libtsp/MA/MAfSyBilin.c

$(DIR)MAfSyQuad.o: lib/AFsp/libtsp/MA/MAfSyQuad.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfSyQuad.o lib/AFsp/libtsp/MA/MAfSyQuad.c

$(DIR)MAfTpQuad.o: lib/AFsp/libtsp/MA/MAfTpQuad.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfTpQuad.o lib/AFsp/libtsp/MA/MAfTpQuad.c

$(DIR)MAfTpSolve.o: lib/AFsp/libtsp/MA/MAfTpSolve.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAfTpSolve.o lib/AFsp/libtsp/MA/MAfTpSolve.c

$(DIR)MAiPrint.o: lib/AFsp/libtsp/MA/MAiPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAiPrint.o lib/AFsp/libtsp/MA/MAiPrint.c

$(DIR)MAlPrint.o: lib/AFsp/libtsp/MA/MAlPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MAlPrint.o lib/AFsp/libtsp/MA/MAlPrint.c

$(DIR)MScoefMC.o: lib/AFsp/libtsp/MS/nucleus/MScoefMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MScoefMC.o lib/AFsp/libtsp/MS/nucleus/MScoefMC.c

$(DIR)MSdSlopeMC.o: lib/AFsp/libtsp/MS/nucleus/MSdSlopeMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdSlopeMC.o lib/AFsp/libtsp/MS/nucleus/MSdSlopeMC.c

$(DIR)MSevalMC.o: lib/AFsp/libtsp/MS/nucleus/MSevalMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSevalMC.o lib/AFsp/libtsp/MS/nucleus/MSevalMC.c

$(DIR)MSfSlopeMC.o: lib/AFsp/libtsp/MS/nucleus/MSfSlopeMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfSlopeMC.o lib/AFsp/libtsp/MS/nucleus/MSfSlopeMC.c

$(DIR)MSdConvCof.o: lib/AFsp/libtsp/MS/MSdConvCof.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdConvCof.o lib/AFsp/libtsp/MS/MSdConvCof.c

$(DIR)MSdDeconvCof.o: lib/AFsp/libtsp/MS/MSdDeconvCof.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/MSmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdDeconvCof.o lib/AFsp/libtsp/MS/MSdDeconvCof.c

$(DIR)MSdIntLin.o: lib/AFsp/libtsp/MS/MSdIntLin.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/MSmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdIntLin.o lib/AFsp/libtsp/MS/MSdIntLin.c

$(DIR)MSdIntMC.o: lib/AFsp/libtsp/MS/MSdIntMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/MSmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdIntMC.o lib/AFsp/libtsp/MS/MSdIntMC.c

$(DIR)MSdLocate.o: lib/AFsp/libtsp/MS/MSdLocate.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdLocate.o lib/AFsp/libtsp/MS/MSdLocate.c

$(DIR)MSdNint.o: lib/AFsp/libtsp/MS/MSdNint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdNint.o lib/AFsp/libtsp/MS/MSdNint.c

$(DIR)MSdPolyInt.o: lib/AFsp/libtsp/MS/MSdPolyInt.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSdPolyInt.o lib/AFsp/libtsp/MS/MSdPolyInt.c

$(DIR)MSfConvCof.o: lib/AFsp/libtsp/MS/MSfConvCof.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfConvCof.o lib/AFsp/libtsp/MS/MSfConvCof.c

$(DIR)MSfGaussRand.o: lib/AFsp/libtsp/MS/MSfGaussRand.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfGaussRand.o lib/AFsp/libtsp/MS/MSfGaussRand.c

$(DIR)MSfIntLin.o: lib/AFsp/libtsp/MS/MSfIntLin.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/MSmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfIntLin.o lib/AFsp/libtsp/MS/MSfIntLin.c

$(DIR)MSfIntMC.o: lib/AFsp/libtsp/MS/MSfIntMC.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/MSmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfIntMC.o lib/AFsp/libtsp/MS/MSfIntMC.c

$(DIR)MSfUnifRand.o: lib/AFsp/libtsp/MS/MSfUnifRand.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSfUnifRand.o lib/AFsp/libtsp/MS/MSfUnifRand.c

$(DIR)MSiCeil.o: lib/AFsp/libtsp/MS/MSiCeil.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSiCeil.o lib/AFsp/libtsp/MS/MSiCeil.c

$(DIR)MSiFloor.o: lib/AFsp/libtsp/MS/MSiFloor.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSiFloor.o lib/AFsp/libtsp/MS/MSiFloor.c

$(DIR)MSiPower.o: lib/AFsp/libtsp/MS/MSiPower.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSiPower.o lib/AFsp/libtsp/MS/MSiPower.c

$(DIR)MSlCeil.o: lib/AFsp/libtsp/MS/MSlCeil.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSlCeil.o lib/AFsp/libtsp/MS/MSlCeil.c

$(DIR)MSlFloor.o: lib/AFsp/libtsp/MS/MSlFloor.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSlFloor.o lib/AFsp/libtsp/MS/MSlFloor.c

$(DIR)MSratio.o: lib/AFsp/libtsp/MS/MSratio.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)MSratio.o lib/AFsp/libtsp/MS/MSratio.c

$(DIR)SPdPreFFT.o: lib/AFsp/libtsp/SP/nucleus/SPdPreFFT.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPdPreFFT.o lib/AFsp/libtsp/SP/nucleus/SPdPreFFT.c

$(DIR)SPdTrMat2.o: lib/AFsp/libtsp/SP/nucleus/SPdTrMat2.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPdTrMat2.o lib/AFsp/libtsp/SP/nucleus/SPdTrMat2.c

$(DIR)SPfPreFFT.o: lib/AFsp/libtsp/SP/nucleus/SPfPreFFT.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfPreFFT.o lib/AFsp/libtsp/SP/nucleus/SPfPreFFT.c

$(DIR)SPfTrMat2.o: lib/AFsp/libtsp/SP/nucleus/SPfTrMat2.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfTrMat2.o lib/AFsp/libtsp/SP/nucleus/SPfTrMat2.c

$(DIR)SPautoc.o: lib/AFsp/libtsp/SP/SPautoc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPautoc.o lib/AFsp/libtsp/SP/SPautoc.c

$(DIR)SPcepXpc.o: lib/AFsp/libtsp/SP/SPcepXpc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcepXpc.o lib/AFsp/libtsp/SP/SPcepXpc.c

$(DIR)SPcorBWexp.o: lib/AFsp/libtsp/SP/SPcorBWexp.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcorBWexp.o lib/AFsp/libtsp/SP/SPcorBWexp.c

$(DIR)SPcorFilt.o: lib/AFsp/libtsp/SP/SPcorFilt.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcorFilt.o lib/AFsp/libtsp/SP/SPcorFilt.c

$(DIR)SPcorFmse.o: lib/AFsp/libtsp/SP/SPcorFmse.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcorFmse.o lib/AFsp/libtsp/SP/SPcorFmse.c

$(DIR)SPcorPmse.o: lib/AFsp/libtsp/SP/SPcorPmse.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcorPmse.o lib/AFsp/libtsp/SP/SPcorPmse.c

$(DIR)SPcorXpc.o: lib/AFsp/libtsp/SP/SPcorXpc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcorXpc.o lib/AFsp/libtsp/SP/SPcorXpc.c

$(DIR)SPcovar.o: lib/AFsp/libtsp/SP/SPcovar.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovar.o lib/AFsp/libtsp/SP/SPcovar.c

$(DIR)SPcovCXpc.o: lib/AFsp/libtsp/SP/SPcovCXpc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovCXpc.o lib/AFsp/libtsp/SP/SPcovCXpc.c

$(DIR)SPcovFilt.o: lib/AFsp/libtsp/SP/SPcovFilt.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovFilt.o lib/AFsp/libtsp/SP/SPcovFilt.c

$(DIR)SPcovFmse.o: lib/AFsp/libtsp/SP/SPcovFmse.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovFmse.o lib/AFsp/libtsp/SP/SPcovFmse.c

$(DIR)SPcovLXpc.o: lib/AFsp/libtsp/SP/SPcovLXpc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovLXpc.o lib/AFsp/libtsp/SP/SPcovLXpc.c

$(DIR)SPcovMXpc.o: lib/AFsp/libtsp/SP/SPcovMXpc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovMXpc.o lib/AFsp/libtsp/SP/SPcovMXpc.c

$(DIR)SPcovPmse.o: lib/AFsp/libtsp/SP/SPcovPmse.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovPmse.o lib/AFsp/libtsp/SP/SPcovPmse.c

$(DIR)SPcovXpc.o: lib/AFsp/libtsp/SP/SPcovXpc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPcovXpc.o lib/AFsp/libtsp/SP/SPcovXpc.c

$(DIR)SPdCFFT.o: lib/AFsp/libtsp/SP/SPdCFFT.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPdCFFT.o lib/AFsp/libtsp/SP/SPdCFFT.c

$(DIR)SPdQuantL.o: lib/AFsp/libtsp/SP/SPdQuantL.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPdQuantL.o lib/AFsp/libtsp/SP/SPdQuantL.c

$(DIR)SPdRFFT.o: lib/AFsp/libtsp/SP/SPdRFFT.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPdRFFT.o lib/AFsp/libtsp/SP/SPdRFFT.c

$(DIR)SPecXpc.o: lib/AFsp/libtsp/SP/SPecXpc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPecXpc.o lib/AFsp/libtsp/SP/SPecXpc.c

$(DIR)SPfCFFT.o: lib/AFsp/libtsp/SP/SPfCFFT.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfCFFT.o lib/AFsp/libtsp/SP/SPfCFFT.c

$(DIR)SPfDCT.o: lib/AFsp/libtsp/SP/SPfDCT.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfDCT.o lib/AFsp/libtsp/SP/SPfDCT.c

$(DIR)SPfQuantL.o: lib/AFsp/libtsp/SP/SPfQuantL.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfQuantL.o lib/AFsp/libtsp/SP/SPfQuantL.c

$(DIR)SPfQuantU.o: lib/AFsp/libtsp/SP/SPfQuantU.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfQuantU.o lib/AFsp/libtsp/SP/SPfQuantU.c

$(DIR)SPfRFFT.o: lib/AFsp/libtsp/SP/SPfRFFT.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPfRFFT.o lib/AFsp/libtsp/SP/SPfRFFT.c

$(DIR)SPlpcISdist.o: lib/AFsp/libtsp/SP/SPlpcISdist.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPlpcISdist.o lib/AFsp/libtsp/SP/SPlpcISdist.c

$(DIR)SPlpcLSdist.o: lib/AFsp/libtsp/SP/SPlpcLSdist.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPlpcLSdist.o lib/AFsp/libtsp/SP/SPlpcLSdist.c

$(DIR)SPlsfXpc.o: lib/AFsp/libtsp/SP/SPlsfXpc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPlsfXpc.o lib/AFsp/libtsp/SP/SPlsfXpc.c

$(DIR)SPpcBWexp.o: lib/AFsp/libtsp/SP/SPpcBWexp.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcBWexp.o lib/AFsp/libtsp/SP/SPpcBWexp.c

$(DIR)SPpcXcep.o: lib/AFsp/libtsp/SP/SPpcXcep.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcXcep.o lib/AFsp/libtsp/SP/SPpcXcep.c

$(DIR)SPpcXcor.o: lib/AFsp/libtsp/SP/SPpcXcor.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcXcor.o lib/AFsp/libtsp/SP/SPpcXcor.c

$(DIR)SPpcXec.o: lib/AFsp/libtsp/SP/SPpcXec.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcXec.o lib/AFsp/libtsp/SP/SPpcXec.c

$(DIR)SPpcXlsf.o: lib/AFsp/libtsp/SP/SPpcXlsf.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcXlsf.o lib/AFsp/libtsp/SP/SPpcXlsf.c

$(DIR)SPpcXrc.o: lib/AFsp/libtsp/SP/SPpcXrc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPpcXrc.o lib/AFsp/libtsp/SP/SPpcXrc.c

$(DIR)SPrcXpc.o: lib/AFsp/libtsp/SP/SPrcXpc.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/SPmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)SPrcXpc.o lib/AFsp/libtsp/SP/SPrcXpc.c

$(DIR)STdec1val.o: lib/AFsp/libtsp/ST/nucleus/STdec1val.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdec1val.o lib/AFsp/libtsp/ST/nucleus/STdec1val.c

$(DIR)STdecNval.o: lib/AFsp/libtsp/ST/nucleus/STdecNval.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecNval.o lib/AFsp/libtsp/ST/nucleus/STdecNval.c

$(DIR)STdecPair.o: lib/AFsp/libtsp/ST/nucleus/STdecPair.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecPair.o lib/AFsp/libtsp/ST/nucleus/STdecPair.c

$(DIR)STstrDots.o: lib/AFsp/libtsp/ST/nucleus/STstrDots.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STstrDots.o lib/AFsp/libtsp/ST/nucleus/STstrDots.c

$(DIR)STstrstrNM.o: lib/AFsp/libtsp/ST/nucleus/STstrstrNM.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STstrstrNM.o lib/AFsp/libtsp/ST/nucleus/STstrstrNM.c

$(DIR)STtrimIws.o: lib/AFsp/libtsp/ST/nucleus/STtrimIws.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STtrimIws.o lib/AFsp/libtsp/ST/nucleus/STtrimIws.c

$(DIR)STtrimNMax.o: lib/AFsp/libtsp/ST/nucleus/STtrimNMax.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STtrimNMax.o lib/AFsp/libtsp/ST/nucleus/STtrimNMax.c

$(DIR)STcatMax.o: lib/AFsp/libtsp/ST/STcatMax.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STcatMax.o lib/AFsp/libtsp/ST/STcatMax.c

$(DIR)STcopyMax.o: lib/AFsp/libtsp/ST/STcopyMax.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STcopyMax.o lib/AFsp/libtsp/ST/STcopyMax.c

$(DIR)STcopyNMax.o: lib/AFsp/libtsp/ST/STcopyNMax.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STcopyNMax.o lib/AFsp/libtsp/ST/STcopyNMax.c

$(DIR)STdec1double.o: lib/AFsp/libtsp/ST/STdec1double.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdec1double.o lib/AFsp/libtsp/ST/STdec1double.c

$(DIR)STdec1float.o: lib/AFsp/libtsp/ST/STdec1float.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdec1float.o lib/AFsp/libtsp/ST/STdec1float.c

$(DIR)STdec1int.o: lib/AFsp/libtsp/ST/STdec1int.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdec1int.o lib/AFsp/libtsp/ST/STdec1int.c

$(DIR)STdec1long.o: lib/AFsp/libtsp/ST/STdec1long.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdec1long.o lib/AFsp/libtsp/ST/STdec1long.c

$(DIR)STdecDfrac.o: lib/AFsp/libtsp/ST/STdecDfrac.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecDfrac.o lib/AFsp/libtsp/ST/STdecDfrac.c

$(DIR)STdecIfrac.o: lib/AFsp/libtsp/ST/STdecIfrac.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecIfrac.o lib/AFsp/libtsp/ST/STdecIfrac.c

$(DIR)STdecIrange.o: lib/AFsp/libtsp/ST/STdecIrange.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecIrange.o lib/AFsp/libtsp/ST/STdecIrange.c

$(DIR)STdecLrange.o: lib/AFsp/libtsp/ST/STdecLrange.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecLrange.o lib/AFsp/libtsp/ST/STdecLrange.c

$(DIR)STdecNdouble.o: lib/AFsp/libtsp/ST/STdecNdouble.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecNdouble.o lib/AFsp/libtsp/ST/STdecNdouble.c

$(DIR)STdecNfloat.o: lib/AFsp/libtsp/ST/STdecNfloat.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecNfloat.o lib/AFsp/libtsp/ST/STdecNfloat.c

$(DIR)STdecNint.o: lib/AFsp/libtsp/ST/STdecNint.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecNint.o lib/AFsp/libtsp/ST/STdecNint.c

$(DIR)STdecNlong.o: lib/AFsp/libtsp/ST/STdecNlong.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STdecNlong.o lib/AFsp/libtsp/ST/STdecNlong.c

$(DIR)STfindToken.o: lib/AFsp/libtsp/ST/STfindToken.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STfindToken.o lib/AFsp/libtsp/ST/STfindToken.c

$(DIR)STkeyMatch.o: lib/AFsp/libtsp/ST/STkeyMatch.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STkeyMatch.o lib/AFsp/libtsp/ST/STkeyMatch.c

$(DIR)STkeyXpar.o: lib/AFsp/libtsp/ST/STkeyXpar.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/STmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STkeyXpar.o lib/AFsp/libtsp/ST/STkeyXpar.c

$(DIR)STstrLC.o: lib/AFsp/libtsp/ST/STstrLC.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STstrLC.o lib/AFsp/libtsp/ST/STstrLC.c

$(DIR)STtrim.o: lib/AFsp/libtsp/ST/STtrim.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STtrim.o lib/AFsp/libtsp/ST/STtrim.c

$(DIR)STunQuote.o: lib/AFsp/libtsp/ST/STunQuote.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)STunQuote.o lib/AFsp/libtsp/ST/STunQuote.c

$(DIR)UTbyteCode.o: lib/AFsp/libtsp/UT/nucleus/UTbyteCode.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTbyteCode.o lib/AFsp/libtsp/UT/nucleus/UTbyteCode.c

$(DIR)UTbyteOrder.o: lib/AFsp/libtsp/UT/nucleus/UTbyteOrder.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTtypes.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTbyteOrder.o lib/AFsp/libtsp/UT/nucleus/UTbyteOrder.c

$(DIR)UTcheckIEEE.o: lib/AFsp/libtsp/UT/nucleus/UTcheckIEEE.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTcheckIEEE.o lib/AFsp/libtsp/UT/nucleus/UTcheckIEEE.c

$(DIR)UTctime.o: lib/AFsp/libtsp/UT/nucleus/UTctime.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTctime.o lib/AFsp/libtsp/UT/nucleus/UTctime.c

$(DIR)UTdecOption.o: lib/AFsp/libtsp/UT/nucleus/UTdecOption.c lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTdecOption.o lib/AFsp/libtsp/UT/nucleus/UTdecOption.c

$(DIR)UTdIEEE80.o: lib/AFsp/libtsp/UT/nucleus/UTdIEEE80.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTdIEEE80.o lib/AFsp/libtsp/UT/nucleus/UTdIEEE80.c

$(DIR)UTeIEEE80.o: lib/AFsp/libtsp/UT/nucleus/UTeIEEE80.c lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTtypes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTeIEEE80.o lib/AFsp/libtsp/UT/nucleus/UTeIEEE80.c

$(DIR)UTgetHost.o: lib/AFsp/libtsp/UT/nucleus/UTgetHost.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTgetHost.o lib/AFsp/libtsp/UT/nucleus/UTgetHost.c

$(DIR)UTgetUser.o: lib/AFsp/libtsp/UT/nucleus/UTgetUser.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/sysOS.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTgetUser.o lib/AFsp/libtsp/UT/nucleus/UTgetUser.c

$(DIR)UTswapCode.o: lib/AFsp/libtsp/UT/nucleus/UTswapCode.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTswapCode.o lib/AFsp/libtsp/UT/nucleus/UTswapCode.c

$(DIR)UTdate.o: lib/AFsp/libtsp/UT/UTdate.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTdate.o lib/AFsp/libtsp/UT/UTdate.c

$(DIR)UTerror.o: lib/AFsp/libtsp/UT/UTerror.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTerror.o lib/AFsp/libtsp/UT/UTerror.c

$(DIR)UTfree.o: lib/AFsp/libtsp/UT/UTfree.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTfree.o lib/AFsp/libtsp/UT/UTfree.c

$(DIR)UTgetOption.o: lib/AFsp/libtsp/UT/UTgetOption.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTgetOption.o lib/AFsp/libtsp/UT/UTgetOption.c

$(DIR)UThalt.o: lib/AFsp/libtsp/UT/UThalt.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UThalt.o lib/AFsp/libtsp/UT/UThalt.c

$(DIR)UTmalloc.o: lib/AFsp/libtsp/UT/UTmalloc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTmalloc.o lib/AFsp/libtsp/UT/UTmalloc.c

$(DIR)UTrealloc.o: lib/AFsp/libtsp/UT/UTrealloc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTrealloc.o lib/AFsp/libtsp/UT/UTrealloc.c

$(DIR)UTsetProg.o: lib/AFsp/libtsp/UT/UTsetProg.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTsetProg.o lib/AFsp/libtsp/UT/UTsetProg.c

$(DIR)UTsysMsg.o: lib/AFsp/libtsp/UT/UTsysMsg.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTsysMsg.o lib/AFsp/libtsp/UT/UTsysMsg.c

$(DIR)UTuserName.o: lib/AFsp/libtsp/UT/UTuserName.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTuserName.o lib/AFsp/libtsp/UT/UTuserName.c

$(DIR)UTwarn.o: lib/AFsp/libtsp/UT/UTwarn.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)UTwarn.o lib/AFsp/libtsp/UT/UTwarn.c

$(DIR)VRswapBytes.o: lib/AFsp/libtsp/VR/nucleus/VRswapBytes.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/nucleus.h \
		lib/AFsp/include/libtsp/UTtypes.h \
		lib/AFsp/include/libtsp/VRmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRswapBytes.o lib/AFsp/libtsp/VR/nucleus/VRswapBytes.c

$(DIR)VRdAdd.o: lib/AFsp/libtsp/VR/VRdAdd.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdAdd.o lib/AFsp/libtsp/VR/VRdAdd.c

$(DIR)VRdCopy.o: lib/AFsp/libtsp/VR/VRdCopy.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdCopy.o lib/AFsp/libtsp/VR/VRdCopy.c

$(DIR)VRdCorSym.o: lib/AFsp/libtsp/VR/VRdCorSym.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdCorSym.o lib/AFsp/libtsp/VR/VRdCorSym.c

$(DIR)VRdDotProd.o: lib/AFsp/libtsp/VR/VRdDotProd.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdDotProd.o lib/AFsp/libtsp/VR/VRdDotProd.c

$(DIR)VRdLinInc.o: lib/AFsp/libtsp/VR/VRdLinInc.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdLinInc.o lib/AFsp/libtsp/VR/VRdLinInc.c

$(DIR)VRdMult.o: lib/AFsp/libtsp/VR/VRdMult.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdMult.o lib/AFsp/libtsp/VR/VRdMult.c

$(DIR)VRdPrint.o: lib/AFsp/libtsp/VR/VRdPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdPrint.o lib/AFsp/libtsp/VR/VRdPrint.c

$(DIR)VRdRFFTMSq.o: lib/AFsp/libtsp/VR/VRdRFFTMSq.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdRFFTMSq.o lib/AFsp/libtsp/VR/VRdRFFTMSq.c

$(DIR)VRdScale.o: lib/AFsp/libtsp/VR/VRdScale.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdScale.o lib/AFsp/libtsp/VR/VRdScale.c

$(DIR)VRdSet.o: lib/AFsp/libtsp/VR/VRdSet.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdSet.o lib/AFsp/libtsp/VR/VRdSet.c

$(DIR)VRdShift.o: lib/AFsp/libtsp/VR/VRdShift.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdShift.o lib/AFsp/libtsp/VR/VRdShift.c

$(DIR)VRdSymPart.o: lib/AFsp/libtsp/VR/VRdSymPart.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdSymPart.o lib/AFsp/libtsp/VR/VRdSymPart.c

$(DIR)VRdZero.o: lib/AFsp/libtsp/VR/VRdZero.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRdZero.o lib/AFsp/libtsp/VR/VRdZero.c

$(DIR)VRfAdd.o: lib/AFsp/libtsp/VR/VRfAdd.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfAdd.o lib/AFsp/libtsp/VR/VRfAdd.c

$(DIR)VRfAmpldB.o: lib/AFsp/libtsp/VR/VRfAmpldB.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/VRmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfAmpldB.o lib/AFsp/libtsp/VR/VRfAmpldB.c

$(DIR)VRfCopy.o: lib/AFsp/libtsp/VR/VRfCopy.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfCopy.o lib/AFsp/libtsp/VR/VRfCopy.c

$(DIR)VRfCorSym.o: lib/AFsp/libtsp/VR/VRfCorSym.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfCorSym.o lib/AFsp/libtsp/VR/VRfCorSym.c

$(DIR)VRfDiff.o: lib/AFsp/libtsp/VR/VRfDiff.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfDiff.o lib/AFsp/libtsp/VR/VRfDiff.c

$(DIR)VRfDiffSq.o: lib/AFsp/libtsp/VR/VRfDiffSq.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfDiffSq.o lib/AFsp/libtsp/VR/VRfDiffSq.c

$(DIR)VRfDotProd.o: lib/AFsp/libtsp/VR/VRfDotProd.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfDotProd.o lib/AFsp/libtsp/VR/VRfDotProd.c

$(DIR)VRfLog10.o: lib/AFsp/libtsp/VR/VRfLog10.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/VRmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfLog10.o lib/AFsp/libtsp/VR/VRfLog10.c

$(DIR)VRfMax.o: lib/AFsp/libtsp/VR/VRfMax.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/VRmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfMax.o lib/AFsp/libtsp/VR/VRfMax.c

$(DIR)VRfMin.o: lib/AFsp/libtsp/VR/VRfMin.c lib/AFsp/include/libtsp.h \
		lib/AFsp/include/libtsp/VRmsg.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfMin.o lib/AFsp/libtsp/VR/VRfMin.c

$(DIR)VRfMult.o: lib/AFsp/libtsp/VR/VRfMult.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfMult.o lib/AFsp/libtsp/VR/VRfMult.c

$(DIR)VRfPow10.o: lib/AFsp/libtsp/VR/VRfPow10.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfPow10.o lib/AFsp/libtsp/VR/VRfPow10.c

$(DIR)VRfPrint.o: lib/AFsp/libtsp/VR/VRfPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfPrint.o lib/AFsp/libtsp/VR/VRfPrint.c

$(DIR)VRfRev.o: lib/AFsp/libtsp/VR/VRfRev.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfRev.o lib/AFsp/libtsp/VR/VRfRev.c

$(DIR)VRfScale.o: lib/AFsp/libtsp/VR/VRfScale.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfScale.o lib/AFsp/libtsp/VR/VRfScale.c

$(DIR)VRfSet.o: lib/AFsp/libtsp/VR/VRfSet.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfSet.o lib/AFsp/libtsp/VR/VRfSet.c

$(DIR)VRfShift.o: lib/AFsp/libtsp/VR/VRfShift.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfShift.o lib/AFsp/libtsp/VR/VRfShift.c

$(DIR)VRfSum.o: lib/AFsp/libtsp/VR/VRfSum.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfSum.o lib/AFsp/libtsp/VR/VRfSum.c

$(DIR)VRfZero.o: lib/AFsp/libtsp/VR/VRfZero.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRfZero.o lib/AFsp/libtsp/VR/VRfZero.c

$(DIR)VRiPrint.o: lib/AFsp/libtsp/VR/VRiPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRiPrint.o lib/AFsp/libtsp/VR/VRiPrint.c

$(DIR)VRiZero.o: lib/AFsp/libtsp/VR/VRiZero.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRiZero.o lib/AFsp/libtsp/VR/VRiZero.c

$(DIR)VRlPrint.o: lib/AFsp/libtsp/VR/VRlPrint.c lib/AFsp/include/libtsp.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)VRlPrint.o lib/AFsp/libtsp/VR/VRlPrint.c

$(DIR)sac_arbitraryTree.o: lib/SAC/sac_dec/src/sac_arbitraryTree.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_types.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_arbitraryTree.o lib/SAC/sac_dec/src/sac_arbitraryTree.c

$(DIR)sac_bitdec.o: lib/SAC/sac_dec/src/sac_bitdec.c lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_resdecode.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_restns.h \
		lib/SAC/sac_dec/include/sac_nlc_dec.h \
		lib/SAC/sac_dec/include/sac_types.h \
		lib/SAC/sac_dec/include/sac_dec_hybfilter.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_bitdec.o lib/SAC/sac_dec/src/sac_bitdec.c

$(DIR)sac_bitinput.o: lib/SAC/sac_dec/src/sac_bitinput.c lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_resdefs.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_bitinput.o lib/SAC/sac_dec/src/sac_bitinput.c

$(DIR)sac_blind.o: lib/SAC/sac_dec/src/sac_blind.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_blind.h \
		lib/SAC/sac_dec/include/sac_bitdec.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_blind.o lib/SAC/sac_dec/src/sac_blind.c

$(DIR)sac_calcM1andM2.o: lib/SAC/sac_dec/src/sac_calcM1andM2.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h \
		lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_hrtf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_calcM1andM2.o lib/SAC/sac_dec/src/sac_calcM1andM2.c

$(DIR)sac_config.o: lib/SAC/sac_dec/src/sac_config.c lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_restns.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_config.o lib/SAC/sac_dec/src/sac_config.c

$(DIR)sac_dec.o: lib/SAC/sac_dec/src/sac_dec.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_process.h \
		lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_decor.h \
		lib/SAC/sac_dec/include/sac_dec_hybfilter.h \
		lib/SAC/sac_dec/include/sac_types.h \
		lib/SAC/sac_dec/include/sac_smoothing.h \
		lib/SAC/sac_dec/include/sac_tonality.h \
		lib/SAC/sac_dec/include/sac_TPprocess.h \
		lib/SAC/sac_dec/include/sac_reshapeBBEnv.h \
		lib/SAC/sac_dec/include/sac_nlc_dec.h \
		lib/SAC/sac_dec/include/sac_mdct2qmf.h \
		lib/SAC/sac_dec/include/sac_blind.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h \
		lib/SAC/sac_dec/include/sac_hrtf.h \
		lib/SAC/sac_dec/include/sac_parallelReverb.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_dec.o lib/SAC/sac_dec/src/sac_dec.c

$(DIR)sac_decor.o: lib/SAC/sac_dec/src/sac_decor.c lib/SAC/sac_dec/include/sac_decor.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h \
		lib/SAC/sac_dec/include/sac_dec_hybfilter.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_decor.o lib/SAC/sac_dec/src/sac_decor.c

$(DIR)sac_hrtf.o: lib/SAC/sac_dec/src/sac_hrtf.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_hrtf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_hrtf.o lib/SAC/sac_dec/src/sac_hrtf.c

$(DIR)sac_hrtffilter.o: lib/SAC/sac_dec/src/sac_hrtffilter.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_hrtf.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h \
		lib/SAC/sac_dec/include/sac_dec_hybfilter.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_hrtffilter.o lib/SAC/sac_dec/src/sac_hrtffilter.c

$(DIR)sac_dec_hybfilter.o: lib/SAC/sac_dec/src/sac_dec_hybfilter.c lib/SAC/sac_dec/include/sac_dec_hybfilter.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_dec_hybfilter.o lib/SAC/sac_dec/src/sac_dec_hybfilter.c

$(DIR)sac_mdct2qmf.o: lib/SAC/sac_dec/src/sac_mdct2qmf.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_mdct2qmf.h \
		lib/SAC/sac_dec/include/sac_mdct2qmf_wf_tables.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_mdct2qmf.o lib/SAC/sac_dec/src/sac_mdct2qmf.c

$(DIR)sac_mdct2qmf_wf_tables.o: lib/SAC/sac_dec/src/sac_mdct2qmf_wf_tables.c lib/SAC/sac_dec/include/sac_mdct2qmf_wf_tables.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_mdct2qmf.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_mdct2qmf_wf_tables.o lib/SAC/sac_dec/src/sac_mdct2qmf_wf_tables.c

$(DIR)sac_nlc_dec.o: lib/SAC/sac_dec/src/sac_nlc_dec.c lib/SAC/sac_dec/include/sac_nlc_dec.h \
		lib/SAC/sac_dec/include/sac_types.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nlc_dec.o lib/SAC/sac_dec/src/sac_nlc_dec.c

$(DIR)sac_nodes_cld.o: lib/SAC/sac_dec/src/sac_nodes_cld.c lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nodes_cld.o lib/SAC/sac_dec/src/sac_nodes_cld.c

$(DIR)sac_nodes_cpc.o: lib/SAC/sac_dec/src/sac_nodes_cpc.c lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nodes_cpc.o lib/SAC/sac_dec/src/sac_nodes_cpc.c

$(DIR)sac_nodes_icc.o: lib/SAC/sac_dec/src/sac_nodes_icc.c lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nodes_icc.o lib/SAC/sac_dec/src/sac_nodes_icc.c

$(DIR)sac_nodes_pt0.o: lib/SAC/sac_dec/src/sac_nodes_pt0.c lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nodes_pt0.o lib/SAC/sac_dec/src/sac_nodes_pt0.c

$(DIR)sac_nodes_reshape.o: lib/SAC/sac_dec/src/sac_nodes_reshape.c lib/SAC/sac_dec/include/sac_huff_nodes.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_nodes_reshape.o lib/SAC/sac_dec/src/sac_nodes_reshape.c

$(DIR)sac_parallelReverb.o: lib/SAC/sac_dec/src/sac_parallelReverb.c lib/SAC/sac_dec/include/sac_parallelReverb.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_parallelReverb.o lib/SAC/sac_dec/src/sac_parallelReverb.c

$(DIR)sac_partiallyComplex.o: lib/SAC/sac_dec/src/sac_partiallyComplex.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_partiallyComplex.o lib/SAC/sac_dec/src/sac_partiallyComplex.c

$(DIR)sac_dec_polyphase.o: lib/SAC/sac_dec/src/sac_dec_polyphase.c lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_dec_polyphase.o lib/SAC/sac_dec/src/sac_dec_polyphase.c

$(DIR)sac_process.o: lib/SAC/sac_dec/src/sac_process.c lib/SAC/sac_dec/include/sac_process.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_dec_hybfilter.h \
		lib/SAC/sac_dec/include/sac_decor.h \
		lib/SAC/sac_dec/include/sac_mdct2qmf.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_process.o lib/SAC/sac_dec/src/sac_process.c

$(DIR)sac_resdecode.o: lib/SAC/sac_dec/src/sac_resdecode.c lib/SAC/sac_dec/include/sac_resdecode.h \
		lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_restns.h \
		lib/SAC/sac_dec/include/sac_reshuffdec.h \
		lib/SAC/sac_dec/include/sac_reshuffinit.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_resdecode.o lib/SAC/sac_dec/src/sac_resdecode.c

$(DIR)sac_reshapeBBEnv.o: lib/SAC/sac_dec/src/sac_reshapeBBEnv.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_reshapeBBEnv.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshapeBBEnv.o lib/SAC/sac_dec/src/sac_reshapeBBEnv.c

$(DIR)sac_reshuffdec1.o: lib/SAC/sac_dec/src/sac_reshuffdec1.c lib/SAC/sac_dec/include/sac_resintrins.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_reshuffdec.h \
		lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_restns.h \
		lib/SAC/sac_dec/include/sac_reshuffdec_intern.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshuffdec1.o lib/SAC/sac_dec/src/sac_reshuffdec1.c

$(DIR)sac_reshuffdec2.o: lib/SAC/sac_dec/src/sac_reshuffdec2.c lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_reshuffdec_intern.h \
		lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_restns.h \
		lib/SAC/sac_dec/include/sac_resintrins.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshuffdec2.o lib/SAC/sac_dec/src/sac_reshuffdec2.c

$(DIR)sac_reshuffdec3.o: lib/SAC/sac_dec/src/sac_reshuffdec3.c lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_reshuffdec_intern.h \
		lib/SAC/sac_dec/include/sac_restns.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshuffdec3.o lib/SAC/sac_dec/src/sac_reshuffdec3.c

$(DIR)sac_reshuffinit.o: lib/SAC/sac_dec/src/sac_reshuffinit.c lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_reshufftables.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshuffinit.o lib/SAC/sac_dec/src/sac_reshuffinit.c

$(DIR)sac_reshufftables.o: lib/SAC/sac_dec/src/sac_reshufftables.c lib/SAC/sac_dec/include/sac_reshuffinit.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_reshufftables.o lib/SAC/sac_dec/src/sac_reshufftables.c

$(DIR)sac_resintrins.o: lib/SAC/sac_dec/src/sac_resintrins.c lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_resintrins.h \
		lib/SAC/sac_dec/include/sac_resdefs.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_resintrins.o lib/SAC/sac_dec/src/sac_resintrins.c

$(DIR)sac_restns.o: lib/SAC/sac_dec/src/sac_restns.c lib/SAC/sac_dec/include/sac_resintrins.h \
		lib/SAC/sac_dec/include/sac_resdefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_restns.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_restns.o lib/SAC/sac_dec/src/sac_restns.c

$(DIR)sac_smoothing.o: lib/SAC/sac_dec/src/sac_smoothing.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_bitdec.h \
		lib/SAC/sac_dec/include/sac_smoothing.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_smoothing.o lib/SAC/sac_dec/src/sac_smoothing.c

$(DIR)sac_tonality.o: lib/SAC/sac_dec/src/sac_tonality.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_tonality.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_tonality.o lib/SAC/sac_dec/src/sac_tonality.c

$(DIR)sac_TPprocess.o: lib/SAC/sac_dec/src/sac_TPprocess.c lib/SAC/sac_dec/include/sac_dec.h \
		lib/AFsp/include/libtsp.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h \
		lib/SAC/sac_dec/include/sac_dec_polyphase.h \
		lib/SAC/sac_dec/include/sac_bitinput.h \
		lib/SAC/sac_dec/include/sac_config.h \
		lib/SAC/sac_dec/include/sac_chandefs.h \
		lib/SAC/sac_dec/include/sac_interface.h \
		lib/SAC/sac_dec/include/sac_TPprocess.h \
		lib/SAC/sac_dec/include/sac_process.h \
		lib/SAC/sac_dec/include/sac_calcM1andM2.h \
		lib/SAC/sac_dec/include/sac_sbrconst.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)sac_TPprocess.o lib/SAC/sac_dec/src/sac_TPprocess.c

$(DIR)spatial_bdreader.o: lib/SAC/sac_dec/src/spatial_bdreader.c lib/AFsp/include/libtsp/AFpar.h \
		lib/AFsp/include/libtsp/UTpar.h \
		lib/SAC/sac_dec/include/spatial_bdreader.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)spatial_bdreader.o lib/SAC/sac_dec/src/spatial_bdreader.c

$(DIR)spatial_filereader.o: lib/SAC/sac_dec/src/spatial_filereader.c lib/SAC/sac_dec/include/spatial_filereader.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)spatial_filereader.o lib/SAC/sac_dec/src/spatial_filereader.c

$(DIR)spatial_hrtfreader.o: lib/SAC/sac_dec/src/spatial_hrtfreader.c lib/SAC/sac_dec/include/spatial_hrtfreader.h \
		lib/SAC/sac_dec/include/sac_dec_interface.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(DIR)spatial_hrtfreader.o lib/SAC/sac_dec/src/spatial_hrtfreader.c
	
# Compile effects

# @effects
$(DIR)Compressor.cp.o: src/effects/Compressor.cpp src/effects/Compressor.cp.h \
		$(COMPILE_EFFECTS)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Compressor.cp.o src/effects/Compressor.cpp

$(DIR)Equalizer.cp.o: src/effects/Equalizer.cpp src/effects/Equalizer.cp.h \
		$(COMPILE_EFFECTS)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Equalizer.cp.o src/effects/Equalizer.cpp

$(DIR)Panning.cp.o: src/effects/Panning.cpp src/effects/Panning.cp.h \
		$(COMPILE_EFFECTS)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Panning.cp.o src/effects/Panning.cpp

$(DIR)Reverb.cp.o: src/effects/Reverb.cpp src/effects/Reverb.cp.h \
		$(COMPILE_EFFECTS)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(DIR)Reverb.cp.o src/effects/Reverb.cpp
# @endeffects