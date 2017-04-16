/*******************************************************************************
This software module was originally developed by

Agere Systems, Coding Technologies, Fraunhofer IIS, Philips

and edited by

-

in the course of development of ISO/IEC 23003 for reference purposes and its
performance may not have been optimized. This software module is an
implementation of one or more tools as specified by ISO/IEC 23003. ISO/IEC gives
You a royalty-free, worldwide, non-exclusive, copyright license to copy,
distribute, and make derivative works of this software module or modifications
thereof for use in implementations of ISO/IEC 23003 in products that satisfy
conformance criteria (if any). Those intending to use this software module in
products are advised that its use may infringe existing patents. ISO/IEC have no
liability for use of this software module or modifications thereof. Copyright is
not released for products that do not conform to audiovisual and image-coding
related ITU Recommendations and/or ISO/IEC International Standards.

Agere Systems, Coding Technologies, Fraunhofer IIS, Philips retain full right to
modify and use the code for its (their) own purpose, assign or donate the code
to a third party and to inhibit third parties from using the code for products
that do not conform to MPEG-related ITU Recommendations and/or ISO/IEC
International Standards. This copyright notice must be included in all copies or
derivative works.

Copyright (c) ISO/IEC 2009.
*******************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>



#undef STANDALONE
#define STANDALONE

#define PRINT fprintf
#define  SE stderr

#include "sac_dec.h"
 
#include "libtsp.h"
#include <libtsp/AFpar.h>
#include "sac_polyphase.h"
#include "spatial_filereader.h"
#include "spatial_bdreader.h"
#ifdef HRTF_DYNAMIC_UPDATE
#include "spatial_hrtfreader.h"
#endif 

#include "sac_parallelReverb.h"

void myexit(char *s);

#define	ARGBEGIN	for(argv++,argc--;\
			    argv[0] && argv[0][0]=='-' && argv[0][1];\
			    argc--, argv++) {\
				char *_args, *_argt;\
				char _argc;\
				_args = &argv[0][1];\
				if((_args[0]=='-') && (_args[1]==0)){\
					argc--; argv++; break;\
				}\
				_argc = 0;\
				while((_argc = *_args++))\
				switch(_argc)
#define	ARGEND		}
#define	ARGF()		(_argt=_args, _args="",\
				(*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	ARGC()		_argc


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

int upmixtype = 0;
int hrtfModel = 0;
int binauralQuality = 0;
Twiddle *twiddleExtern = NULL;
BITSTREAM_SOURCE bitstreamSource = BS_FILE;

int              debug[256];
long frameCounter =0;

static int
parse_cmdline(int argc, char *argv[]);


jmp_buf g_JmpBuf;

int
main(int argc, char *argv[])
{
  spatialDec* ourDec;
  long int nSamples;
  long int nChannels;
  double sampleFreq;
  int qmfBands = 0;
  int nTimeSlots;

  AFILE* pcmIn = 0;
  HANDLE_FILE_READER fileReader = NULL;
  HANDLE_BURIED_DATA_READER bdReader = NULL;
  BD_INPUT_BUFFER bdBuffer;
  HANDLE_BYTE_READER bitstream = NULL;
#ifdef HRTF_DYNAMIC_UPDATE
  HANDLE_HRTF_READER hrtfReader = NULL;
  HANDLE_HRTF_SOURCE hrtfSource = NULL;
#endif 

  int done = 0;
  int samplesToReadPerCall;
  int samplesRead;
  float* inSamples;
  float* inSamplesDeinterleaved;

  int i;
  int j;
  int channel;
  int ts;
  long int offset=0;

  SAC_POLYPHASE_ANA_FILTERBANK *filterbank[MAX_INPUT_CHANNELS] = {0};
  float** qmfInReal[MAX_INPUT_CHANNELS];
  float** qmfInImag[MAX_INPUT_CHANNELS];

  float** inPointers[2*MAX_INPUT_CHANNELS];


  PRINT(SE,"***** MPEG Surround Decoder - RefSoft Version 1.10 (PCM downmix) *****\n");
  PRINT(SE,"29 January 2009\n");
  PRINT(SE,"\n");

  
#ifdef PARTIALLY_COMPLEX
  for (i=0 ; i < MAX_INPUT_CHANNELS ; i++) {
    qmfInReal[i] = (float **) calloc (MAX_TIME_SLOTS, sizeof (float*));
    for (j=0 ; j <  MAX_TIME_SLOTS; j++) {
      qmfInReal[i][j] = (float *) calloc (MAX_QMF_BANDS, sizeof (float));
    }
    inPointers[i] = qmfInReal[i];
    (qmfInImag);
  }
#else
  for (i=0 ; i < MAX_INPUT_CHANNELS ; i++) {
    qmfInReal[i] = (float **) calloc (MAX_TIME_SLOTS, sizeof (float*));
    qmfInImag[i] = (float **) calloc (MAX_TIME_SLOTS, sizeof (float*));
    for (j=0 ; j < MAX_TIME_SLOTS; j++) {
      qmfInReal[i][j] = (float *) calloc (MAX_QMF_BANDS, sizeof (float));
      qmfInImag[i][j] = (float *) calloc (MAX_QMF_BANDS, sizeof (float));
    }
    inPointers[2*i  ] = qmfInReal[i];
    inPointers[2*i+1] = qmfInImag[i];
  }
#endif

  if(setjmp(g_JmpBuf)==0) {
  }
  else {
    exit(-1);
  }

  parse_cmdline(argc,argv);

  if (pcmInFile)
    pcmIn = AFopnRead(pcmInFile,
                      &nSamples,
                      &nChannels,
                      &sampleFreq,
                      0);

  if (bitstreamSource == BS_FILE) {
    fileReader = FileReaderOpen(spatialBitstreamFile);
    bitstream = FileReaderGetByteReader(fileReader);
  }

  if (sampleFreq < 27713.0) {
    qmfBands = 32;
  } else if (sampleFreq >= 55426.0) {
    qmfBands = 128;
  } else {
    qmfBands = 64;
  }

  if (bitstreamSource == BS_BURIED) {
    float header[BD_HEADER_LENGTH * MAX_INPUT_CHANNELS];
    int headerSamples = BD_HEADER_LENGTH * nChannels;
    int samplesToRead;

    bdReader = BdReaderOpen(qmfBands);
    bitstream = BdReaderGetByteReader(bdReader);

    bdBuffer.channels      = nChannels;
    bdBuffer.sampleFormat  = pcmIn->Format;
    bdBuffer.bitsPerSample = pcmIn->NbS;
    bdBuffer.data          = header;

    offset += (samplesRead = AFfReadData(pcmIn, offset, header, headerSamples));
    if (samplesRead != headerSamples) done = 1;

    if (BdReaderGetFrameSize(bdReader, &bdBuffer, &nTimeSlots) != 0) done = 1;

    samplesToReadPerCall = nChannels * qmfBands * nTimeSlots;
    inSamples = (float*)calloc(sizeof(float), samplesToReadPerCall);

    bdBuffer.data = inSamples;

    memcpy(inSamples, header, sizeof(float) * headerSamples);
    samplesToRead = samplesToReadPerCall - headerSamples;

    offset += (samplesRead = AFfReadData(pcmIn, offset, &inSamples[headerSamples], samplesToRead));
    if (samplesRead == samplesToRead) {
      if (BdReaderParseFrame(bdReader, &bdBuffer) != 0) done = 1;
    }
    else done = 1;
  }

#ifdef HRTF_DYNAMIC_UPDATE
  if (hrtfModel == 3) {
    hrtfReader = HrtfReaderOpen(hrtfSourceFileName);
    hrtfSource = HrtfReaderGetHrtfSource(hrtfReader);
  }
#endif 

  ourDec = SpatialDecOpen(bitstream,
                          spatialPcmOutFile,
                          (int)sampleFreq,
                          (int)nChannels,
                          &nTimeSlots,
                          qmfBands,
                          decType,
                          upmixtype,
                          hrtfModel,
#ifdef HRTF_DYNAMIC_UPDATE
                          hrtfSource,
#endif 
                          binauralQuality);


  SacDecInitAnaFilterbank(NULL, qmfBands);
  for (channel = 0; channel < nChannels; channel++)
    SacDecOpenAnaFilterbank(&filterbank[channel]);

  inSamplesDeinterleaved = (float*) calloc(sizeof(float), qmfBands);

  if (bitstreamSource != BS_BURIED) {
    samplesToReadPerCall = nChannels * qmfBands * nTimeSlots;
    inSamples = (float*)calloc(sizeof(float), samplesToReadPerCall);
  }

  
  while (!done) {

    if ((bitstreamSource != BS_BURIED) || (frameCounter > 0)) {
      offset += (samplesRead = AFfReadData(pcmIn, offset, inSamples, samplesToReadPerCall));
      if (samplesRead == samplesToReadPerCall) {
        if (bitstreamSource == BS_BURIED) {
          if (BdReaderParseFrame(bdReader, &bdBuffer) != 0) done = 1;
          if (BdReaderEndOfStream(bdReader)) done = 1;
        }
      }
      else done = 1;
    }

    if ((bitstreamSource == BS_FILE) && FileReaderEof(fileReader)) done = 1;

#ifdef HRTF_DYNAMIC_UPDATE
    if ((hrtfModel == 3) && HrtfReaderEof(hrtfReader)) done = 1;
#endif 

    if ((!done) && (setjmp(g_JmpBuf)==0)) {

      if (upmixtype != 1) {
        SpatialDecParseFrame(ourDec);
      }

#ifdef DMX_GAIN_FIX
      BinauralDoParallelReverb(ourDec, inSamples, 32768.0f);
#else
      BinauralDoParallelReverb(ourDec, inSamples, 32768.0f * SpatialGetClipProtectGain(ourDec));
#endif

      
      for (channel=0; channel < nChannels; channel++) {
        
        for (ts=0; ts< nTimeSlots; ts++) {
          for (i=0; i< qmfBands; i++) {
#ifdef DMX_GAIN_FIX
            if (upmixtype == 2 || upmixtype == 3)
              inSamplesDeinterleaved[i] = inSamples[nChannels* (ts*qmfBands+i)+channel] * 32768.0f;
            else
#endif
              inSamplesDeinterleaved[i] = inSamples[nChannels* (ts*qmfBands+i)+channel] * 32768.0f * SpatialGetClipProtectGain(ourDec);
          }

#ifdef PARTIALLY_COMPLEX
          SacDecCalculateAnaFilterbank( filterbank[channel],
                                     inSamplesDeinterleaved,
                                     inPointers[channel][ts],
                                     NULL );
#else
          SacDecCalculateAnaFilterbank( filterbank[channel],
                                     inSamplesDeinterleaved,
                                     inPointers[2*channel][ts],
                                     inPointers[2*channel+1][ts] );
#endif
        }
      }
      SpatialDecApplyFrame(ourDec,
                           nTimeSlots,
                           inPointers);

      frameCounter++;

    }
    else {
      break;
    }
  }

  
  if (inSamples != NULL)              free(inSamples);
  if (inSamplesDeinterleaved != NULL) free(inSamplesDeinterleaved);

  for (channel = 0; channel < nChannels; channel++)
    SacDecCloseAnaFilterbank(filterbank[channel]);

  SpatialDecClose(ourDec);
  if (bitstreamSource == BS_FILE) FileReaderClose(fileReader);
  if (bitstreamSource == BS_BURIED) BdReaderClose(bdReader);
#ifdef HRTF_DYNAMIC_UPDATE
  if (hrtfModel == 3) HrtfReaderClose(hrtfReader);
#endif 
  AFclose (pcmIn);

  
#ifdef PARTIALLY_COMPLEX
  for (i=0 ; i < MAX_INPUT_CHANNELS ; i++) {
    for (j=0 ; j < MAX_TIME_SLOTS ; j++) {
      if (qmfInReal[i][j] != NULL) free(qmfInReal[i][j]);
    }
    if (qmfInReal[i] != NULL) free(qmfInReal[i]);
  }
#else
  for (i=0 ; i < MAX_INPUT_CHANNELS ; i++) {
    for (j=0 ; j < MAX_TIME_SLOTS ; j++) {
      if (qmfInReal[i][j] != NULL) free(qmfInReal[i][j]);
      if (qmfInImag[i][j] != NULL) free(qmfInImag[i][j]);
    }
    if (qmfInReal[i] != NULL) free(qmfInReal[i]);
    if (qmfInImag[i] != NULL) free(qmfInImag[i]);
  }
#endif
  return 0;
}

void
myexit(char *s)
{

  if (s) {
    PRINT(SE,"\nMYEXIT: %s\n", s);
  }
  longjmp(g_JmpBuf,1);
}

static void usage(char *s)
{
    PRINT(SE, "usage: [options] %s upmixType pcmIn spatialBitstreamIn pcmOut [decType]\n\n", s);
    PRINT(SE, "	options are:\n");
    PRINT(SE, "  upmixType          - 0: normal, 1: blind, 2: binaural, 3: stereo\n");
    PRINT(SE, "  pcmIn              - File name input PCM\n");
    PRINT(SE, "  spatialBitstreamIn - File name input bit stream (not present in blind upmix case)\n");
    PRINT(SE, "  pcmOut             - File name output PCM\n");
#ifndef PARTIALLY_COMPLEX
    PRINT(SE, "  decType            - 0: low, 1: high (default: 0)\n");
#endif
    PRINT(SE, " -D [a-Z]            - enable debugging printouts\n");
    PRINT(SE, " -h                  - show this help\n");
    PRINT(SE, " -s <source>         - Bitstream source (file or buried)\n");
#ifdef HRTF_DYNAMIC_UPDATE
    PRINT(SE, " -t <model>          - HRTF model (kemar, vast, mps_vt or dynamic)\n");
    PRINT(SE, " -y <filename>       - Dynamic HRTF model file\n");
#else 
    PRINT(SE, " -t <model>          - HRTF model (kemar, vast or mps_vt)\n");
#endif 
    PRINT(SE, " -q [0,1]            - Binaural quality: 0 - parametric; 1 - filtering\n");

    myexit(0);
}

static int
parse_cmdline(int argc, char *argv[])
{
  char *cmd;
  char* s;

  cmd = argv[0];

  ARGBEGIN {


  default:
    PRINT(SE, "unknown option\n");
  case 'h':
    usage(cmd);
  case 'D':
    for (s = ARGF(); *s; s++) {
      debug[(int)(*s)] = 1;
      PRINT(SE, "debug option %c enabled\n", *s);
    }
    break;
  case 's':
    s = ARGF();
    if (strcmp(s, "file") == 0) {
      bitstreamSource = BS_FILE;
    }
    else if (strcmp(s, "buried") == 0) {
      bitstreamSource = BS_BURIED;
    }
    else {
      PRINT(SE, "unknown bitstream source\n");
      myexit(0);
    }
    break;
  case 't':
    s = ARGF();
    if (strcmp(s, "kemar") == 0) {
      hrtfModel = 0;
    }
    else if (strcmp(s, "vast") == 0) {
      hrtfModel = 1;
    }
    else if (strcmp(s, "mps_vt") == 0) {
      hrtfModel = 2;
    }
#ifdef HRTF_DYNAMIC_UPDATE
    else if (strcmp(s, "dynamic") == 0) {
      hrtfModel = 3;
    }
#endif
    else if (strcmp(s, "test") == 0) {
      hrtfModel = 4;
    }
    else {
      PRINT(SE, "unknown HRTF model\n");
      myexit(0);
    }
    break;
#ifdef HRTF_DYNAMIC_UPDATE
  case 'y':
    hrtfSourceFileName = ARGF();
    break;
#endif 
  case 'q':
    s = ARGF();
    binauralQuality = atoi(s);
    break;
  }
  ARGEND

  if(argc < 1){
    usage(cmd);
  }
  upmixtype = atoi(argv[0]);
  if ((upmixtype!=0)&&(upmixtype!=1)&&(upmixtype!=2)&&(upmixtype!=3)){
    usage(cmd);
  }
  if (upmixtype == 1){
    bitstreamSource = BS_NONE;
  }
  if (bitstreamSource == BS_FILE){
    if((argc < 4)||(argc>5)){
      usage(cmd);
    }
    pcmInFile = argv[1];
    spatialBitstreamFile = argv[2];
    spatialPcmOutFile = argv[3];
#ifndef PARTIALLY_COMPLEX
    if (argc>4)
    {
      decType = atoi(argv[4]);
    }
#endif
  }
  else {
    if((argc < 3)||(argc>4)){
      usage(cmd);
    }
    pcmInFile = argv[1];
    spatialPcmOutFile = argv[2];
#ifndef PARTIALLY_COMPLEX
    if (argc>3)
    {
      decType = atoi(argv[3]);
    }
#endif

#ifdef PARTIALLY_COMPLEX
    if (upmixType == 2 && binauralQuality == 1)
    {
      fprintf(stdout, "Warning: High quality binaural decoding is not allowed in Low Power mode!!\n\n");
      usage(cmd);
    }
#endif
  }
  if ((decType!=0)&&(decType!=1)){
    usage(cmd);
  }

  return argc;
}
