
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _MBF_H
#define _MBF_H

/*----------------------------------------------------------------*/
/* data structures                                                */
/*----------------------------------------------------------------*/

typedef struct {        /* msbin float structure                  */
   char m1;
   char m2;
   char m3;
   char exp;
} msbin;

/*----------------------------------------------------------------*/
/* interface                                                      */
/*----------------------------------------------------------------*/

extern void LongToMbf(long, msbin *);
extern void MbfToLong(msbin *, long *);

#endif

