
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       xqt_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the XQT package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the XQT utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef XQT_UTIL_H
#define XQT_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _XqtStream *XqtStream;	/* Execution stream handle.     */

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int xqt_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  xqt_close P_((XqtStream stream));

extern  int  xqt_fd P_((XqtStream stream));

extern  int  xqt_open P_((const char *shell,
                         XqtStream *stream));

extern  int  xqt_poll P_((XqtStream stream));

extern  int  xqt_read P_((XqtStream stream,
                         int maxLength,
                         char *buffer));

extern  int  xqt_write P_((XqtStream stream,
                          const char *format,
                          ...));

#ifdef __cplusplus
    }
#endif

#endif

