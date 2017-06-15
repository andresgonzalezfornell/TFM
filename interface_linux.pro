#----------------------------------------------------------
# SAC EFFECTS PLATFORM
# Audio effects platform over Spatial Audio Coding decoder
# Created by: Andrés González Fornell
# Escuela Ténica Superior de Ingenieros de Telecomunicación
# Universidad Politécnica de Madrid
#----------------------------------------------------------

QT       += widgets charts multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "SAC Effects"
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

SOURCES += src/interface/main.cpp \
    src/interface/AudioInfo.cpp \
    src/interface/AudioOutput.cpp \
    src/interface/ChannelsList.cpp \
    src/interface/Chart2D.cpp \
    src/interface/EffectsMonitor.cpp \
    src/interface/Encoder.cpp \
    src/interface/SACEffects.cpp \
    src/process/ProcessManager.cpp \
    src/process/File.cpp \
    src/process/AudioSignal.cpp \
    src/sac/sac_decoder.c \
    src/sac/sac_encoder.c \
    src/sac/SACBitstream.cpp \
    src/tools/Logger.cpp \
    src/effects/Effect.cpp \
    src/effects/Compressor.cpp \
    src/effects/Equalizer.cpp \
    src/effects/Reverb.cpp \
    src/effects/Panning.cpp \
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
    lib/SAC/sac_dec/src/sac_dec_hybfilter.c \
    lib/SAC/sac_dec/src/sac_dec_polyphase.c \
    lib/SAC/sac_dec/src/sac_decor.c \
    lib/SAC/sac_dec/src/sac_hrtf.c \
    lib/SAC/sac_dec/src/sac_hrtffilter.c \
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
    lib/SAC/sac_dec/src/spatial_hrtfreader.c \
    lib/SAC/sac_enc/src/bitstream.c \
    lib/SAC/sac_enc/src/sac_bd_embedder.c \
    lib/SAC/sac_enc/src/sac_enc.c \
    lib/SAC/sac_enc/src/sac_enc_hybfilter.c \
    lib/SAC/sac_enc/src/sac_enc_polyphase.c \
    lib/SAC/sac_enc/src/sac_huff_cld.c \
    lib/SAC/sac_enc/src/sac_huff_cpc.c \
    lib/SAC/sac_enc/src/sac_huff_icc.c \
    lib/SAC/sac_enc/src/sac_nlc_enc.c \
    lib/SAC/sac_enc/src/sac_stream.c

HEADERS += \
    src/interface/AudioInfo.h \
    src/interface/AudioObject.h \
    src/interface/AudioOutput.h \
    src/interface/ChannelsList.h \
    src/interface/Chart2D.h \
    src/interface/EffectsMonitor.h \
    src/interface/Encoder.h \
    src/interface/SACEffects.h \
    src/process/ProcessManager.h \
    src/process/File.h \
    src/process/AudioSignal.h \
    src/sac/sac_decoder.h \
    src/sac/sac_encoder.h \
    src/sac/SACBitstream.h \
    src/tools/Logger.h \
    src/effects/Compressor.h \
    src/effects/EffectBase.h \
    src/effects/Effect.h \
    src/effects/Equalizer.h \
    src/effects/Reverb.h \
    src/effects/Panning.h \
    lib/AFsp/include/libtsp/AFdataio.h \
    lib/AFsp/include/libtsp/AFheader.h \
    lib/AFsp/include/libtsp/AFmsg.h \
    lib/AFsp/include/libtsp/AFpar.h \
    lib/AFsp/include/libtsp/AIpar.h \
    lib/AFsp/include/libtsp/AUpar.h \
    lib/AFsp/include/libtsp/ESpar.h \
    lib/AFsp/include/libtsp/FImsg.h \
    lib/AFsp/include/libtsp/FIpar.h \
    lib/AFsp/include/libtsp/FLmsg.h \
    lib/AFsp/include/libtsp/FNmsg.h \
    lib/AFsp/include/libtsp/MSmsg.h \
    lib/AFsp/include/libtsp/nucleus.h \
    lib/AFsp/include/libtsp/SPmsg.h \
    lib/AFsp/include/libtsp/STmsg.h \
    lib/AFsp/include/libtsp/sysOS.h \
    lib/AFsp/include/libtsp/UTmsg.h \
    lib/AFsp/include/libtsp/UTpar.h \
    lib/AFsp/include/libtsp/UTtypes.h \
    lib/AFsp/include/libtsp/VRmsg.h \
    lib/AFsp/include/libtsp/WVpar.h \
    lib/AFsp/include/libtsp.h \
    lib/SAC/sac_dec/include/sac_bitdec.h \
    lib/SAC/sac_dec/include/sac_bitinput.h \
    lib/SAC/sac_dec/include/sac_bitstream.h \
    lib/SAC/sac_dec/include/sac_blind.h \
    lib/SAC/sac_dec/include/sac_calcM1andM2.h \
    lib/SAC/sac_dec/include/sac_chandefs.h \
    lib/SAC/sac_dec/include/sac_config.h \
    lib/SAC/sac_dec/include/sac_dec.h \
    lib/SAC/sac_dec/include/sac_dec_hybfilter.h \
    lib/SAC/sac_dec/include/sac_dec_interface.h \
    lib/SAC/sac_dec/include/sac_dec_polyphase.h \
    lib/SAC/sac_dec/include/sac_decor.h \
    lib/SAC/sac_dec/include/sac_hrtf.h \
    lib/SAC/sac_dec/include/sac_huff_nodes.h \
    lib/SAC/sac_dec/include/sac_interface.h \
    lib/SAC/sac_dec/include/sac_intrins.h \
    lib/SAC/sac_dec/include/sac_mdct2qmf.h \
    lib/SAC/sac_dec/include/sac_mdct2qmf_wf_tables.h \
    lib/SAC/sac_dec/include/sac_nlc_dec.h \
    lib/SAC/sac_dec/include/sac_parallelReverb.h \
    lib/SAC/sac_dec/include/sac_process.h \
    lib/SAC/sac_dec/include/sac_resdecode.h \
    lib/SAC/sac_dec/include/sac_resdefs.h \
    lib/SAC/sac_dec/include/sac_reshapeBBEnv.h \
    lib/SAC/sac_dec/include/sac_reshuffdec.h \
    lib/SAC/sac_dec/include/sac_reshuffdec_intern.h \
    lib/SAC/sac_dec/include/sac_reshuffinit.h \
    lib/SAC/sac_dec/include/sac_reshufftables.h \
    lib/SAC/sac_dec/include/sac_resintrins.h \
    lib/SAC/sac_dec/include/sac_restns.h \
    lib/SAC/sac_dec/include/sac_sbrconst.h \
    lib/SAC/sac_dec/include/sac_smoothing.h \
    lib/SAC/sac_dec/include/sac_stream.h \
    lib/SAC/sac_dec/include/sac_tonality.h \
    lib/SAC/sac_dec/include/sac_TPprocess.h \
    lib/SAC/sac_dec/include/sac_types.h \
    lib/SAC/sac_dec/include/spatial_bdreader.h \
    lib/SAC/sac_dec/include/spatial_filereader.h \
    lib/SAC/sac_dec/include/spatial_frontend.h \
    lib/SAC/sac_dec/include/spatial_hrtfreader.h \
    lib/SAC/sac_enc/include/bitstream.h \
    lib/SAC/sac_enc/include/defines.h \
    lib/SAC/sac_enc/include/sac_bd_embedder.h \
    lib/SAC/sac_enc/include/sac_enc.h \
    lib/SAC/sac_enc/include/sac_enc_hybfilter.h \
    lib/SAC/sac_enc/include/sac_enc_polyphase.h \
    lib/SAC/sac_enc/include/sac_huff_tab.h \
    lib/SAC/sac_enc/include/sac_nlc_enc.h \
    lib/SAC/sac_enc/include/sac_sbrconst.h \
    lib/SAC/sac_enc/include/sac_stream.h \
    lib/SAC/sac_enc/include/sac_types.h

RESOURCES += \
    src/interface/media.qrc

FORMS += \
    src/interface/AudioInfo.ui \
    src/interface/AudioTest.ui \
    src/interface/ChannelsCharts.ui \
    src/interface/Encoder.ui \
    src/interface/SACEffects.ui

# FFTw3
LIBS += -L/usr/local/lib/ -lfftw3
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
PRE_TARGETDEPS += /usr/local/lib/libfftw3.a

# AFsp
#LIBS += -L$$PWD/lib/AFsp/lib/ -ltsp
INCLUDEPATH += $$PWD/lib/AFsp/include
#DEPENDPATH += $$PWD/lib/AFsp/include
#PRE_TARGETDEPS += $$PWD/lib/AFsp/lib/libtsp.a

# SAC encoder
#LIBS += -L$$PWD/lib/SAC/sac_enc/lib/ -lsac_enc
INCLUDEPATH += $$PWD/lib/SAC/sac_enc/include
#DEPENDPATH += $$PWD/lib/SAC/sac_enc/include
#PRE_TARGETDEPS += $$PWD/lib/SAC/sac_enc/lib/libsac_enc.a

# SAC decoder
#LIBS += -L$$PWD/lib/SAC/sac_dec/lib/ -lsac_dec
INCLUDEPATH += $$PWD/lib/SAC/sac_dec/include
#DEPENDPATH += $$PWD/lib/SAC/sac_dec/include
#PRE_TARGETDEPS += $$PWD/lib/SAC/sac_dec/lib/libsac_dec.a
