/*-------------- Telecommunications & Signal Processing Lab ---------------
                             McGill University

Routine:
  int STdecDfrac (const char String[], double *Dval1, double *Dval2)

Purpose:
  Decode a ratio specification of two double values

Description:
  This routine decodes a string specifying a ratio of double values.  The
  ratio is specified in the form "V" or "V1/V2", for example "-23.1 / 45".
  Optional white-space (as defined by isspace) can surround the values.  For
  the case of a single value V, this is equivalent to the ratio "V/1".  If an
  error is encountered, neither output value is set.

Parameters:
  <-  int STdecRfrac
      Error status, 0 for no error, 1 for error
   -> const char String[]
      Input string
  <-  double *Dval1
      First value
  <-  double *Dval2
      Second value

Author / revision:
  P. Kabal  Copyright (C) 2003
  $Revision: 1.7 $  $Date: 2003/05/09 03:02:44 $

-------------------------------------------------------------------------*/

#include <libtsp.h>
#include <libtsp/nucleus.h>
#include <libtsp/STmsg.h>

#define MAXC	23
#define DP_EMPTY		0
#define DP_LVAL			1
#define DP_DELIM		2
#define DP_RVAL			4	


int
STdecDfrac (const char String[], double *Dval1, double *Dval2)

{
  int status;
  double dval1, dval2;

  /* Decode the ratio values */
  status = STdecPair (String, "/", 'D', (void *) (&dval1), (void *) (&dval2));

  if (status == DP_LVAL) {
    *Dval1 = dval1;
    *Dval2 = 1.0;
  }
  else if (status == (DP_LVAL + DP_DELIM + DP_RVAL)) {
    *Dval1 = dval1;
    *Dval2 = dval2;
  }
  else if (status >= 0) {
    UTwarn ("STdecDfrac - %s: \"%s\"", STM_DataErr, STstrDots (String, MAXC));
    status = -1;
  }

  return (status < 0);
}
