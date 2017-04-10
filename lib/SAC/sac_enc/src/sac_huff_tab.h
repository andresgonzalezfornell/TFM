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


#ifndef __HUFF_TAB_H__
#define __HUFF_TAB_H__


typedef struct {
  unsigned int value[31];
  unsigned int length[31];
} HUFF_CLD_TAB;

typedef struct {
  unsigned int value[8];
  unsigned int length[8];
} HUFF_ICC_TAB;

typedef struct {
  unsigned int value[26];
  unsigned int length[26];
} HUFF_CPC_TAB;


typedef struct {

  HUFF_CLD_TAB huffPt0;
  HUFF_CLD_TAB huffDiff[2];

} HUFF_CLD_TABLE;

typedef struct {

  HUFF_ICC_TAB huffPt0;
  HUFF_ICC_TAB huffDiff[2];

} HUFF_ICC_TABLE;

typedef struct {

  HUFF_CPC_TAB huffPt0;
  HUFF_CPC_TAB huffDiff[2];

} HUFF_CPC_TABLE;

#endif
