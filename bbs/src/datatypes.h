
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

#ifndef _DATATYPES_H
#define _DATATYPES_H

#if !defined(linux)
#if !(((defined(__FreeBSD__) && __FreeBSD_version >= 440000)) || defined(NeXTBSD))
typedef unsigned short ushort;   /* must be 16 bits wide */
#endif
typedef unsigned long  ulong;    /* must be 32 bits wide */
#endif
typedef unsigned char  uchar;    /* must be  8 bits wide */
typedef unsigned int   uint;

#endif

