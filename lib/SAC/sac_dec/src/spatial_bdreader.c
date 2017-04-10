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
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <libtsp/AFpar.h>

#include "spatial_bdreader.h"

#define MIN(x,y)   (((x) < (y)) ? (x) : (y))
#define MAX(x,y)   (((x) > (y)) ? (x) : (y))
#define sqr(x)    ((x)*(x))        

#define CRC16_POLYNOMIAL    0x8005   
#define CRC8_POLYNOMIAL     0x0007       
#define SYNC_PATTERN        0xAA95

#define SUCCES                   0
#define NO_SYNC_FOUND            1
#define CRC8_ERROR               2
#define CRC16_ERROR              3
#define CONFIG_ERROR             4
#define LENGTH_ERROR             5
#define UNSUPPORTED              6

#define MAX_CHANNELS            16
#define MAX_FRMSIZE           8192
#define MAX_SUBFRAMES           16

#define PCM_BLOCK_SIZE          64

#define PRINT fprintf
#define SE stderr

struct BURIED_DATA_READER {
  HANDLE_BYTE_READER byte_reader;
  int                stream_id;
  int                block_size;
  int                read_ssc;
  int                bit_alloc[MAX_SUBFRAMES][MAX_CHANNELS];
  unsigned char      databuf[2*MAX_CHANNELS*MAX_FRMSIZE];
  int                data_bytes;
  int                read_offset;
};

static unsigned char alloc_mask[9] = { 0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff };
static unsigned char empty_ext[5] = { 0x7f, 0xfd, 0x03, 0xe0, 0x0a };

static int get_sample (BD_INPUT_BUFFER *inputbuf, int sample, int channel)
{
  float scaleFactor = 32768.0f;

  if (inputbuf->sampleFormat == FD_INT24)
  {
    if (inputbuf->bitsPerSample == 20)
    {
      
      scaleFactor = 524288.0f;
    }
    else
    {
      
      scaleFactor = 8388608.0f;
    }
  }

  return (int)(inputbuf->data[inputbuf->channels * sample + channel] * scaleFactor);
}

static int getbits (unsigned char *buffer, int bits, int *byte_ptr, int *bit_ptr)
{
  int bit, nr = bits, out = 0;

  while (nr > 0)
  {
    nr--;
    bit = ((int) buffer[*byte_ptr] & (1 << *bit_ptr)) >> *bit_ptr;
    out += bit << nr;
    if (*bit_ptr == 0)
    {
      (*byte_ptr)++;
      *bit_ptr = 7;
    }
    else
    {
      (*bit_ptr)--;
    }
  }

  return (out);
} 

static void putbits (int data, unsigned char *buffer, int bits, int *byte_ptr, int *bit_ptr)
{
  int bit, nr = bits;

  while (nr > 0)
  {
    nr--;
    bit = (data & (1 << nr)) >> nr;
    if (*bit_ptr == 7)
      buffer[*byte_ptr] = (unsigned char) (bit << *bit_ptr);
    else
      buffer[*byte_ptr] = (unsigned char) (buffer[*byte_ptr] + (bit << *bit_ptr));
    if (*bit_ptr == 0)
    {
      (*byte_ptr)++;
      *bit_ptr = 7;
    }
    else
    {
      (*bit_ptr)--;
    }
  }
} 


static void update_CRC8 (unsigned int data, unsigned int length,
              unsigned int *crc)
{
  unsigned int masking, carry;

  masking = 1 << length;

  while ((masking >>= 1))
  {
    carry = *crc & 0x80;
    *crc <<= 1;
    if (!carry ^ !(data & masking))
      *crc ^= CRC8_POLYNOMIAL;
  }
  *crc &= 0xff;
}

static void update_CRC16 (unsigned int data, unsigned int length,
               unsigned int *crc)
{
  unsigned int masking, carry;

  masking = 1 << length;

  while ((masking >>= 1))
  {
    carry = *crc & 0x8000;
    *crc <<= 1;
    if (!carry ^ !(data & masking))
      *crc ^= CRC16_POLYNOMIAL;
  }
  *crc &= 0xffff;
}

static unsigned int derand_byte (unsigned int data, unsigned int *state)
{
  unsigned int x1, x3, x14, x16, result;

  *state = (*state << 8) + data;
  x1  = *state >> 1;
  x3  = *state >> 3;
  x14 = *state >> 14;
  x16 = *state >> 16;
  result = (data ^ x1 ^ x3 ^ x14 ^ x16) & 0xff;
  *state &= 0xffff;
  return (result);
}

static int calc_Hbytes (int channels, int subframes)
{
  int Hbits = 40 + subframes * channels * 4;
  int Hsize = (Hbits + channels - 1) / channels;
  int Hbytes = ((Hsize + 7) / 8);

  Hbytes = MAX (Hbytes, 4);

  return (Hbytes);
}

static int get_frame_size (BD_INPUT_BUFFER *inputbuf,
                           int             *frmsize)
{
  int j;
  unsigned char databuf[4];
  int byte_ptr, bit_ptr;
  unsigned int rnd_state = 0xffff;

  
  byte_ptr = 0;
  bit_ptr = 7;
  for (j = 0; j < 32; j++)
  {
    putbits (get_sample(inputbuf, j, 0), databuf, 1, &byte_ptr, &bit_ptr);
  }
  if ((((int)databuf[0] << 8) + databuf[1]) != SYNC_PATTERN)
      return (NO_SYNC_FOUND);
  for (j = 2; j < 4; j++)
  {
    databuf[j] = (unsigned char) derand_byte (databuf[j], &rnd_state);
  }
  byte_ptr = 2;
  bit_ptr = 3;
  *frmsize = getbits (databuf, 7, &byte_ptr, &bit_ptr) + 1;
  return (SUCCES);
}

static int decode_block (BD_INPUT_BUFFER *inputbuf,
                         unsigned char   databuf[2*MAX_CHANNELS*MAX_FRMSIZE],
                         int             *data_bytes,
                         int             bit_alloc[MAX_SUBFRAMES][MAX_CHANNELS],
                         unsigned int    *rnd_state,
                         int             *block,
                         int             *frmsize,
                         int             *subframes,
                         int             *channels)
{
  int i, j, blk, ch;
  unsigned int sync, crc8_result, crc8_state;
  unsigned char header[2];
  int byte_ptr, bit_ptr;
  int dummy, Hbits, Hbytes, Hsize, padding_bits;
  int bsChannels, bsFramelength, bsSubframes, frm_blocks, pcm_blocks;

  if (*block == 0)
  {
    int pcm_block_size = PCM_BLOCK_SIZE;
    crc8_state = 0xff;
    
    byte_ptr = 0;
    bit_ptr = 7;
    for (j = 0; j < 32; j++)
    {
      putbits (get_sample(inputbuf, j, 0), databuf, 1, &byte_ptr, &bit_ptr);
    }
    sync = databuf[0];
    sync = (sync << 8) + databuf[1];
    if (sync != SYNC_PATTERN)
        return (NO_SYNC_FOUND);
    for (j = 2; j < 4; j++)
    {
      databuf[j] = (unsigned char) derand_byte (databuf[j], rnd_state);
      header[j-2] = databuf[j];
    }
    byte_ptr = 2;
    bit_ptr = 7;
    
    bsChannels = getbits (databuf, 4, &byte_ptr, &bit_ptr);
    *channels = bsChannels + 1;
    bsFramelength = getbits (databuf, 7, &byte_ptr, &bit_ptr);
    frm_blocks = bsFramelength + 1;
    *frmsize = frm_blocks * PCM_BLOCK_SIZE;
    bsSubframes = getbits (databuf, 4, &byte_ptr, &bit_ptr);
    *subframes = bsSubframes + 1;
    pcm_blocks = frm_blocks / *subframes;


    dummy = getbits (databuf, 1, &byte_ptr, &bit_ptr);
    update_CRC8 (bsChannels, 4, &crc8_state);
    update_CRC8 (bsFramelength, 7, &crc8_state);
    update_CRC8 (bsSubframes, 4, &crc8_state);
    update_CRC8 (dummy, 1, &crc8_state);
    if (*channels > MAX_CHANNELS)
    {
      return (CONFIG_ERROR);
    }

    Hbits = 40 + *subframes * *channels * 4;
    Hbytes = calc_Hbytes (*channels, *subframes);
    Hsize = Hbytes * *channels;
    padding_bits = Hsize * 8 - Hbits;
    if (Hbytes > 8)
    {
      pcm_block_size *= 2;
      (*block)++;
    }

    
    byte_ptr = 0;
    bit_ptr = 7;
    for (ch = 0; ch < *channels; ch++)
    {
      for (j = 0; j < Hbytes*8; j++)
      {
        putbits (get_sample(inputbuf, j, ch), databuf, 1, &byte_ptr, &bit_ptr);
      }
    }

    databuf[2] = header[0];
    databuf[3] = header[1];
    for (j = 4; j < Hsize; j++)
    {
      databuf[j] = (unsigned char) derand_byte (databuf[j], rnd_state);
    }
    byte_ptr = 4;
    bit_ptr = 7;
    for (blk = 0; blk < *subframes; blk++)
    {
      for (ch = 0; ch < *channels; ch++)
      {
        bit_alloc[blk][ch] = getbits (databuf, 4, &byte_ptr, &bit_ptr);
        update_CRC8 (bit_alloc[blk][ch], 4, &crc8_state);
      }
    }
    crc8_result = getbits (databuf, 8, &byte_ptr, &bit_ptr);
    for (i=0; i<padding_bits; i++)
    {
      getbits (databuf, 1, &byte_ptr, &bit_ptr);
    }
    if (crc8_result != crc8_state)
    {
        return (CRC8_ERROR);
    }
    
    for (j = 0; j < Hbytes*8; j++)
    {
      for (ch = 0; ch < *channels; ch++)
      {
        if (bit_alloc[0][ch] > 1)
        {
          putbits (get_sample(inputbuf, j, ch) >> 1, databuf, bit_alloc[0][ch]-1, &byte_ptr, &bit_ptr);
        }
      }
    }
    
    for (j = Hbytes*8; j < pcm_block_size; j++)
    {
      for (ch = 0; ch < *channels; ch++)
      {
        if (bit_alloc[0][ch] > 0)
        {
          putbits (get_sample(inputbuf, j, ch), databuf, bit_alloc[0][ch], &byte_ptr, &bit_ptr);
        }
      }
    }
    
    for (i = Hsize; i < byte_ptr; i++)
    {
      databuf[i] = (unsigned char) derand_byte (databuf[i], rnd_state);
    }
    *data_bytes = byte_ptr;
    
    if (*data_bytes > Hsize)
    {
      for (i = Hsize; i < *data_bytes; i++)
      {
        databuf[i-Hsize] = databuf[i];
      }
      *data_bytes -= Hsize;
    }
    else
    {
      *data_bytes = 0;
    }
  }
  else
  {
    pcm_blocks = (*frmsize / PCM_BLOCK_SIZE) / *subframes;

    
    byte_ptr = *data_bytes;
    bit_ptr = 7;
    for (j = 0; j < PCM_BLOCK_SIZE; j++)
    {
      for (ch = 0; ch < *channels; ch++)
      {
        putbits (get_sample(inputbuf, *block * PCM_BLOCK_SIZE + j, ch), databuf, bit_alloc[*block/pcm_blocks][ch], &byte_ptr, &bit_ptr);
      }
    }
    
    for (i = *data_bytes; i < byte_ptr; i++)
    {
      databuf[i] = (unsigned char) derand_byte (databuf[i], rnd_state);
    }
    *data_bytes = byte_ptr;
  }
  (*block)++;
  return (SUCCES);
}

static int parse_block (unsigned char   databuf[2*MAX_CHANNELS*MAX_FRMSIZE],
                        int             *data_bytes,
                        int             selected_id,
                        int             selected_type_mask,
                        int             *read_ssc)
{
  unsigned int      crc_state, crc_result;
  int i, index_in, index_out, stream_type, stream_id, length_idx, stream_length;

  index_in = index_out = 0;
  while (*data_bytes - index_in >= 3)
  {
    stream_type = databuf[index_in] >> 5;
    stream_id = (databuf[index_in] >> 2) & 0x07;
    length_idx = databuf[index_in++] & 0x03;
    crc_state = 0xffff;
    for (i = 0; i < 1+length_idx; i++)
    {
      update_CRC16 (databuf[index_in+i-1], 8, &crc_state);
    }
    stream_length = 0;
    while (length_idx > 0)
    {
      stream_length <<= 8;
      stream_length += databuf[index_in++];
      length_idx--;
    }
    if (index_in + stream_length + 2 > *data_bytes)
    {
      return (LENGTH_ERROR);
    }
    if ((stream_id == selected_id) && ((selected_type_mask & (1 << stream_type)) != 0))
    {
      int offset = 0;
      
      if (stream_type == 1)
      {
        int header_len = (databuf[index_in] & 0x7F) + 1;
        if ((databuf[index_in] & 0x80) != 0) {
          PRINT(SE, "The sacTimeAlignFlag is not implemented\n");
          return (UNSUPPORTED);
        }
        if (header_len == 128)
        {
          header_len += ((databuf[index_in+1] << 8) | databuf[index_in+2]) + 2;
        }
        if (*read_ssc)
        {
          
          *read_ssc = 0;
        }
        else
        {
          
          offset = header_len;
        }
      }
      for (i = 0; i < stream_length; i++)
      {
        if (i >= offset)
        {
          databuf[index_out++] = databuf[index_in];
        }
        update_CRC16 (databuf[index_in++], 8, &crc_state);
      }
    }
    else
    {
      for (i = 0; i < stream_length; i++)
      {
        update_CRC16 (databuf[index_in++], 8, &crc_state);
      }
    }
    crc_result = (((unsigned int) databuf[index_in]) << 8) + databuf[index_in+1];
    index_in += 2;
    if (crc_state != crc_result)
    {
      *data_bytes = 0;
      return (CRC16_ERROR);
    }
  }
  *data_bytes = index_out;
  return (SUCCES);
}


static int ReadByte(HANDLE_BYTE_READER byte_reader)
{
  HANDLE_BURIED_DATA_READER self = *(HANDLE_BURIED_DATA_READER*)&byte_reader[1];

  return (self->read_offset < self->data_bytes)? self->databuf[self->read_offset++]: -1;
}


HANDLE_BURIED_DATA_READER BdReaderOpen(int blockSize)
{
  int error = 0;
  HANDLE_BURIED_DATA_READER self = calloc(1, sizeof(*self));
  if (self == NULL) error = 1;

  if (!error)
  {
    
    self->byte_reader = calloc(1, sizeof(*self->byte_reader) + sizeof(self));
    if (self->byte_reader == NULL) error = 1;
  }

  if (!error)
  {
    self->byte_reader->ReadByte = ReadByte;
    *(HANDLE_BURIED_DATA_READER*)&self->byte_reader[1] = self;

    self->block_size = blockSize;
    self->read_ssc = 1;

    if (self->block_size != PCM_BLOCK_SIZE)
    {
      PRINT(SE, "The only supported buried data block size is %d\n", PCM_BLOCK_SIZE);
      error = 1;
    }
  }
  
  if (error)
  {
    BdReaderClose(self);
    self = NULL;
  }

  return self;
}


void BdReaderClose(HANDLE_BURIED_DATA_READER self)
{
  if (self != NULL)
  {
    if (self->byte_reader != NULL) free(self->byte_reader);
    free(self);
  }
}


int BdReaderGetFrameSize(HANDLE_BURIED_DATA_READER self,
                         BD_INPUT_BUFFER *inputBuffer,
                         int *frameSize)
{
  (self);
  return get_frame_size (inputBuffer, frameSize);
}


int BdReaderParseFrame(HANDLE_BURIED_DATA_READER self,
                       BD_INPUT_BUFFER *inputBuffer)
{
  int status = SUCCES;
  unsigned int rnd_state = 0xffff;
  int block = 0;
  int frmsize = PCM_BLOCK_SIZE;
  int subframes;
  int channels;

  self->data_bytes = 0;
  self->read_offset = 0;

  while ((status == SUCCES) && (block < (frmsize / PCM_BLOCK_SIZE)))
  {
    status = decode_block (inputBuffer, self->databuf, &self->data_bytes, self->bit_alloc,
                           &rnd_state, &block, &frmsize, &subframes, &channels);
  }

  if (status == SUCCES) 
  {
    status = parse_block (self->databuf, &self->data_bytes, self->stream_id, 0x3, &self->read_ssc);
  }

  if (status != SUCCES)
  {
    self->data_bytes = 0;
  }

  return status;
}


int BdReaderEndOfStream(HANDLE_BURIED_DATA_READER self)
{
  return (self->data_bytes - self->read_offset) <= 0;
}


HANDLE_BYTE_READER BdReaderGetByteReader(HANDLE_BURIED_DATA_READER self)
{
  return self->byte_reader;
}
