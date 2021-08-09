
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       crlf_priv.h                                                  */
/*                                                                           */
/* FACILITY:    Private variables and structures for the CRLF package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the CMD utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef CRLF_PRIV_H
#define CRLF_PRIV_H 1

#include  <limits.h>			    /* Maximum/minimum value definitions.*/
#include  <stdio.h>			        /* Standard I/O definitions.        */
#include  <stdlib.h>			    /* Standard C Library definitions.  */
#include  <string.h>			    /* Standard C string functions.     */
#if !__STDC__ && defined(sun)
#    define  memmove(dest,src,length)  bcopy(src,dest,length)
#endif
#include  "gpl/crlf_util.h"			    /* CR/LF utilities.                 */


extern int crlf_util_debug;         /* Global debug switch (1/0 = yes/no).*/

#endif

