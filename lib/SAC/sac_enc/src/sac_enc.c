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

#include "stdlib.h"
#include "string.h"
#include "math.h"

#include "sac_enc.h"
#include "bitstream.h"

static int kernels_20[MAX_HYBRID_BANDS] = {
    1,
    0,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    14,
    15,
    15,
    15,
    16,
    16,
    16,
    16,
    17,
    17,
    17,
    17,
    17,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    18,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19,
    19
};

static int ICCQuant(float val)
{
  float pQSteps[7]= {0.9685f, 0.88909f, 0.72105f, 0.48428f, 0.18382f, -0.2945f, -0.7895f};
  int i;

  if(val>=pQSteps[0]) {
    return 0;
  }
  for(i=1;i<6;i++){
    if ((val>=pQSteps[i]) && (val<=pQSteps[i-1])) {
      return i;
    }
  }
  return 7;
}

static int CLDQuant(float val)
{

  float pQSteps[30]= {-47.5, -42.5, -37.5, -32.5, -27.5, -23.5, -20.5, -17.5, -14.5, -11.5, -9.0, -7.0, -5.0, -3.0, -1.0, 1.0, 3.0, 5.0, 7.0, 9.0, 11.5, 14.5, 17.5, 20.5, 23.5, 27.5, 32.5, 37.5, 42.5, 47.5};

  int i;

  if(val<pQSteps[0]) {
    return 0-15;
  }
  for(i=1;i<30;i++){
    if ((val<=pQSteps[i]) && (val>=pQSteps[i-1])) {
      return i-15;
    }
  }
  return 30-15;
}

static void TttBox(int slots, float pReal1[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pImag1[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pReal2[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pImag2[MAX_TIME_SLOTS][MAX_HYBRID_BANDS], float pReal3[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pImag3[MAX_TIME_SLOTS][MAX_HYBRID_BANDS], int* pQClds1, int* pQClds2)
{
  int i,j;

  float CLDs1[PARAMETER_BANDS];
  float CLDs2[PARAMETER_BANDS];
  float pPow1[MAX_HYBRID_BANDS];
  float pPow2[MAX_HYBRID_BANDS];
  float pPow3[MAX_HYBRID_BANDS];

  float pPowParBand1[PARAMETER_BANDS];
  float pPowParBand2[PARAMETER_BANDS];
  float pPowParBand3[PARAMETER_BANDS];

  memset(CLDs1,0,sizeof(CLDs1));
  memset(CLDs2,0,sizeof(CLDs2));
  memset(pPow1,0,sizeof(pPow1));
  memset(pPow2,0,sizeof(pPow2));
  memset(pPow3,0,sizeof(pPow3));
  memset(pPowParBand1,0,sizeof(pPowParBand1));
  memset(pPowParBand2,0,sizeof(pPowParBand2));
  memset(pPowParBand3,0,sizeof(pPowParBand3));


  for(i = 0; i < slots; i++) {


    for(j=0;j<MAX_HYBRID_BANDS;j++) {
      pPow1[j] += pReal1[i][j]*pReal1[i][j]+pImag1[i][j]*pImag1[i][j];
      pPow2[j] += pReal2[i][j]*pReal2[i][j]+pImag2[i][j]*pImag2[i][j];
      pPow3[j] += pReal3[i][j]*pReal3[i][j]+pImag3[i][j]*pImag3[i][j];


      pReal1[i][j] = (pReal1[i][j]+pReal3[i][j]*0.7071f);
      pImag1[i][j] = (pImag1[i][j]+pImag3[i][j]*0.7071f);

      pReal2[i][j] = (pReal2[i][j]+pReal3[i][j]*0.7071f);
      pImag2[i][j] = (pImag2[i][j]+pImag3[i][j]*0.7071f);
    }
  }
  for(i=0;i<MAX_HYBRID_BANDS;i++) {
    pPowParBand1[kernels_20[i]] += pPow1[i];
    pPowParBand2[kernels_20[i]] += pPow2[i];
    pPowParBand3[kernels_20[i]] += pPow3[i];
  }
  for(i=0;i<PARAMETER_BANDS;i++) {
    CLDs1[i] = ((pPowParBand1[i]+pPowParBand2[i])/(pPowParBand3[i]+1e-10f));
    CLDs1[i] = (float)(10.0*log(CLDs1[i]+1e-10)/log(10.0));
    pQClds1[i] = CLDQuant(CLDs1[i]);

    CLDs2[i] = (pPowParBand1[i]/(pPowParBand2[i]+1e-10f));
    CLDs2[i] = (float)(10*log(CLDs2[i]+1e-10f)/log(10.0));
    pQClds2[i] = CLDQuant(CLDs2[i]);
  }
}

static void OttBox(int slots, float pReal1[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pImag1[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pReal2[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],float pImag2[MAX_TIME_SLOTS][MAX_HYBRID_BANDS], int* pQCLDs, int* pQICCs)
{
  int i,j;

  float CLDs[PARAMETER_BANDS];
  float ICCs[PARAMETER_BANDS];
  float pPow1[MAX_HYBRID_BANDS];
  float pPow2[MAX_HYBRID_BANDS];
  float pXCorReal[MAX_HYBRID_BANDS];

  float pPowParBand1[PARAMETER_BANDS];
  float pPowParBand2[PARAMETER_BANDS];
  float pXCorParBand[PARAMETER_BANDS];

  memset(CLDs,0,sizeof(CLDs));
  memset(ICCs,0,sizeof(ICCs));
  memset(pPow1,0,sizeof(pPow1));
  memset(pPow2,0,sizeof(pPow2));
  memset(pXCorReal, 0, sizeof(pXCorReal));
  memset(pPowParBand1,0,sizeof(pPowParBand1));
  memset(pPowParBand2,0,sizeof(pPowParBand2));
  memset(pXCorParBand, 0, sizeof(pXCorParBand));


  for(i = 0; i < slots; i++) {


    for(j = 0; j < MAX_HYBRID_BANDS; j++) {
      pPow1[j] += pReal1[i][j]*pReal1[i][j]+pImag1[i][j]*pImag1[i][j];
      pPow2[j] += pReal2[i][j]*pReal2[i][j]+pImag2[i][j]*pImag2[i][j];
      pXCorReal[j] += pReal1[i][j]*pReal2[i][j] + pImag1[i][j]*pImag2[i][j];


      pReal1[i][j] = (pReal1[i][j]+pReal2[i][j]);
      pImag1[i][j] = (pImag1[i][j]+pImag2[i][j]);
    }
  }
  for(i=0;i<MAX_HYBRID_BANDS;i++) {
    pPowParBand1[kernels_20[i]] += pPow1[i];
    pPowParBand2[kernels_20[i]] += pPow2[i];
    pXCorParBand[kernels_20[i]] += pXCorReal[i];
  }
  for(i=0;i<PARAMETER_BANDS;i++) {
    CLDs[i] = (pPowParBand1[i]/(pPowParBand2[i]+1e-10f));
    ICCs[i] = pXCorParBand[i]/(float)sqrt((pPowParBand1[i] * pPowParBand2[i] + 1e-10));
    CLDs[i] = (float)(10*log(CLDs[i]+1e-10)/log(10.0));
    pQCLDs[i] = CLDQuant(CLDs[i]);
    pQICCs[i] = ICCQuant(ICCs[i]);
  }
}

tSpatialEnc *SpatialEncOpen(int treeConfig, int timeSlots, int *bufferSize, Stream *bitstream)
{
  int i, sacHeaderLen;
  SPATIALSPECIFICCONFIG *pConfig;
  tSpatialEnc *enc = calloc(1, sizeof(tSpatialEnc));

  enc->treeConfig=treeConfig;

  switch(treeConfig) {
  case 5151:
  case 5152:
    enc->outputChannels=1;
    break;
  case 525:
    enc->outputChannels=2;
  }

  enc->timeSlots = timeSlots;

  enc->frameSize = NUM_QMF_BANDS * enc->timeSlots;
  *bufferSize = enc->frameSize;


  SacEncInitSbrSynFilterbank(0,0);
  for(i = 0; i < 6; i++) {
    SacEncInitAnaFilterbank(&enc->filterbank[i]);
    SacEncInitAnaHybFilterbank(&enc->hybStateEnc[i]);
  }


  CreateSpatialEncoder(&enc->bitstreamFormatter);
  pConfig=GetSpatialSpecificConfig(enc->bitstreamFormatter);

  memset(pConfig,0,sizeof(SPATIALSPECIFICCONFIG));

  pConfig->bsSamplingFrequency=44100;

  switch(treeConfig) {
  case 5151:
    pConfig->bsTreeConfig=0;
    pConfig->ottConfig[4].bsOttBands=2;
    break;
  case 5152:
    pConfig->bsTreeConfig=1;
    pConfig->ottConfig[2].bsOttBands=2;
    break;
  case 525:
    pConfig->bsTreeConfig=2;
    pConfig->ottConfig[0].bsOttBands=2;
    pConfig->tttConfig->bsTttBandsLow=PARAMETER_BANDS;
    pConfig->tttConfig->bsTttModeLow=5;
  }

  pConfig->bsFrameLength = enc->timeSlots-1;
  pConfig->bsFreqRes=2;

  pConfig->bsFixedGainSur = 2;
  pConfig->bsFixedGainLFE = 1;
  pConfig->bsFixedGainDMX = 0;

  writeBits(bitstream, 0, 1);
  sacHeaderLen = getSpatialSpecificConfigLength(&enc->bitstreamFormatter->spatialSpecificConfig);
  if (sacHeaderLen < 127) {
	writeBits(bitstream, sacHeaderLen, 7);
  }
  else {
	writeBits(bitstream, 127, 7);
	writeBits(bitstream, sacHeaderLen-127, 16);
  }

  WriteSpatialSpecificConfig(bitstream, enc->bitstreamFormatter);

  return enc;
}

void SpatialEncApply(tSpatialEnc *self, float *audioInput, float *audioOutput, Stream *bitstream)
{
  static int independencyCounter = 0;

  int outputChannels = 0;
  int bufferSize = 0;
  int offset = 0;

  float in[MAX_INPUT_CHANNELS][MAX_BUFFER_SIZE];
  float out[2][MAX_BUFFER_SIZE];

  float mQmfReal[6][MAX_TIME_SLOTS][NUM_QMF_BANDS];
  float mQmfImag[6][MAX_TIME_SLOTS][NUM_QMF_BANDS];
  float mHybridReal[6][MAX_TIME_SLOTS][MAX_HYBRID_BANDS];
  float mHybridImag[6][MAX_TIME_SLOTS][MAX_HYBRID_BANDS];

  float mQmfRealOut[6][MAX_TIME_SLOTS][NUM_QMF_BANDS];
  float mQmfImagOut[6][MAX_TIME_SLOTS][NUM_QMF_BANDS];

  SPATIALFRAME *pFrameData;

  int i, j, k, l;

  float pPreScale[6]={1.f, 1.f, 1.f, 0.3162f, 0.7071f, 0.7071f};


  for(i = 0; i < self->frameSize; i++) {
    for(j = 0; j < 6; j++) {
      in[j][i]=audioInput[i*6+j]*pPreScale[j];
    }
  }


  for(l = 0; l < self->timeSlots; l++) {
    for(j = 0; j < 6; j++) {
      SacEncCalculateAnaFilterbank(&self->filterbank[j], &in[j][l*64], mQmfReal[j][l], mQmfImag[j][l]);
    }
  }


  for(k = 0; k < 6; k++) {
    SacEncApplyAnaHybFilterbank(
      &self->hybStateEnc[k],
      mQmfReal[k],
      mQmfImag[k],
      self->timeSlots,
      mHybridReal[k],
      mHybridImag[k]);
  }

  pFrameData=GetSpatialFrame(self->bitstreamFormatter);


  switch(self->treeConfig) {
  case 5151:

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[1],mHybridImag[1],pFrameData->ottData.cld[3][0], pFrameData->ottData.icc[3][0]);

    OttBox(self->timeSlots,mHybridReal[4],mHybridImag[4],mHybridReal[5],mHybridImag[5],pFrameData->ottData.cld[2][0], pFrameData->ottData.icc[2][0]);

    OttBox(self->timeSlots,mHybridReal[2],mHybridImag[2],mHybridReal[3],mHybridImag[3],pFrameData->ottData.cld[4][0], pFrameData->ottData.icc[4][0]);

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[2],mHybridImag[2],pFrameData->ottData.cld[1][0], pFrameData->ottData.icc[1][0]);

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[4],mHybridImag[4],pFrameData->ottData.cld[0][0], pFrameData->ottData.icc[0][0]);
    break;
  case 5152:

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[4],mHybridImag[4],pFrameData->ottData.cld[3][0], pFrameData->ottData.icc[3][0]);

    OttBox(self->timeSlots,mHybridReal[1],mHybridImag[1],mHybridReal[5],mHybridImag[5],pFrameData->ottData.cld[4][0], pFrameData->ottData.icc[4][0]);

    OttBox(self->timeSlots,mHybridReal[2],mHybridImag[2],mHybridReal[3],mHybridImag[3],pFrameData->ottData.cld[2][0], pFrameData->ottData.icc[2][0]);

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[1],mHybridImag[1],pFrameData->ottData.cld[1][0], pFrameData->ottData.icc[1][0]);

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[2],mHybridImag[2],pFrameData->ottData.cld[0][0], pFrameData->ottData.icc[0][0]);
    break;
  case 525:

    OttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[4],mHybridImag[4],pFrameData->ottData.cld[1][0],pFrameData->ottData.icc[1][0]);

    OttBox(self->timeSlots,mHybridReal[1],mHybridImag[1],mHybridReal[5],mHybridImag[5],pFrameData->ottData.cld[2][0],pFrameData->ottData.icc[2][0]);

    OttBox(self->timeSlots,mHybridReal[2],mHybridImag[2],mHybridReal[3],mHybridImag[3],pFrameData->ottData.cld[0][0],pFrameData->ottData.icc[0][0]);

    TttBox(self->timeSlots,mHybridReal[0],mHybridImag[0],mHybridReal[1],mHybridImag[1],mHybridReal[2],mHybridImag[2],pFrameData->tttData.cpc_cld1[0][0],pFrameData->tttData.cpc_cld2[0][0]);
  }


  pFrameData->framingInfo.bsFramingType=1;
  pFrameData->framingInfo.bsNumParamSets=1;
  pFrameData->framingInfo.bsParamSlots[0]=31;

  pFrameData->bsIndependencyFlag=0;
  if(independencyCounter-- == 0) {
    independencyCounter = 9;
    pFrameData->bsIndependencyFlag=1;
  }

  WriteSpatialFrame(bitstream, self->bitstreamFormatter);


  if(self->outputChannels == 1) {
    SacEncApplySynHybFilterbank(
      mHybridReal[0],
      mHybridImag[0],
      self->timeSlots,
      mQmfRealOut[0],
      mQmfImagOut[0]);

    for(i = 0; i < self->timeSlots; i++) {
      SacEncCalculateSbrSynFilterbank(mQmfRealOut[0][i], mQmfImagOut[0][i], &audioOutput[i*64], 0, 0);
    }
  } else {
    SacEncApplySynHybFilterbank(
      mHybridReal[0],
      mHybridImag[0],
      self->timeSlots,
      mQmfRealOut[0],
      mQmfImagOut[0]);
    SacEncApplySynHybFilterbank(
      mHybridReal[1],
      mHybridImag[1],
      self->timeSlots,
      mQmfRealOut[1],
      mQmfImagOut[1]);

    for(i = 0; i < self->timeSlots; i++) {
      SacEncCalculateSbrSynFilterbank(mQmfRealOut[0][i],
        mQmfImagOut[0][i],
        out[0],
        0,
        0);
      SacEncCalculateSbrSynFilterbank(mQmfRealOut[1][i],
        mQmfImagOut[1][i],
        out[1],
        0,
        1);

      for(j=0;j<64;j++) {
        for(k=0;k<2;k++) {
          audioOutput[j*2+k+2*64*i]=out[k][j];
        }
      }
    }
  }
}

void SpatialEncClose(tSpatialEnc *self)
{
  if(self != NULL) {
    DestroySpatialEncoder(&self->bitstreamFormatter);
    free(self);
  }
}
