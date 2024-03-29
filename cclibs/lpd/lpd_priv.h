
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       lpd_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the LPD package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the LPD utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef LPD_PRIV_H
#define LPD_PRIV_H 1

#define  _BSD_SOURCE  1

#include <errno.h>                  /* System error definitions.        */
#include <limits.h>                 /* Maximum/minimum value definitions.*/
#include <stdio.h>                  /* Standard I/O definitions.        */
#include <stdlib.h>                 /* Standard C Library definitions.  */
#include <string.h>                 /* C Library string functions.      */

#include "gpl/vperror.h"
#include "gpl/tcp_util.h"           /* TCP/IP networking utilities.     */
#include "gpl/lfn_util.h"           /* LFN network.                     */
#include "cclibs/lpd_util.h"        /* LPD definations.                 */

/*----------------------------------------------------------------------*/
/* LPD Handle - contains information for the LPD routines.              */
/*----------------------------------------------------------------------*/

typedef  struct  _LpdHandle {
	LfnStream stream;               /* Underlaying LFN data stream.     */
	double timeout;                 /* Timeout for network operations.  */
}  _LpdHandle ;

extern int lpd_util_debug;          /* Global debug switch (1/0 = yes/no).*/

#define LPD_K_BUFSIZ 1024           /* Input buffer size of 1024 bytes  */

#endif

