/*
@(#)  FILE: lfn_util.h  RELEASE: 1.2  DATE: 07/12/96, 15:44:48
*/
/*******************************************************************************

    lfn_util.h

    Line Feed-Terminated Networking Utility Definitions.

*******************************************************************************/

#ifndef  LFN_UTIL_H             /* Has the file been INCLUDE'd already? */
#define  LFN_UTIL_H  yes

#ifdef __cplusplus
    extern  "C" {
#endif


#include  "ansi_setup.h"                /* ANSI or non-ANSI C? */
#include  "tcp_util.h"                  /* TCP/IP networking utilities. */


/*******************************************************************************
    LFN Structures (Client View) and Definitions.
*******************************************************************************/

typedef  struct  _LfnStream  *LfnStream ;       /* LFN stream handle. */


/*******************************************************************************
    Miscellaneous declarations.
*******************************************************************************/

extern  int  lfn_util_debug ;           /* Global debug switch (1/0 = yes/no). */


/*******************************************************************************
    Public functions.
*******************************************************************************/

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

#endif                          /* If this file was not INCLUDE'd previously. */
