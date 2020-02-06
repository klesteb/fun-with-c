
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       gsc.h                                                        */
/*                                                                           */
/* FACILITY:    Global variables and structures for the GSC package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the GSC utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      20-Nov-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef GSC_H
#define GSC_H 1

/*----------------------------------------------------------------------*/
/*    GSC Structures and Definitions.                                   */
/*----------------------------------------------------------------------*/

typedef struct _GblSec{
    WORD chan;
    ULONG lock_id;
    ULONG active;
    ULONG sec_address[2];
	ULONG status;
    char sec_name[32];
    ECB ecb;
} GblSec;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

int   gsc_open(char *filename, char *sec_name, int sec_size, GblSec *sec);
int   gsc_flush(GblSec *sec);
int   gsc_close(GblSec *sec);
int   gsc_lock(GblSec *sec);
int   gsc_unlock(GblSec *sec);
void *gsc_associate(GblSec *sec);

#endif

