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




#include "sac_dec.h"
#include "sac_process.h"
#include "sac_bitdec.h"
#include "sac_decor.h"
#include "sac_dec_hybfilter.h"
#include "sac_types.h"
#include "sac_smoothing.h"
#include "sac_tonality.h"
#include "sac_TPprocess.h"
#include "sac_reshapeBBEnv.h"
#include "sac_nlc_dec.h"
#include "sac_mdct2qmf.h"

#include "sac_blind.h"

#include "sac_calcM1andM2.h"
#include "sac_hrtf.h"
#include "sac_parallelReverb.h"

#include "math.h"

#include <assert.h>

#define DECORR_FIX 





#ifdef PARTIALLY_COMPLEX
void Cos2Exp ( const float prData[][PC_NUM_BANDS], float *piData, int nDim );
void QmfAlias( spatialDec  *self );
#endif

extern Twiddle *twiddleExtern;


spatialDec* SpatialDecOpen(HANDLE_BYTE_READER bitstream,
                           char* spatialPcmOutFileName,
                           int samplingFreq,
                           int nInChannels,
                           int* pTimeSlots,
                           int nQmfBands,
                           int nDecType,
                           int nUpmixType,
                           int nHrtfModel,
#ifdef HRTF_DYNAMIC_UPDATE
                           HANDLE_HRTF_SOURCE hrtfSource,
#endif 
                           int nBinauralQuality) {


  int nCh, i, j, k, sacHeaderLen;

  spatialDec* self =0;
  self = (spatialDec*) calloc (sizeof(spatialDec),1);

  if (self==0) return 0;
  self->bitstream =0;
  self->outputFile =0;

  
  self->numParameterSets = 1;

  self->decType = nDecType;

  self->upmixType = nUpmixType;

  self->qmfBands = nQmfBands;

#ifdef HRTF_DYNAMIC_UPDATE
  self->hrtfSource = hrtfSource;
#endif 

  self->binauralQuality = nBinauralQuality;


  

  if (self->upmixType != 1) {
    self->bitstream = s_bitinput_open(bitstream);
    if (self->bitstream ==0) {
        SpatialDecClose(self);
        return 0;
    }
  }

  

  if (self->upmixType != 1) {
    self->sacTimeAlignFlag = s_GetBits(self->bitstream, 1);

    sacHeaderLen = s_GetBits(self->bitstream, 7);
    if (sacHeaderLen == 127) {
      sacHeaderLen += s_GetBits(self->bitstream, 16);
    }

    SpatialDecParseSpecificConfig(self, sacHeaderLen);

    s_byteAlign(self->bitstream);

    if (self->sacTimeAlignFlag) {
      self->sacTimeAlign = s_GetBits(self->bitstream, 16);
      if (self->sacTimeAlign != 0) {
        myexit("ERROR: sacTimeAlign != 0 not implemented");
      }
    }
  }
  else {
    SpatialDecDefaultSpecificConfig(self, samplingFreq);
  }

  
  for(i=0; i<MAX_OUTPUT_CHANNELS_AT; i++)
    for(j=0; j<MAX_PARAMETER_BANDS; j++)
      self->prevGainAT[i][j] = 1.0f;

  SpatialDecDecodeHeader(self);


  assert(samplingFreq == self->samplingFreq);
  assert(nInChannels == self->numInputChannels);
  assert((nQmfBands == 32)||(nQmfBands == 64)||(nQmfBands == 128));


  *pTimeSlots = self->timeSlots;

  

  SpatialDecInitResidual(self);
  SPACE_MDCT2QMF_Create(self);

  
  initSpatialTonality(self);
  initParameterSmoothing(self);

  

  SacInitBBEnv(self);

  if (twiddleExtern != NULL) {
    SacDecInitSynFilterbank(twiddleExtern, self->qmfBands);
  }
  else {
    SacDecInitSynFilterbank(NULL, self->qmfBands);
  }

  for (nCh = 0; nCh < self->numOutputChannelsAT; nCh++) {
    SacDecOpenSynFilterbank(&self->qmfFilterState[nCh]);
  }

  
  for (nCh = 0; nCh < self->numInputChannels; nCh++) {
    SacDecInitAnaHybFilterbank(&self->hybFilterState[nCh]);
  }

  if (self->residualCoding) {
    int offset = self->numInputChannels;
    for (nCh = 0; nCh < self->numOttBoxes + self->numTttBoxes; nCh++) {
      if (self->resBands[nCh] > 0) {
        SacDecInitAnaHybFilterbank(&self->hybFilterState[offset + nCh]);
      }
    }
  }

  if (self->arbitraryDownmix == 2) {
    int offset = self->numInputChannels + self->numOttBoxes + self->numTttBoxes;
    for (nCh = 0; nCh < self->numInputChannels; nCh++) {
      SacDecInitAnaHybFilterbank(&self->hybFilterState[offset + nCh]);
    }
  }

  for (nCh=0; nCh<self->numOutputChannelsAT; nCh++)
  {
    SacDecInitSynHybFilterbank();
  }

#ifdef PARTIALLY_COMPLEX
  {
    int seedVec[MAX_NO_DECORR_CHANNELS];
    int psIdx = 0;

    for (k=0; k<MAX_NO_DECORR_CHANNELS; k++){
      seedVec[k] = k;
    }


    if (self->treeConfig == TREE_5151){
      seedVec[0] = 0;
      seedVec[1] = 1;
      seedVec[2] = 2;
      seedVec[3] = 2;
      psIdx = 2;
#ifdef DECORR_FIX
      if (self->upmixType == 2) { 
        psIdx = 0;
      }
#endif
    }
    if (self->treeConfig == TREE_5152){
      seedVec[0] = 0;
      seedVec[1] = 2;
      seedVec[2] = 1;
      seedVec[3] = 1;
      psIdx = 2;
#ifdef DECORR_FIX
      if (self->upmixType == 2) {
        psIdx = 0;
      }
#endif
    }
    if (self->treeConfig == TREE_525){
      seedVec[0] = 0;
      seedVec[1] = 0;
      psIdx = 0;
    }
    if (self->treeConfig == TREE_7271){
      seedVec[0] = 1;
      seedVec[1] = 1;
      seedVec[2] = 2;
      seedVec[3] = 0;
      seedVec[4] = 0;
      psIdx = 1;
    }
    if (self->treeConfig == TREE_7272){
      seedVec[0] = 1;
      seedVec[1] = 1;
      seedVec[2] = 2;
      seedVec[3] = 0;
      seedVec[4] = 0;
      psIdx = 1;
    }
    if (self->treeConfig == TREE_7571){
      seedVec[0] = 0;
      seedVec[1] = 0;
      psIdx = 0;
    }
    if (self->treeConfig == TREE_7572){
      seedVec[0] = 0;
      seedVec[1] = 0;
      psIdx = 0;
    }


    for (k=0; k<MAX_NO_DECORR_CHANNELS; k++)
    {
      int errCode;

      errCode = SpatialDecDecorrelateCreate(  &(self->apDecor[k]),
                                                  self->hybridBands,
                                                  seedVec[k],
                                                  self->decType,
                                                  seedVec[k] == psIdx,
                                                  self->decorrConfig
                                               );
      if (errCode != ERR_NONE) break;
    }
  }

#else

  for (k=0; k<MAX_NO_DECORR_CHANNELS; k++)
  {
    int errCode, idec;

#ifdef DECORR_FIX
    if (self->upmixType == 3) {
      idec = 0;
    }
    else {
      idec = k;
    }
#else
    idec = k;
#endif

    errCode = SpatialDecDecorrelateCreate(  &(self->apDecor[k]),
                                                self->hybridBands,
                                                idec,
                                                self->decType,
                                                0,
                                                self->decorrConfig
                                             );
    if (errCode != ERR_NONE) break;
  }
#endif

  if (self->upmixType == 1) {
    SpatialDecInitBlind(self);
  }

  

  self->outputFile = AFopnWrite(spatialPcmOutFileName,
                                2, 
                                5, 
                                self->numOutputChannelsAT,
                                (double)self->samplingFreq,
                                0);



  SpatialDecInitM1andM2(self, nHrtfModel);


  
  BinauralInitParallelReverb(self);


  
  self->parseNextBitstreamFrame=1;


  
  for(k = 0; k < 2; k++) {

    SPATIAL_BS_FRAME bsFrame = self->bsFrame[k];

    for(i = 0; i < MAX_NUM_OTT; i++) {
      for(j = 0; j < MAX_PARAMETER_BANDS; j++) {
        bsFrame.ottCLDidxPrev       [i][j] = 0;
        bsFrame.ottICCidxPrev       [i][j] = 0;
        bsFrame.cmpOttCLDidxPrev    [i][j] = 0;
        bsFrame.cmpOttICCidxPrev    [i][j] = 0;
#ifdef VERIFICATION_TEST_COMPATIBILITY
        bsFrame.cmpOttICCtempidxPrev[i][j] = 0;
#endif
      }
    }
    for(i = 0; i < MAX_NUM_TTT; i++) {
      for(j = 0; j < MAX_PARAMETER_BANDS; j++) {
        bsFrame.tttCPC1idxPrev   [i][j] = 0;
        bsFrame.tttCPC2idxPrev   [i][j] = 0;
        bsFrame.tttCLD1idxPrev   [i][j] = 0;
        bsFrame.tttCLD2idxPrev   [i][j] = 0;
        bsFrame.tttICCidxPrev    [i][j] = 0;
        bsFrame.cmpTttCPC1idxPrev[i][j] = 0;
        bsFrame.cmpTttCPC2idxPrev[i][j] = 0;
        bsFrame.cmpTttCLD1idxPrev[i][j] = 0;
        bsFrame.cmpTttCLD2idxPrev[i][j] = 0;
        bsFrame.cmpTttICCidxPrev [i][j] = 0;
      }
    }
    for(i = 0; i < MAX_INPUT_CHANNELS; i++) {
      for(j = 0; j < MAX_PARAMETER_BANDS; j++) {
        bsFrame.arbdmxGainIdxPrev[i][j]    = 0;
        bsFrame.cmpArbdmxGainIdxPrev[i][j] = 0;
      }
    }

  }

  return self;
}



void SpatialDecClose(spatialDec* self) {
  int k;

  if (self) {
    for (k = 0; k < self->numOutputChannelsAT; k++) {
      SacDecCloseSynFilterbank(self->qmfFilterState[k]);
    }

    SacDecCloseAnaHybFilterbank(); 
    SacDecCloseSynHybFilterbank();

    for (k = 0; k < MAX_NO_DECORR_CHANNELS; k++) {
      SpatialDecDecorrelateDestroy(&self->apDecor[k]);
    }

    if (self->upmixType == 1) {
      SpatialDecCloseBlind(self);
    }

    if (self->upmixType == 2) {
      SpatialDecCloseHrtfFilterbank(self->hrtfFilter);

#ifdef HRTF_DYNAMIC_UPDATE
      if (self->hrtfSource != NULL) {
        free((HRTF_DATA*) self->hrtfData);
      }
#endif 
    }

	BinauralDestroyParallelReverb(self);

    if (self->outputFile) AFclose(self->outputFile);
    if (self->bitstream) s_bitinput_close(self->bitstream);
    free(self);
  }
  return;
}





void SpatialDecApplyFrame(spatialDec* self,
                          int inTimeSlots,
                          float** pointers[4]   
                          )
{

  float pcmOutBuf[MAX_OUTPUT_CHANNELS*MAX_TIME_SLOTS*MAX_NUM_QMF_BANDS];
  int ch,ts,sam,qs,n;

  
  assert(self->curTimeSlot+inTimeSlots <= self->timeSlots);


#ifdef PARTIALLY_COMPLEX
  
  for (ts = 0; ts < inTimeSlots; ts++) {
    const float invSqrt2 = (float) (1.0 / sqrt(2.0));

    for (ch = 0; ch < self->numInputChannels; ch++) {
      for (qs = 0; qs < PC_NUM_BANDS; qs++) {
        for (n = 0; n < (PC_FILTERLENGTH - 1); n++) {
          self->qmfCos2ExpInput[ch][n][qs] = self->qmfCos2ExpInput[ch][n + 1][qs];
        }
        self->qmfCos2ExpInput[ch][PC_FILTERLENGTH - 1][qs] = pointers[ch][ts][qs];
      }

      Cos2Exp(self->qmfCos2ExpInput[ch], self->qmfInputImag[ch][self->curTimeSlot+ts], PC_NUM_BANDS);

      for (qs = 0; qs < PC_NUM_BANDS; qs++) {
        self->qmfInputReal[ch][self->curTimeSlot+ts][qs]  = invSqrt2 * self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs];
        self->qmfInputImag[ch][self->curTimeSlot+ts][qs] *= invSqrt2;

        self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs] = pointers[ch][ts][qs];
      }

      for (qs = PC_NUM_BANDS; qs < self->qmfBands; qs++) {
        self->qmfInputReal[ch][self->curTimeSlot+ts][qs] = self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs];
        self->qmfInputImag[ch][self->curTimeSlot+ts][qs] = 0;

        self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs] = pointers[ch][ts][qs];
      }
    }

    self->qmfInputDelayIndex++;

    if (self->qmfInputDelayIndex == PC_FILTERDELAY) {
      self->qmfInputDelayIndex = 0;
    }
  }
#else
  
  if (self->upmixType == 1) {
    for (ts = 0; ts < inTimeSlots; ts++) {
      for (ch = 0; ch < self->numInputChannels; ch++) {
        for (qs = 0; qs < self->qmfBands; qs++) {
          self->qmfInputReal[ch][self->curTimeSlot+ts][qs] = pointers[2*ch][ts][qs];
          self->qmfInputImag[ch][self->curTimeSlot+ts][qs] = pointers[2*ch+1][ts][qs];
        }
      }
    }
  }
  else {
    for (ts = 0; ts < inTimeSlots; ts++) {
      for (ch = 0; ch < self->numInputChannels; ch++) {
        for (qs = 0; qs < self->qmfBands; qs++) {
          self->qmfInputReal[ch][self->curTimeSlot+ts][qs] = self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs];
          self->qmfInputImag[ch][self->curTimeSlot+ts][qs] = self->qmfInputDelayImag[ch][self->qmfInputDelayIndex][qs];

          self->qmfInputDelayReal[ch][self->qmfInputDelayIndex][qs] = pointers[2*ch][ts][qs];
          self->qmfInputDelayImag[ch][self->qmfInputDelayIndex][qs] = pointers[2*ch+1][ts][qs];
        }
      }

      self->qmfInputDelayIndex++;

      if (self->qmfInputDelayIndex == PC_FILTERDELAY) {
        self->qmfInputDelayIndex = 0;
      }
    }
  }
#endif

  self->curTimeSlot += inTimeSlots;

  
  if (self->curTimeSlot < self->timeSlots)
    return;

  self->curTimeSlot = 0;

  SpatialDecDecodeFrame(self);

  SpatialDecMDCT2QMF(self);

  SpatialDecHybridQMFAnalysis(self);

  if (self->upmixType == 1) {
    SpatialDecApplyBlind(self);
  }

#ifdef PARTIALLY_COMPLEX
  QmfAlias(self);
#endif

  SpatialDecCreateX(self);

  SpatialDecCalculateM1andM2(self);
  SpatialDecSmoothM1andM2(self);

  SpatialDecApplyM1(self);

  
  SpatialDecCreateW(self);

  SpatialDecApplyM2(self);



  if (self->upmixType != 2) {
    
    if (self->tempShapeConfig == 2) {
      spatialDecReshapeBBEnv(self);
    }
  }

  tpProcess(self);


  if(!self->bsConfig.arbitraryTree && self->upmixType != 2 && self->upmixType != 3) {
    
    for (n=0; n< self->frameLength; n++) {
      self->timeOut[3][n] *= self->lfeGain;       
      self->timeOut[4][n] *= self->surroundGain;  
      self->timeOut[5][n] *= self->surroundGain;  
    
      if(self->treeConfig == 4 || self->treeConfig == 6){
        self->timeOut[6][n] *= self->surroundGain;  
        self->timeOut[7][n] *= self->surroundGain;  
      }
    }
  }

  BinauralApplyParallelReverb(self);

  

  for (ch=0; ch< self->numOutputChannelsAT; ch++) {
    for (sam=0; sam<self->frameLength; sam++) {
      pcmOutBuf[sam*self->numOutputChannelsAT + ch] =
        (float)(self->timeOut[ch][sam] / 32768.0f);
    }
  }

  SpatialDecUpdateBuffers(self);

  

  AFfWriteData(self->outputFile, pcmOutBuf,self->frameLength*self->numOutputChannelsAT);

  self->parseNextBitstreamFrame=1;
}



float SpatialGetClipProtectGain(spatialDec* self) {
  return self->clipProtectGain;
}


