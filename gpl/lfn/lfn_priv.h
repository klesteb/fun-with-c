
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       lfn_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the LFN package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the LFN utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef LFN_PRIV_H
#define LFN_PRIV_H 1

#define  _BSD_SOURCE  1

#include  <errno.h>                     /* System error definitions. */
#include  <limits.h>                    /* Maximum/minimum value definitions. */
#if __STDC__
#    include  <stdarg.h>                /* Variable-length argument lists. */
#else
#    include  <varargs.h>               /* Variable-length argument lists. */
#endif
#include  <stdio.h>                     /* Standard I/O definitions. */
#include  <stdlib.h>                    /* Standard C Library definitions. */
#include  <string.h>                    /* C Library string functions. */
#include  "opt_util.h"                  /* Option scanning definitions. */
#include  "str_util.h"                  /* String manipulation functions. */
#include  "tcp_util.h"                  /* TCP/IP networking utilities. */
#include  "tv_util.h"                   /* "timeval" manipulation functions. */
#include  "vperror.h"                   /* VPERROR() definitions. */
#include  "lfn_util.h"                  /* LF-terminated network I/O. */
#include  "meo_util.h"                  /* memory operations */

/*----------------------------------------------------------------------*/
/* LFN Stream - contains information about a LF-terminated, data stream,*/
/* including the underlying network connection, the input buffer, and   */
/* other attributes.                                                    */
/*----------------------------------------------------------------------*/

typedef  struct  _LfnStream {
    TcpEndpoint  connection ;           /* TCP/IP connection. */
    int  bufferSize ;                   /* Size of input buffer. */
    int  nextChar ;                     /* Index of next character in buffer. */
    int  lastChar ;                     /* Index of last character in buffer. */
    char  *inputBuffer ;                /* Buffered input. */
    int  maxLength ;                    /* Maximum message length. */
    char  *inputString ;                /* Last string read. */
    char  *outputString ;               /* Formatted string to be output. */
    int  debug ;                        /* Debug switch (1/0 = yes/no). */
}  _LfnStream ;

#define  INPUT_BUFFER_SIZE  2048
#define  MAX_STRING_LENGTH  2047

#endif

