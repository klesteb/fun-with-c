
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       XXX_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the XXX package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the XXX utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef NIX_UTIL_H
#define NIX_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/* Network I/O Handler Structures (Client View) and Definitions.        */

typedef struct _NxAppContext  *NxAppContext ;		/* Application context. */
typedef struct _NxIOSource  *NxInputId ;		/* I/O source. */
typedef struct _NxTimer  *NxIntervalId ;		/* Timeout timer. */
typedef struct _NxBackgroundTask  *NxWorkProcId ;	/* Work procedure. */

typedef long  NxInputMask ;				/* SELECT(2) masks. */
#define NxInputNoneMask	0L
#define NxInputReadMask	(1L << 0)
#define NxInputWriteMask	(1L << 1)
#define NxInputExceptMask	(1L << 2)

/* Callback function prototypes.                                        */

typedef int (*NxInputCallback) P_((NxAppContext, NxInputId, int, void *)) ;
typedef int (*NxTimerCallback) P_((NxAppContext, NxIntervalId, void *)) ;
typedef int (*NxWorkProc) P_((NxAppContext, NxWorkProcId, void *)) ;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int nix_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  NxInputId  NxAddInput P_((NxAppContext context,
                                  int source,
                                  NxInputMask condition,
                                  NxInputCallback callbackF,
                                  void *client_data)) ;

extern  NxIntervalId  NxAddTimeOut P_((NxAppContext context,
                                       double interval,
                                       NxTimerCallback callbackF,
                                       void *client_data)) ;

extern  NxWorkProcId  NxAddWorkProc P_((NxAppContext context,
                                        NxWorkProc workprocF,
                                        void *client_data)) ;

extern  int  NxCreateContext P_((NxAppContext *context)) ;

extern  int  NxMainLoop P_((NxAppContext context)) ;

#ifdef VMS
    extern  int  NxMainLoopEF P_((NxAppContext context, int timer_ef)) ;
#endif

extern  int  NxRemoveInput P_((NxAppContext context,
                               NxInputId source_ID)) ;

extern  int  NxRemoveTimeOut P_((NxAppContext context,
                                 NxIntervalId timer_ID)) ;

extern  int  NxRemoveWorkProc P_((NxAppContext context,
                                  NxWorkProcId workproc_ID)) ;

extern  int  NxSetDebug P_((NxAppContext context,
                            int enable)) ;

#ifdef __cplusplus
    }
#endif

#endif

