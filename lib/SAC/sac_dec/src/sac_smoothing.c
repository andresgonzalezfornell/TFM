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
#include "sac_bitdec.h"
#include "sac_smoothing.h"


static float calcFilterCoeff(spatialDec *self, int ps)
{
  int dSlots;
  float delta;
  
  
  if (ps ==0)
    dSlots = self->paramSlot[ps] +1;
  else
    dSlots = self->paramSlot[ps]- self->paramSlot[ps-1];
  
  
  if (self->smoothControl) {
    delta = (float)dSlots / self->smgTime[ps];
  }
  else{
    delta = (float)dSlots / 256.0f;
  }

  return delta;
}






static int getSmoothOnOff(spatialDec *self, float ton[], int ps, int pb)
{

  int smoothBand = 0;

  
  if (self->smoothControl) {
    
    smoothBand = self->smgData[ps][pb];
  }
  else {
    
    smoothBand = (ton[pb] > 0.8f);
  }

  return smoothBand;
}



void  SpatialDecSmoothM1andM2(spatialDec *self)
{
  float ton[MAX_PARAMETER_BANDS];
  int smoothBand;
  float delta;

  int i, ps, pb, row, col;
  int resBands = 0;

  if (self->residualCoding) {
    for (i=0; i<MAX_RESIDUAL_CHANNELS; i++){
      if (self->resBands[i] > resBands){
        resBands = self->resBands[i];
      }
    }
  }

  if (self->arbitraryDownmix == 2) {
    if (resBands < self->arbdmxResidualBands) {
      resBands = self->arbdmxResidualBands;
    }
  }

  if (self->smoothConfig) {
    
    spatialMeasureTonality(self, ton);
  }
  else{
    for(pb=0; pb < MAX_PARAMETER_BANDS; pb++){
      ton[pb] = 0;
    }
  }

  for (ps = 0; ps < self->numParameterSets; ps++) {

    
    delta = calcFilterCoeff(self, ps);

    for (pb = 0; pb < self->numParameterBands; pb++) {
      
      smoothBand = getSmoothOnOff(self, ton, ps, pb);

      for(row = 0; row < MAX_M_OUTPUT; row++){
        for(col = 0; col < MAX_M_INPUT; col++){
          if ( smoothBand && (pb >= resBands) ) {
            
            if (ps > 0) {
              self->M0arbdmx[ps][pb][row][col]    = delta * self->M0arbdmx[ps][pb][row][col] + (1-delta) * self->M0arbdmx[ps-1][pb][row][col];
              self->M1paramReal[ps][pb][row][col] = delta * self->M1paramReal[ps][pb][row][col] + (1-delta) * self->M1paramReal[ps-1][pb][row][col];
              self->M1paramImag[ps][pb][row][col] = delta * self->M1paramImag[ps][pb][row][col] + (1-delta) * self->M1paramImag[ps-1][pb][row][col];
              self->M2decorReal[ps][pb][row][col] = delta * self->M2decorReal[ps][pb][row][col] + (1-delta) * self->M2decorReal[ps-1][pb][row][col];
              self->M2decorImag[ps][pb][row][col] = delta * self->M2decorImag[ps][pb][row][col] + (1-delta) * self->M2decorImag[ps-1][pb][row][col];
              self->M2residReal[ps][pb][row][col] = delta * self->M2residReal[ps][pb][row][col] + (1-delta) * self->M2residReal[ps-1][pb][row][col];
              self->M2residImag[ps][pb][row][col] = delta * self->M2residImag[ps][pb][row][col] + (1-delta) * self->M2residImag[ps-1][pb][row][col];
            }
            else {
              self->M0arbdmx[ps][pb][row][col]    = delta * self->M0arbdmx[ps][pb][row][col] + (1-delta) * self->M0arbdmxPrev[pb][row][col];
              self->M1paramReal[ps][pb][row][col] = delta * self->M1paramReal[ps][pb][row][col] + (1-delta) * self->M1paramRealPrev[pb][row][col];
              self->M1paramImag[ps][pb][row][col] = delta * self->M1paramImag[ps][pb][row][col] + (1-delta) * self->M1paramImagPrev[pb][row][col];
              self->M2decorReal[ps][pb][row][col] = delta * self->M2decorReal[ps][pb][row][col] + (1-delta) * self->M2decorRealPrev[pb][row][col];
              self->M2decorImag[ps][pb][row][col] = delta * self->M2decorImag[ps][pb][row][col] + (1-delta) * self->M2decorImagPrev[pb][row][col];
              self->M2residReal[ps][pb][row][col] = delta * self->M2residReal[ps][pb][row][col] + (1-delta) * self->M2residRealPrev[pb][row][col];
              self->M2residImag[ps][pb][row][col] = delta * self->M2residImag[ps][pb][row][col] + (1-delta) * self->M2residImagPrev[pb][row][col];
            }
          }
        } 
      } 
    } 
  } 
}



void initParameterSmoothing(spatialDec* self)
{
}
