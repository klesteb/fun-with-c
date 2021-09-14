
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

/* Create some new data types. Mainly for easier reading. */

/* long live the AMIGA! */

typedef char               BYTE;
typedef unsigned char      UBYTE;
typedef short int          WORD;
typedef unsigned short int UWORD;
typedef long               LONG;
typedef unsigned long      ULONG;
typedef unsigned char *    STRPTR;

/* #if !defined(linux) */
/* #if !(((defined(__FreeBSD__) && __FreeBSD_version >= 440000)) || defined(NeXTBSD)) */
/* typedef unsigned short      ushort; */  /* must be 16 bits wide */
/* #endif */
/* typedef unsigned long       ulong; */   /* must be 32 bits wide */
/* #endif */

typedef unsigned short      ushort;   /* must be 16 bits wide */
typedef unsigned long       ulong;    /* must be 32 bits wide */
typedef unsigned char       uchar;    /* must be  8 bits wide */
typedef unsigned int        uint;
typedef unsigned long long	uid;

#ifndef unix
typedef unsigned long long	off64_t;
#endif

#endif

