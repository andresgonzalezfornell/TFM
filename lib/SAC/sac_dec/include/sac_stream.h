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

#ifndef __SAC_STREAM_H__
#define __SAC_STREAM_H__

#ifdef __cplusplus
extern "C"
{
#endif



#include <stdio.h>


typedef enum {

  STREAM_READ  = 0x0,
  STREAM_WRITE = 0x1

} STREAM_TYPE;


typedef struct {

  FILE*         p_file;
  unsigned char nextByte;
  int           bitsInBuffer;
  STREAM_TYPE   streamType;

} Stream;


void InitStream( Stream*, char*, STREAM_TYPE );
int ReadBits( Stream*, int, unsigned long* );
void WriteBits( Stream*, unsigned int, int );
void CloseStream( Stream* );
int  GetBitsInStream( Stream* );
void ByteAlignWrite( Stream* strm );
void ByteAlignRead( Stream* strm );


#ifdef __cplusplus
}
#endif


#endif
