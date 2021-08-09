
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       skt_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the SKT package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the SKT utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef SKT_UTIL_H
#define SKT_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  skt_isreadable P_((const char *name,
			       int fd)) ;

extern  int  skt_isup P_((const char *name,
			 int fd)) ;

extern  int  skt_iswriteable P_((const char *name,
				int fd)) ;

extern  char  *skt_peer P_((const char *name,
			   int fd)) ;

extern  int  skt_port P_((const char *name,
			 int fd)) ;

extern  int  skt_setbuf P_((const char *name,
			   int fd,
			   int receiveSize,
			   int sendSize)) ;

#ifdef __cplusplus
    }
#endif

#endif

