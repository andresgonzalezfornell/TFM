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

#include<math.h>
#include"sac_dec_hybfilter.h"
#include"sac_sbrconst.h"

const float sacDecHybFilterCoef8[PROTO_LEN] = {
   0.00746082949812f,   0.02270420949825f,   0.04546865930473f,
   0.07266113929591f,   0.09885108575264f,   0.11793710567217f,
   0.12500000000000f,   0.11793710567217f,   0.09885108575264f,
   0.07266113929591f,   0.04546865930473f,   0.02270420949825f,
   0.00746082949812f};

const float sacDecHybFilterCoef2[PROTO_LEN] = {
                0.0f,   0.01899487526049f,                0.0f,
  -0.07293139167538f,                0.0f,   0.30596630545168f,
                0.5f,   0.30596630545168f,                0.0f,
  -0.07293139167538f,                0.0f,   0.01899487526049f,
                0.0f};

static int hybrid2qmf[MAX_HYBRID_BANDS] = {
   0,   0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   4,   5,   6,   7,   8,
   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,
  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,
  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104,
 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
 121, 122, 123, 124, 125, 126, 127
};


static void sacKChannelFilteringAsym( const float *pQmfReal,
                                      const float *pQmfImag,
                                      float mHybridReal[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS],
                                      float mHybridImag[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS],
                                      int nSamples,
                                      int k,
                                      int bCplx,
                                      const float *p,
                                      int pLen,
                                      float offset);

int SacDecGetHybridSubbands(int qmfSubbands)
{
    return qmfSubbands - QMF_BANDS_TO_HYBRID + 10;
}

int SacDecGetQmfSubband(int hybridSubband)
{
    return hybrid2qmf[hybridSubband];
}

int SacDecGetParameterPhase(int hybridSubband)
{
    return (hybridSubband < 2)? -1: 1;
}

void SacDecInitAnaHybFilterbank(tHybFilterStateDec *hybStateDec)
{
    int k, n;

    for (k=0; k<QMF_BANDS_TO_HYBRID; k++)
    {
        for (n=0; n< PROTO_LEN-1+MAX_TIME_SLOTS; n++)
        {
            hybStateDec->bufferLFReal[k][n] = 0.0;
            hybStateDec->bufferLFImag[k][n] = 0.0;
        }
    }
    
    for (k=0; k<MAX_NUM_QMF_BANDS; k++)
    {
        for (n=0; n<(PROTO_LEN-1)/2+MAX_TIME_SLOTS; n++)
        {
            hybStateDec->bufferHFReal[k][n] = 0.0;
            hybStateDec->bufferHFImag[k][n] = 0.0;
        }
    }
}

void SacDecInitSynHybFilterbank()
{
    ;
}

void SacDecApplyAnaHybFilterbank(tHybFilterStateDec *hybStateDec,
                              float mQmfReal[MAX_TIME_SLOTS][MAX_NUM_QMF_BANDS],
                              float mQmfImag[MAX_TIME_SLOTS][MAX_NUM_QMF_BANDS],
                              int nrBands,
                              int nrSamples,
                              float mHybridReal[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],
                              float mHybridImag[MAX_TIME_SLOTS][MAX_HYBRID_BANDS])
{
    int nrSamplesShiftLF;   
    int nrSamplesShiftHF;   
    int nrQmfBandsLF;       
    int k,n;

    float mTempOutputReal[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS];
    float mTempOutputImag[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS];

    nrSamplesShiftLF = BUFFER_LEN_LF - nrSamples;
    nrSamplesShiftHF = BUFFER_LEN_HF - nrSamples;

    nrQmfBandsLF = QMF_BANDS_TO_HYBRID;

    
    for(k=0;k<nrQmfBandsLF;k++)
    {
        for(n=0;n<nrSamplesShiftLF;n++)
        {
            hybStateDec->bufferLFReal[k][n] = hybStateDec->bufferLFReal[k][n+nrSamples];
            hybStateDec->bufferLFImag[k][n] = hybStateDec->bufferLFImag[k][n+nrSamples];
        }
    }

    for(k=0;k<nrQmfBandsLF;k++)      
    {
        for(n=0;n<nrSamples;n++)
        {
            hybStateDec->bufferLFReal[k][n+nrSamplesShiftLF] = mQmfReal[n][k];
            hybStateDec->bufferLFImag[k][n+nrSamplesShiftLF] = mQmfImag[n][k];
        }
    }

    
    for(k=0;k<nrBands-nrQmfBandsLF;k++)
    {
        for(n=0;n<nrSamplesShiftHF;n++)
        {
            hybStateDec->bufferHFReal[k][n] = hybStateDec->bufferHFReal[k][n+nrSamples];
            hybStateDec->bufferHFImag[k][n] = hybStateDec->bufferHFImag[k][n+nrSamples];
        }
    }

    for(k=0;k<nrBands-nrQmfBandsLF;k++)      
    {
        for(n=0;n<nrSamples;n++)
        {
            hybStateDec->bufferHFReal[k][n+nrSamplesShiftHF] = mQmfReal[n][k+nrQmfBandsLF];
            hybStateDec->bufferHFImag[k][n+nrSamplesShiftHF] = mQmfImag[n][k+nrQmfBandsLF];
        }
    }

    
    sacKChannelFilteringAsym(&(hybStateDec->bufferLFReal[0][nrSamplesShiftLF+1-PROTO_LEN]),
                             &(hybStateDec->bufferLFImag[0][nrSamplesShiftLF+1-PROTO_LEN]),
                             mTempOutputReal,
                             mTempOutputImag,
                             nrSamples,
                             8,
                             1,
                             sacDecHybFilterCoef8,
                             PROTO_LEN,
                             (float)(PROTO_LEN-1)/2);

    for (k=0; k<2; k++)                                      
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k] = mTempOutputReal[k+6][n];
            mHybridImag[n][k] = mTempOutputImag[k+6][n];
        }
    }
    for (k=0; k<2; k++)                                      
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k+2] = mTempOutputReal[k][n];
            mHybridImag[n][k+2] = mTempOutputImag[k][n];
        }
    }
    for (k=0; k<2; k++)                                      
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k+4] = mTempOutputReal[k+2][n];   
            mHybridImag[n][k+4] = mTempOutputImag[k+2][n];

            mHybridReal[n][k+4] += mTempOutputReal[5-k][n];
            mHybridImag[n][k+4] += mTempOutputImag[5-k][n];
        }
    }

    sacKChannelFilteringAsym(&(hybStateDec->bufferLFReal[1][nrSamplesShiftLF+1-PROTO_LEN]),
                             &(hybStateDec->bufferLFImag[1][nrSamplesShiftLF+1-PROTO_LEN]),
                             mTempOutputReal,
                             mTempOutputImag,
                             nrSamples,
                             2,
                             0,
                             sacDecHybFilterCoef2,
                             PROTO_LEN,
                             (float)(PROTO_LEN-1)/2);

    for (k=0; k<2; k++)                                      
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k+6] = mTempOutputReal[1-k][n];
            mHybridImag[n][k+6] = mTempOutputImag[1-k][n];
        }
    }

    sacKChannelFilteringAsym(&(hybStateDec->bufferLFReal[2][nrSamplesShiftLF+1-PROTO_LEN]),
                             &(hybStateDec->bufferLFImag[2][nrSamplesShiftLF+1-PROTO_LEN]),
                             mTempOutputReal,
                             mTempOutputImag,
                             nrSamples,
                             2,
                             0,
                             sacDecHybFilterCoef2,
                             PROTO_LEN,
                             (float)(PROTO_LEN-1)/2);

    for (k=0; k<2; k++)
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k+8] = mTempOutputReal[k][n];
            mHybridImag[n][k+8] = mTempOutputImag[k][n];
        }
    }

    for (k=0; k<nrBands-nrQmfBandsLF; k++) 
    {
        for (n=0; n<nrSamples; n++)
        {
            mHybridReal[n][k+10] = hybStateDec->bufferHFReal[k][n+nrSamplesShiftHF-(PROTO_LEN-1)/2];
            mHybridImag[n][k+10] = hybStateDec->bufferHFImag[k][n+nrSamplesShiftHF-(PROTO_LEN-1)/2];
        }
    }
}

void SacDecApplySynHybFilterbank(float mHybridReal[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],
                              float mHybridImag[MAX_TIME_SLOTS][MAX_HYBRID_BANDS],
                              int nrBands,
                              int nrSamples,
                              float mQmfReal[MAX_TIME_SLOTS][MAX_NUM_QMF_BANDS],
                              float mQmfImag[MAX_TIME_SLOTS][MAX_NUM_QMF_BANDS])
{
    int k, n;

    for (n=0; n<nrSamples; n++)
    {
        mQmfReal[n][0] = mHybridReal[n][0];
        mQmfImag[n][0] = mHybridImag[n][0];

        for (k=1; k<6; k++)
        {
            mQmfReal[n][0] += mHybridReal[n][k];
            mQmfImag[n][0] += mHybridImag[n][k];
        }

        mQmfReal[n][1] = mHybridReal[n][6] + mHybridReal[n][7];
        mQmfImag[n][1] = mHybridImag[n][6] + mHybridImag[n][7];

        mQmfReal[n][2] = mHybridReal[n][8] + mHybridReal[n][9];
        mQmfImag[n][2] = mHybridImag[n][8] + mHybridImag[n][9];

        for (k=3; k<nrBands; k++)
        {
            mQmfReal[n][k] = mHybridReal[n][k-3+10];
            mQmfImag[n][k] = mHybridImag[n][k-3+10];
        }
    }
}



void SacDecCloseAnaHybFilterbank()
{
    ;
}

void SacDecCloseSynHybFilterbank()
{
    ;
}



static void sacKChannelFilteringAsym( const float *pQmfReal,
                                      const float *pQmfImag,
                                      float mHybridReal[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS],
                                      float mHybridImag[MAX_HYBRID_ONLY_BANDS_PER_QMF][MAX_TIME_SLOTS],
                                      int nSamples,
                                      int k,
                                      int bCplx,
                                      const float *p,
                                      int pLen,
                                      float offset)
{
    int i, n, q;
    float real, imag;
    float modr, modi;

    for(i = 0; i < nSamples; i++) {
        
        for(q = 0; q < k; q++) {
            real = 0.0;
            imag = 0.0;
            for(n = 0; n < pLen; n++) {
                if (bCplx)
                {
                    modr = (float) cos(PI*(n-offset)/k*(1+2*q));
                    modi = (float) -sin(PI*(n-offset)/k*(1+2*q));
                }
                else{
                    modr = (float) cos(2*PI*q*(n-offset)/k);
                    modi = 0.0;
                }
                real += p[pLen-1-n] * ( pQmfReal[n+i] * modr - pQmfImag[n+i] * modi );
                imag += p[pLen-1-n] * ( pQmfImag[n+i] * modr + pQmfReal[n+i] * modi );
            }
            mHybridReal[q][i] = real;
            mHybridImag[q][i] = imag;
        }
    }
}
