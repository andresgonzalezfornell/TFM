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
#include <string.h>
#include <assert.h>

#include "sac_types.h"
#include "sac_nlc_enc.h"
#include "bitstream.h"


typedef unsigned int BIT_BUF_WORD;

char *errorMessage = "No error";

static const int SampleRateTable[] = {
  96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000,
  -1,-1,-1,-1
};

TREEDESCRIPTION TreeConfigTable[] = {
  {5, {0,0,0,0,1}, 0, 1, 6, 0},
  {5, {0,0,1,0,0}, 0, 1, 6, 0},
  {3, {1,0,0,0,0}, 1, 2, 6, 0}
};

int FreqResBinTable[] = {0, 28, 20, 14, 10, 7, 5, 4};

int FreqResStrideTable[] = {1, 2, 5, 28};

static int tempShapeChanTable[][7] = { {5, 5, 4, 6, 6, 4, 4}, {5, 5, 5, 7, 7, 4, 4} };


static void ecData(Stream *bitstream, int data[MAX_NUM_PARAMS][MAX_NUM_BINS], int oldData[MAX_NUM_BINS], LOSSLESSDATA *losslessData, DATA_TYPE dataType, int paramIdx, int numParamSets, int independencyFlag, int startBand, int stopBand)
{
  int ps, setIdx, pbStride, dataBands, i, dataSets;
  int si2ps[MAX_NUM_PARAMS] = {0};


  for(ps=0; ps<numParamSets; ps++) {
    losslessData->bsXXXDataMode[paramIdx][ps] = FINECOARSE;
    writeBits(bitstream, losslessData->bsXXXDataMode[paramIdx][ps], 2);
  }


  setIdx = 0;
  for(ps=0; ps<numParamSets; ps++) {
    si2ps[setIdx] = ps;
    if (ps == numParamSets-1) {
      losslessData->bsDataPair[paramIdx][setIdx] = 0;
      setIdx++;
    }
    else {
      losslessData->bsDataPair[paramIdx][setIdx] = 1;
      losslessData->bsDataPair[paramIdx][setIdx+1] = 1;
      ps++;
      setIdx += 2;
    }
  }
  dataSets = setIdx;

  setIdx = 0;
  while(setIdx < dataSets) {

    writeBits(bitstream, losslessData->bsDataPair[paramIdx][setIdx], 1);
    writeBits(bitstream, losslessData->bsQuantCoarseXXX[paramIdx][setIdx], 1);
    writeBits(bitstream, losslessData->bsFreqResStrideXXX[paramIdx][setIdx], 2);

    pbStride = FreqResStrideTable[losslessData->bsFreqResStrideXXX[paramIdx][setIdx]];
    dataBands = (stopBand-startBand-1)/pbStride+1;

    EcDataPairEnc(bitstream, data, oldData, dataType, si2ps[setIdx], startBand, dataBands, losslessData->bsDataPair[paramIdx][setIdx], losslessData->bsQuantCoarseXXX[paramIdx][setIdx], independencyFlag);

    if (losslessData->bsDataPair[paramIdx][setIdx]) {
      assert(losslessData->bsQuantCoarseXXX[paramIdx][setIdx+1] == losslessData->bsQuantCoarseXXX[paramIdx][setIdx]);
      assert(losslessData->bsFreqResStrideXXX[paramIdx][setIdx+1] == losslessData->bsFreqResStrideXXX[paramIdx][setIdx]);
    }

    for(i=0; i<MAX_NUM_BINS; i++) {
      oldData[i] = data[si2ps[setIdx]+losslessData->bsDataPair[paramIdx][setIdx]][i];
    }

    setIdx += losslessData->bsDataPair[paramIdx][setIdx]+1;

  }

}







void DestroySpatialEncoder(BSF_INSTANCE **selfPtr) {
  if(*selfPtr != NULL) {
    free(*selfPtr);
  }
  *selfPtr = NULL;
}


int CreateSpatialEncoder(BSF_INSTANCE **selfPtr) {
  BSF_INSTANCE *self      = NULL;


  self = calloc(1, sizeof(*self));

  if(self == NULL) {
    errorMessage = "Could not allocate encoder";
    return ERROR_NO_FREE_MEMORY;
  }

  *selfPtr = self;

  return ERROR_NONE;
}


int getSpatialSpecificConfigLength(SPATIALSPECIFICCONFIG *config) {

  int i, numBits, numBytes, samplingFreqIdx;

  TREEDESCRIPTION *treeDescription = &(config->treeDescription);

  numBits = 37;

  samplingFreqIdx = 15;
  for(i = 0; i < 15; i++) {
    if(config->bsSamplingFrequency == SampleRateTable[i]) {
      samplingFreqIdx = i;
      break;
    }
  }
  if (samplingFreqIdx == 15) {
    numBits += 24;
  }
  for(i = 0; i < treeDescription->numOttBoxes; i++) {
    if (treeDescription->ottModeLfe[i]) {
      numBits += 5;
    }
  }
  for(i = 0; i < treeDescription->numTttBoxes; i++) {
    numBits += 1;
    numBits += 3;
    if(config->tttConfig[i].bsTttDualMode == 1) {
      numBits += 3;
      numBits += 5;
    }
  }

  numBytes = ((numBits + 7) >> 3);

  return numBytes;
}


SPATIALSPECIFICCONFIG *GetSpatialSpecificConfig(BSF_INSTANCE *selfPtr) {
  return &(selfPtr->spatialSpecificConfig);
}


int WriteSpatialSpecificConfig(Stream *bitstream, BSF_INSTANCE *selfPtr) {
  int i, j, bsSamplingFrequencyIndex;
  SPATIALFRAME *frame = &(selfPtr->currentFrame);
  SPATIALSPECIFICCONFIG *config = &(selfPtr->spatialSpecificConfig);
  TREEDESCRIPTION *treeDescription = &(config->treeDescription);


  if(config->bsTreeConfig <= 6) {
    treeDescription->numOttBoxes = TreeConfigTable[config->bsTreeConfig].numOttBoxes;
    treeDescription->numTttBoxes = TreeConfigTable[config->bsTreeConfig].numTttBoxes;
    treeDescription->numInChan = TreeConfigTable[config->bsTreeConfig].numInChan;
    treeDescription->numOutChan = TreeConfigTable[config->bsTreeConfig].numOutChan;
    for(i = 0; i < MAX_NUM_BOXES; i++) {
      treeDescription->ottModeLfe[i] = TreeConfigTable[config->bsTreeConfig].ottModeLfe[i];
    }
  } else {
    errorMessage = "Reserved tree configuration";
    return ERROR_NOT_IMPLEMENTED;
  }
  selfPtr->numBins = FreqResBinTable[config->bsFreqRes];

  bsSamplingFrequencyIndex = 15;
  for(i = 0; i < 15; i++) {
    if(config->bsSamplingFrequency == SampleRateTable[i]) {
      bsSamplingFrequencyIndex = i;
      break;
    }
  }

  writeBits(bitstream, bsSamplingFrequencyIndex, 4);
  if(bsSamplingFrequencyIndex == 15) {
    writeBits(bitstream, config->bsSamplingFrequency, 24);
  }
  writeBits(bitstream, config->bsFrameLength, 7);
  writeBits(bitstream, config->bsFreqRes, 3);
  writeBits(bitstream, config->bsTreeConfig, 4);
  writeBits(bitstream, config->bsQuantMode, 2);
  writeBits(bitstream, config->bsOneIcc, 1);
  writeBits(bitstream, config->bsArbitraryDownmix, 1);
  writeBits(bitstream, config->bsFixedGainSur, 3);
  writeBits(bitstream, config->bsFixedGainLFE, 3);
  writeBits(bitstream, config->bsFixedGainDMX, 3);
  writeBits(bitstream, config->bsMatrixMode, 1);
  writeBits(bitstream, config->bsTempShapeConfig, 2);
  writeBits(bitstream, config->bsDecorrConfig, 2);
  writeBits(bitstream, config->bs3DaudioMode, 1);

  for(i = 0; i < treeDescription->numOttBoxes; i++) {
    if(treeDescription->ottModeLfe[i]) {
      writeBits(bitstream, config->ottConfig[i].bsOttBands, 5);
    }
  }
  for(i = 0; i < treeDescription->numTttBoxes; i++) {
    writeBits(bitstream, config->tttConfig[i].bsTttDualMode, 1);
    writeBits(bitstream, config->tttConfig[i].bsTttModeLow, 3);
    if(config->tttConfig[i].bsTttDualMode == 1) {
      writeBits(bitstream, config->tttConfig[i].bsTttModeHigh, 3);
      writeBits(bitstream, config->tttConfig[i].bsTttBandsLow, 5);
    }
  }

  if (config->bsTempShapeConfig == 2) {
    errorMessage = "Guided envelope shaping is not supported";
    return ERROR_NOT_IMPLEMENTED;
  }

  if (config->bs3DaudioMode > 0) {
    errorMessage = "3D stereo mode is not supported";
    return ERROR_NOT_IMPLEMENTED;
  }

  ByteAlignWrite(bitstream);


  if (config->bsResidualCoding == 1) {
    errorMessage = "Residual coding is not supported";
    return ERROR_NOT_IMPLEMENTED;
  }
  if (config->bsArbitraryDownmix == 2) {
    errorMessage = "Arbitrary Downmix residual coding is not supported";
    return ERROR_NOT_IMPLEMENTED;
  }
  if (treeDescription->arbitraryTree) {
    errorMessage = "Arbitrary trees are not supported";
    return ERROR_NOT_IMPLEMENTED;
  }


  for(i = 0; i < MAX_NUM_BOXES; i++) {
    config->tttConfig[i].bsTttBandsLow = selfPtr->numBins;
  }


  for(i = 0; i < MAX_NUM_BOXES; i++) {
    for(j = 0; j < MAX_NUM_BINS; j++) {
      frame->ottData.cld_old[i][j] = 0;
      frame->ottData.icc_old[i][j] = 0;
      frame->tttData.cpc_cld1_old[i][j] = 0;
      frame->tttData.cpc_cld2_old[i][j] = 0;
      frame->tttData.icc_old[i][j] = 0;
    }
  }


  for(i = 0; i < treeDescription->numOttBoxes; i++) {
    if(!treeDescription->ottModeLfe[i]) {
      config->ottConfig[i].bsOttBands = selfPtr->numBins;
    }
  }
  for(i = 0; i < treeDescription->numTttBoxes; i++) {
    if(!config->tttConfig[i].bsTttDualMode) {
      config->tttConfig[i].bsTttBandsLow = selfPtr->numBins;
    }
  }

  return ERROR_NONE;
}


SPATIALFRAME *GetSpatialFrame(BSF_INSTANCE *selfPtr) {
  return &(selfPtr->currentFrame);
}


int WriteSpatialFrame(Stream *bitstream, BSF_INSTANCE *selfPtr) {
  static int counter = 0;

  int i, j, prev_bsParamSlot, bitsParamSlot, numTempShapeChan;
  SPATIALFRAME *frame = &(selfPtr->currentFrame);
  SPATIALSPECIFICCONFIG *config = &(selfPtr->spatialSpecificConfig);
  int bsIndependencyFlag = frame->bsIndependencyFlag;
  int numParamSets = frame->framingInfo.bsNumParamSets;
  int numOttBoxes = selfPtr->spatialSpecificConfig.treeDescription.numOttBoxes;
  int numTttBoxes = selfPtr->spatialSpecificConfig.treeDescription.numTttBoxes;
  int *ottModeLfe = selfPtr->spatialSpecificConfig.treeDescription.ottModeLfe;

  counter++;

  selfPtr->frameCounter++;

  writeBits(bitstream, frame->framingInfo.bsFramingType, 1);
  writeBits(bitstream, numParamSets-1, 3);

  if(frame->framingInfo.bsFramingType) {
    prev_bsParamSlot = -1;
    for(i = 0; i < numParamSets; i++) {
      bitsParamSlot = 0;
      while((1<<bitsParamSlot) < (config->bsFrameLength+1-numParamSets+i-prev_bsParamSlot)) bitsParamSlot++;
      if(bitsParamSlot > 0) writeBits(bitstream, frame->framingInfo.bsParamSlots[i]-prev_bsParamSlot-1, bitsParamSlot);
      prev_bsParamSlot = frame->framingInfo.bsParamSlots[i];
    }
  }

  writeBits(bitstream, frame->bsIndependencyFlag, 1);

  for(i = 0; i < numOttBoxes; i++) {
    ecData(bitstream, frame->ottData.cld[i], frame->ottData.cld_old[i], &(frame->CLDLosslessData), t_CLD, i, numParamSets, bsIndependencyFlag, 0, config->ottConfig[i].bsOttBands);
  }
  if(selfPtr->spatialSpecificConfig.bsOneIcc) {
    ecData(bitstream, frame->ottData.icc[0], frame->ottData.icc_old[0], &(frame->CLDLosslessData), t_ICC, 0, numParamSets, bsIndependencyFlag, 0, selfPtr->numBins);
  } else {
    for(i = 0; i < numOttBoxes; i++) {
      if(!ottModeLfe[i]) {
        ecData(bitstream, frame->ottData.icc[i], frame->ottData.icc_old[i], &(frame->CLDLosslessData), t_ICC, i, numParamSets, bsIndependencyFlag, 0, selfPtr->numBins);
      }
    }
  }
  for(i = 0; i < numTttBoxes; i++) {
    if(config->tttConfig[i].bsTttModeLow < 2) {
      ecData(bitstream, frame->tttData.cpc_cld1[i], frame->tttData.cpc_cld1_old[i], &(frame->CPCLosslessData), t_CPC, i, numParamSets, bsIndependencyFlag, 0, config->tttConfig[i].bsTttBandsLow);
      ecData(bitstream, frame->tttData.cpc_cld2[i], frame->tttData.cpc_cld2_old[i], &(frame->CPCLosslessData), t_CPC, i, numParamSets, bsIndependencyFlag, 0, config->tttConfig[i].bsTttBandsLow);
      ecData(bitstream, frame->tttData.icc[i], frame->tttData.icc_old[i], &(frame->ICCLosslessData), t_ICC, i, numParamSets, bsIndependencyFlag, 0, config->tttConfig[i].bsTttBandsLow);
    } else {
      ecData(bitstream, frame->tttData.cpc_cld1[i], frame->tttData.cpc_cld1_old[i], &(frame->CLDLosslessData), t_CLD, i, numParamSets, bsIndependencyFlag, 0, config->tttConfig[i].bsTttBandsLow);
      ecData(bitstream, frame->tttData.cpc_cld2[i], frame->tttData.cpc_cld2_old[i], &(frame->CLDLosslessData), t_CLD, i, numParamSets, bsIndependencyFlag, 0, config->tttConfig[i].bsTttBandsLow);
    }
    if(config->tttConfig[i].bsTttDualMode) {
      if(config->tttConfig[i].bsTttModeHigh < 2) {
        ecData(bitstream, frame->tttData.cpc_cld1[i], frame->tttData.cpc_cld1_old[i], &(frame->CPCLosslessData), t_CPC, i, numParamSets, bsIndependencyFlag, config->tttConfig[i].bsTttBandsLow, selfPtr->numBins);
        ecData(bitstream, frame->tttData.cpc_cld2[i], frame->tttData.cpc_cld2_old[i], &(frame->CPCLosslessData), t_CPC, i, numParamSets, bsIndependencyFlag, config->tttConfig[i].bsTttBandsLow, selfPtr->numBins);
        ecData(bitstream, frame->tttData.icc[i], frame->tttData.icc_old[i], &(frame->ICCLosslessData), t_ICC, i, numParamSets, bsIndependencyFlag, config->tttConfig[i].bsTttBandsLow, selfPtr->numBins);
      } else {
        ecData(bitstream, frame->tttData.cpc_cld1[i], frame->tttData.cpc_cld1_old[i], &(frame->CLDLosslessData), t_CLD, i, numParamSets, bsIndependencyFlag, config->tttConfig[i].bsTttBandsLow, selfPtr->numBins);
        ecData(bitstream, frame->tttData.cpc_cld2[i], frame->tttData.cpc_cld2_old[i], &(frame->CLDLosslessData), t_CLD, i, numParamSets, bsIndependencyFlag, config->tttConfig[i].bsTttBandsLow, selfPtr->numBins);
      }
    }
  }
  for(i = 0; i < numParamSets; i++) {
    writeBits(bitstream, frame->smgData.bsSmoothMode[i], 2);
    if(frame->smgData.bsSmoothMode[i] >= 2) {
      writeBits(bitstream, frame->smgData.bsSmoothTime[i], 2);
    }
    if(frame->smgData.bsSmoothMode[i] == 3) {
      writeBits(bitstream, frame->smgData.bsFreqResStride[i], 2);
      for(j = 0; j < selfPtr->numBins; j+=frame->smgData.bsFreqResStride[i]) {
        writeBits(bitstream, frame->smgData.bsSmgData[i][j], 1);
      }
    }
  }
  if (config->bsTempShapeConfig != 0) {
    writeBits(bitstream, frame->tempShapeData.bsTempShapeEnable, 1);
    if(frame->tempShapeData.bsTempShapeEnable) {
      numTempShapeChan = tempShapeChanTable[config->bsTempShapeConfig-1][config->bsTreeConfig];
      for(i = 0; i < numTempShapeChan; i++) {
        writeBits(bitstream, frame->tempShapeData.bsTempShapeEnableChannel[i], 1);
      }
      if (config->bsTempShapeConfig == 2) {
        return ERROR_NOT_IMPLEMENTED;
      }
    }
  }
  if(config->bsArbitraryDownmix) {
    return ERROR_NOT_IMPLEMENTED;
  }

  ByteAlignWrite(bitstream);

  AddSectionMarker(bitstream);


  if(config->bsResidualCoding == 1) {
    return ERROR_NOT_IMPLEMENTED;
  }
  if(config->bsArbitraryDownmix == 2) {
    return ERROR_NOT_IMPLEMENTED;
  }
  if(config->treeDescription.arbitraryTree) {
    return ERROR_NOT_IMPLEMENTED;
  }

  return ERROR_NONE;
}
