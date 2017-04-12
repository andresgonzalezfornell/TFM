#ifndef SPATIAL_FRONTEND_H
#define SPATIAL_FRONTEND_H

#include "stdio.h"
#include "stdlib.h"
#include "setjmp.h"
#include "string.h"

#undef STANDALONE
#define STANDALONE
#define PRINT fprintf
#define  SE stderr

#include "sac_dec.h"
#include "libtsp.h"
#include "libtsp/AFpar.h"
#include "sac_polyphase.h"
#include "spatial_filereader.h"
#include "spatial_bdreader.h"
#ifdef HRTF_DYNAMIC_UPDATE
#include "spatial_hrtfreader.h"
#endif
#include "sac_parallelReverb.h"

#define DMX_GAIN_FIX
#define MAX_QMF_BANDS 128
typedef enum {
  BS_FILE,
  BS_BURIED,
  BS_NONE
} BITSTREAM_SOURCE;
char* spatialBitstreamFile=0;
char* spatialPcmOutFile=0;
char* pcmInFile=0;
#ifdef HRTF_DYNAMIC_UPDATE
char* hrtfSourceFileName=0;
#endif
int decType   = 0;
int upmixType = 0;
int hrtfModel = 0;
int binauralQuality = 0;
Twiddle *twiddleExtern = NULL;
BITSTREAM_SOURCE bitstreamSource = BS_FILE;
int              debug[256];
long frameCounter =0;
jmp_buf g_JmpBuf;

#endif // SPATIAL_FRONTEND_H
