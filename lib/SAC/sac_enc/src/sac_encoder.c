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
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <libtsp.h>

#include "sac_enc.h"
#include "bitstream.h"
#include "sac_bd_embedder.h"

static void encode(AFILE *inputFile, int nSamples, AFILE *downmixFile, Stream *bitFile, int bury, int dmChannels, int timeSlots);

int main(int argc, char *argv[]) {

  int i;
  char *inputFilename = NULL, *downmixFilename = NULL, *bitFilename = NULL;
  int bury = 0;
  int downmixChan = 0;
  int treeConfig = 5151;
  int timeSlots=32;
  AFILE *inputFile = NULL, *downmixFile = NULL;
  Stream bitstream;
  long int sampleNum, channelNum;
  double sampleFreq;
  int error = 0;


  fprintf(stderr, "***** MPEG Surround Encoder - RefSoft Version 1.10 (PCM downmix) *****\n\n");

  if(argc < 6 || !strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")) {
    fprintf(stderr, "\nCommand line switches:\n\n");
    fprintf(stderr, "  -if <filename>   filename of the multi channel input audio file\n");
    fprintf(stderr, "  -df <filename>   filename of the downmix output audio file\n");
    fprintf(stderr, "  -bf <filename>   filename of the bitstream output file or \"buried\"\n");
    fprintf(stderr, "  -tc <number>     tree config: 5151, 5152, 525 [5151 is the default]\n");
    fprintf(stderr, "  -ts <number>     times slots, 16 or 32 [32 is the default]\n");
    fprintf(stderr, "\nExample: %s -tc 5151 -ts 32 -if input.wav -df downmix.wav -bf bitstream.bs\n\n", argv[0]);
    return -1;
  }



  for(i = 1; i < argc; i++) {

    char *arg = argv[i];

    if(!strcmp(arg, "-if")) {
      if(i == argc-1) {
        fprintf(stderr, "\nArgument expected for -if\n\n");
        return -1;
      }
      inputFilename = argv[i+1];
      i++;
    } else if(!strcmp(arg, "-df")) {
      if(i == argc-1) {
        fprintf(stderr, "\nArgument expected for -df\n\n");
        return -1;
      }
      downmixFilename = argv[i+1];
      i++;
    } else if(!strcmp(arg, "-bf")) {
      if(i == argc-1) {
        fprintf(stderr, "\nArgument expected for -bf\n\n");
        return -1;
      }
      bitFilename = argv[i+1];
      i++;
    } else if(!strcmp(arg, "-tc")) {
      if(i == argc-1) {
        fprintf(stderr, "\nArgument expected for -tc\n\n");
        return -1;
      }
      treeConfig = atoi(argv[i+1]);

      i++;
    } else if(!strcmp(arg, "-ts")) {
      if(i == argc-1) {
        fprintf(stderr, "\nArgument expected for -ts\n\n");
        return -1;
      }
      timeSlots = atoi(argv[i+1]);
      i++;
    } else {
      fprintf(stderr, "\nUnknown command line switch %s\n\n", arg);
      return -1;
    }
  }



  if(inputFilename == NULL) {
    fprintf(stderr, "\nNo input file specified\n\n");
    return -1;
  }

  if(downmixFilename == NULL) {
    fprintf(stderr, "\nNo downmix file specified\n\n");
    return -1;
  }

  if(bitFilename == NULL) {
    fprintf(stderr, "\nNo bitstream file specified\n\n");
    return -1;
  }

  switch(treeConfig) {
  case 5151:
  case 5152:
    downmixChan=1;
    break;
  case 525:
    downmixChan=2;
  }

  if(downmixChan != 1 && downmixChan != 2) {
    fprintf(stderr, "\nInvalid argument for -tc\n\n");
    return -1;
  }



  if(strncmp(bitFilename, "buried", 6) ==0) {
    bitFilename = NULL;
    bury = 1;
  }

  InitStream(&bitstream, bitFilename, STREAM_WRITE);

  inputFile = AFopnRead(inputFilename, &sampleNum, &channelNum, &sampleFreq, NULL);
  if(channelNum != 6) {
    fprintf(stderr, "\nInput audio file must have 6 channels\n\n");
    error = 1;
  }
  if(sampleFreq != 44100) {
    fprintf(stderr, "\nInput audio file must have 44.1 kHz sampling rate\n\n");
    error = 1;
  }

  downmixFile = AFopnWrite(downmixFilename, 2+16, 5 , downmixChan, sampleFreq, NULL);



  if(!error) {
    encode(inputFile, sampleNum, downmixFile, &bitstream, bury, treeConfig, timeSlots);
  }



  CloseStream(&bitstream);
  if(inputFile != NULL) {
    AFclose(inputFile);
  }
  if(downmixFile != NULL) {
    AFclose(downmixFile);
  }

  return 0;
}

static void encode(AFILE *inputFile, int nSamples, AFILE *downmixFile, Stream *bitstream, int bury, int treeConfig, int timeSlots) {

  int bufferSize = 0;
  int offset = 0;
  float inputBuffer[MAX_INPUT_CHANNELS * MAX_BUFFER_SIZE];
  float downmixBuffer[MAX_INPUT_CHANNELS * MAX_BUFFER_SIZE];
  int dmChannels;
  unsigned char *bitstreamBuffer;
  int bitstreamLength;
  int frameCounter = 0;
  tBdEmbedder *embedder = NULL;



  tSpatialEnc *enc = SpatialEncOpen(treeConfig, timeSlots, &bufferSize, bitstream);
  if(enc == NULL) {
    fprintf(stderr, "\nFailed to open spatial encoder\n\n");
    return;
  }

  switch(treeConfig) {
  case 5151:
  case 5152:
    dmChannels=1;
    break;
  case 525:
    dmChannels=2;
    break;
  default:
    dmChannels=1;
  }

  if(bury) {
    embedder = BdEmbedderOpen(dmChannels, timeSlots);
    if(embedder == NULL) {
      fprintf(stderr, "\nFailed to open buried data embedder\n\n");
      return;
    }
  }



  while(offset < nSamples) {

    AFfReadData(inputFile, offset, inputBuffer, bufferSize * 6);

    SpatialEncApply(enc, inputBuffer, downmixBuffer, bitstream);

    if(bury) {
      GetFirstSection(bitstream, &bitstreamBuffer, &bitstreamLength);

      if(bitstreamBuffer != 0) {
        BdEmbedderApply(embedder, bitstreamBuffer, bitstreamLength, (frameCounter == 0), downmixBuffer);
      }

      DeleteFirstSection(bitstream);
    }

    AFfWriteData(downmixFile, downmixBuffer, bufferSize * dmChannels);

    offset += bufferSize * 6;

    printf("[%3d%%]\r", (100*offset)/nSamples);

    fflush(stdout);

    frameCounter++;
  }



  SpatialEncClose(enc);

  if(bury) {
    BdEmbedderClose(embedder);
  }
}
