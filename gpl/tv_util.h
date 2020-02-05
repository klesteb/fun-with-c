
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       tv_util.h                                                    */
/*                                                                           */
/* FACILITY:    Global variables and structures for the TV package.          */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the TV utilities.                      */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef TV_UTIL_H
#define TV_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#if defined(VMS)
#    include  <socket.h>                /* VMS "timeval" definitions.   */
#elif defined(VXWORKS)
#    include  <sys/times.h>             /* System time definitions.     */
#else
#    include  <sys/time.h>              /* System time definitions.     */
#endif
/* #include  <rpc/rpc.h> */             /* RPC/XDR definitions.         */

#include  "ansi_setup.h"                /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

typedef  enum  TvUnit {
    TvDecade,
    TvYear,
    TvMonth,
    TvWeek,
    TvDay,
    TvHour,
    TvMinute
}  TvUnit;

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  struct  timeval tv_add P_((struct timeval time1,
				  struct timeval time2)) ;

extern  int  tv_compare P_((struct timeval time1,
			   struct timeval time2)) ;

extern  struct  timeval  tv_create P_((long seconds,
				      long microseconds)) ;

extern  struct  timeval  tv_createf P_((double fSeconds)) ;

extern  double  tv_float P_((struct timeval time)) ;

extern  char  *tv_show P_((struct timeval binaryTime,
			  int inLocal,
			  const char *format)) ;

extern  struct  timeval  tv_subtract P_((struct timeval time1,
					struct timeval time2)) ;

extern  struct  timeval  tv_tod P_((void)) ;

extern  struct  timeval  tv_truncate P_((struct timeval fullTime,
					TvUnit unit)) ;
/*
 * extern  bool_t  xdr_timeval P_((XDR *xdrStream,
 *				struct timeval *time)) ;
*/

#ifdef __cplusplus
    }
#endif

#endif

