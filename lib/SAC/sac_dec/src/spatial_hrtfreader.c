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
#include <ctype.h>
#include <stdlib.h>

#ifdef HRTF_DYNAMIC_UPDATE

#include "spatial_hrtfreader.h"

#define PRINT fprintf
#define SE stderr

#define MAX_STRING_LENGTH 65536

#define PARAMETRIC 1
#define FILTERING  2

void myexit(char *s);

struct HRTF_READER {
  HRTF_SOURCE hrtfSource;
  FILE *file;
  long fileSize;
  char *buffer;
  int mode;
  SPATIAL_HRTF_DATA hrtfData;
};



static int iseol(int c)
{
    return (strchr( "\r\n", c ) != NULL);
}


static char *ReadLine(HANDLE_HRTF_READER self)
{
  int shift = 0;
  int length;

  fgets(self->buffer, MAX_STRING_LENGTH, self->file);

  length = strlen(self->buffer);

  if (!iseol(self->buffer[length - 1]) && !feof(self->file)) {
    myexit("Line is too long.");
  }

  while ((length > 0) && (isspace(self->buffer[length - 1]))) length--;

  self->buffer[length] = '\0';

  while ((self->buffer[shift] != '\0') && isspace(self->buffer[shift])) shift++;

  return &self->buffer[shift];
}


static char *GetValue(char *line)
{
  char *pos = strchr(line, '=');

  while ((*(++pos) != '\0') && isspace(*pos));

  return pos;
}


static void ReadHeader(HANDLE_HRTF_READER self)
{
  int started = 0;
  int done = 0;

  while (!done && !feof(self->file)) {
    char *line = ReadLine(self);

    if (*line == '\0') {
      if (started) {
        done = 1;
      }
    }
    else {
      if (strcmp(line, "[header]") == 0){
        started = 1;
      } 
      else if (strncmp(line, "sampleRate", 10) == 0) {
        self->hrtfData.sampleRate = strtol(GetValue(line), NULL, 10);
      }
      else if (strncmp(line, "mode", 4) == 0) {
        line = GetValue(line);
        if (strcmp(line, "parametric") == 0){
          self->mode = PARAMETRIC;
        } 
        else if (strcmp(line, "filtering") == 0) {
          self->mode = FILTERING;
        }
      }
      else if (strncmp(line, "azimuths", 8) == 0) {
        self->hrtfData.azimuths = strtol(GetValue(line), NULL, 10);
      }
      else if (strncmp(line, "parameterBands", 14) == 0) {
        self->hrtfData.parameterBands = strtol(GetValue(line), NULL, 10);
      }
    }
  }
}


static char *ReadVector(char *line,
                        int length,
                        float *data)
{
  char *pos = line;
  int i;

  for (i = 0; i < length; i++) {
    data[i] = (float) strtod(pos, &pos);
  }

  return pos;
}


static void ReadAzimuth(HANDLE_HRTF_READER self,
                        int azimuth)
{
  int done = 0;

  while (!done && !feof(self->file)) {
    char *line = ReadLine(self);

    if (*line == '\0') {
      done = 1;
    }
    else {
      if (strncmp(line, "powerLeft", 9) == 0){
        ReadVector(GetValue(line), self->hrtfData.parameterBands, self->hrtfData.directional[azimuth].powerLeft);
      } 
      else if (strncmp(line, "powerRight", 10) == 0) {
        ReadVector(GetValue(line), self->hrtfData.parameterBands, self->hrtfData.directional[azimuth].powerRight);
      }
      else if (strncmp(line, "icc", 3) == 0) {
        ReadVector(GetValue(line), self->hrtfData.parameterBands, self->hrtfData.directional[azimuth].icc);
      }
      else if (strncmp(line, "ipd", 3) == 0) {
        ReadVector(GetValue(line), self->hrtfData.parameterBands, self->hrtfData.directional[azimuth].ipd);
      }
    }
  }
}


static void ReadParamSet(HANDLE_HRTF_READER self)
{
  int started = 0;
  int done = 0;
  int azimuth = 0;

  while (!done && !feof(self->file)) {
    char *line = ReadLine(self);

    if (*line == '\0') {
      if (started) {
        done = 1;
      }
    }
    else {
      if (strncmp(line, "[paramSet", 9) == 0){
        started = 1;
      } 
      else if (strncmp(line, "[azimuth", 8) == 0) {
        ReadAzimuth(self, azimuth++);
      }
    }
  }
}


static SPATIAL_HRTF_DATA *GetHRTF(HANDLE_HRTF_SOURCE hrtfSource)
{
  HANDLE_HRTF_READER self = (HANDLE_HRTF_READER) hrtfSource;

  ReadParamSet(self);

  return &self->hrtfData;
}


HANDLE_HRTF_READER HrtfReaderOpen(char *fileName)
{
  int error = 0;
  int az;

  HANDLE_HRTF_READER self = calloc(1, sizeof(*self));
  if (self == NULL) error = 1;

  if (!error) {
    self->buffer = calloc(MAX_STRING_LENGTH, 1);
    if (self->buffer == NULL) error = 1;
  }

  if (!error) {
    self->hrtfSource.GetHRTF = GetHRTF;

    if ((self->file = fopen(fileName, "rt")) == NULL) {
      PRINT(SE, "Error opening %s\n", fileName);
      error = 1;
    }
  }

  if (!error) {
    fseek(self->file, 0, SEEK_END);
    self->fileSize = ftell(self->file);
    fseek(self->file, 0, SEEK_SET);
  }

  if (!error) {
    ReadHeader(self);

    self->hrtfData.directional = calloc(self->hrtfData.azimuths, sizeof(SPATIAL_HRTF_DIRECTIONAL_DATA));
    if (self->hrtfData.directional == NULL) error = 1;

    for (az = 0; !error && (az < self->hrtfData.azimuths); az++) {
      self->hrtfData.directional[az].powerLeft  = calloc(self->hrtfData.parameterBands, sizeof(float));
      self->hrtfData.directional[az].powerRight = calloc(self->hrtfData.parameterBands, sizeof(float));
      self->hrtfData.directional[az].icc        = calloc(self->hrtfData.parameterBands, sizeof(float));
      self->hrtfData.directional[az].ipd        = calloc(self->hrtfData.parameterBands, sizeof(float));

      if ((self->hrtfData.directional[az].powerLeft == NULL) ||
          (self->hrtfData.directional[az].powerRight == NULL) ||
          (self->hrtfData.directional[az].icc == NULL) ||
          (self->hrtfData.directional[az].ipd == NULL))
      {
        error = 1;
      }
    }
  }

  if (error) {
    HrtfReaderClose(self);
    self = NULL;
  }

  return self;
}


void HrtfReaderClose(HANDLE_HRTF_READER self)
{
  int az;

  if (self != NULL) {
    if (self->hrtfData.directional != NULL) {
      for (az = 0; az < self->hrtfData.azimuths; az++) {
        free(self->hrtfData.directional[az].powerLeft);
        free(self->hrtfData.directional[az].powerRight);
        free(self->hrtfData.directional[az].icc);
        free(self->hrtfData.directional[az].ipd);
      }
      free(self->hrtfData.directional);
    }

    if (self->file != NULL) fclose(self->file);

    free(self->buffer);
    free(self);
  }
}


int HrtfReaderEof(HANDLE_HRTF_READER self)
{
  return ftell(self->file) == self->fileSize;
}


HANDLE_HRTF_SOURCE HrtfReaderGetHrtfSource(HANDLE_HRTF_READER self)
{
  return (HANDLE_HRTF_SOURCE) self;
}

#endif 
