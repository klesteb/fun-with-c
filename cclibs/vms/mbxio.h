
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
/* TITLE:       mbxio.h                                                      */
/*                                                                           */
/* FACILITY:    Global variables and structures for the MBXIO package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the MBXIO functions.                   */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      02-Sep-1999 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef MBXIO_H
#define MBXIO_H

/*----------------------------------------------------------------------*/
/*    MBXIO Structures and Definitions.                                 */
/*----------------------------------------------------------------------*/

enum {                    /* Access modes                 */
    mbx_a_read_only = 1,
    mbx_a_write_only,
    mbx_a_both
} mbx_acc_mode;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

#define mbx_lun(x) ((x)->lun)
#define mbx_ef(x)  ((x)->ef)

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

IFILE *mbx_open(char *, ULONG, ULONG, ULONG, ULONG);
int    mbx_read(IFILE *, void *, int);
int    mbx_write(IFILE *, void *, int);
void   mbx_close(IFILE *);
char  *mbx_name(IFILE *);
int    mbx_count(IFILE *);
void   mbx_eof(IFILE *);

#endif

