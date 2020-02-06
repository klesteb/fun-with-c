
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       latio.h                                                      */
/*                                                                           */
/* FACILITY:    Global variables and structures for the LATIO package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the GSC utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      30Jul-1992  */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef LATIO
#define LATIO 1

typedef struct {                        /* set mode buffer              */
    unsigned char class;                /* device class - dcdef.h       */
    unsigned char type;                 /* term type - ttdef.h          */
    union {
        unsigned long basic;            /* basic term characteristics   */
        unsigned char other[4];         /* remap term characteristics   */
    } map;
    unsigned long extended;             /* extended term characteristics*/
} mode;

typedef struct _latio {
	mode mode;
	IOSB iosb;
	UWORD channel;
} latio_t;

latio_t *lat_open(char *);                       /* open lat channel             */
void  lat_close(latio_t *);                      /* close lat channel            */
int   lat_read(latio_t *, char *, UWORD, UWORD); /* read data from lat channel   */
int   lat_write(latio_t *, char *, UWORD);       /* write data to lat channel    */

#endif

/* end of latio.h */

