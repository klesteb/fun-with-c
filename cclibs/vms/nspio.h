
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2001 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       nspio.h                                                      */
/*                                                                           */
/* FACILITY:    Global variables and structures for the NSPIO package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the NSPIO functions.                   */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      01-Oct-2001 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef NSPIO_H
#define NSPIO_H

/*----------------------------------------------------------------------*/
/*    NSPIO Structures and Definitions.                                 */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

#define nsp_lun(x) ((x)->lun)
#define nsp_ef(x)  ((x)->ef)

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

IFILE *nsp_open(char *, char *, char *, char *, int);
int    nsp_write(IFILE *, void *, int, int);
int    nsp_read(IFILE *, void *, int);
void   nsp_close(IFILE *);

#endif

