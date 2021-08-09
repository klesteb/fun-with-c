
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       lfn_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the LFN package.         */
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

#ifndef LFN_UTIL_H
#define LFN_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */
#include "gpl/tcp_util.h"               /* TCP/IP networking utilities. */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _LfnStream *LfnStream;   /* LFN stream handle.           */

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int lfn_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  lfn_create P_((TcpEndpoint dataPoint,
			   const char *options,
			   LfnStream *stream)) ;

extern  int  lfn_destroy P_((LfnStream stream)) ;

extern  int  lfn_fd P_((LfnStream stream)) ;

extern  int  lfn_getline P_((LfnStream stream,
			    double timeout,
			    char **string)) ;

extern  int  lfn_isreadable P_((LfnStream stream)) ;

extern  int  lfn_isup P_((LfnStream stream)) ;

extern  int  lfn_iswriteable P_((LfnStream stream)) ;

extern  char  *lfn_name P_((LfnStream stream)) ;

extern  int  lfn_putline P_((LfnStream stream,
			    double timeout,
			    const char *format,
			    ...)) ;

extern  int  lfn_read P_((LfnStream stream,
			 double timeout,
			 int numBytesToRead,
			 char *buffer,
			 int *numBytesRead)) ;

extern  int  lfn_write P_((LfnStream stream,
			  double timeout,
			  int numBytesToWrite,
			  const char *buffer,
			  int *numBytesWritten)) ;

#ifdef __cplusplus
    }
#endif

#endif

