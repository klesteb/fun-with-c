
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       nix_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the NIX package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the NIX utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef NIX_PRIV_H
#define NIX_PRIV_H 1

#define  _BSD_SOURCE  1

#include  <errno.h>             /* System error definitions.            */
#include  <stdio.h>             /* Standard I/O definitions.            */
#include  <stdlib.h>            /* Standard C library definitions.      */
#include  <string.h>            /* Standard C string functions.         */
#if defined(VMS)
#    include  <libdtdef.h>      /* VMS date/time definitions.           */
#    include  <socket.h>        /* Socket-related definitions.          */
#    include  <stsdef.h>        /* VMS status code structure.           */
#    include  <starlet.h>       /* VMS system services (K.Esteb)        */
#    include  <lib$routines.h>  /* VMS RTL services (K.Esteb)           */
#    include  "fd.h"            /* File descriptor set definitions.     */
#elif defined(VXWORKS)
#    include  <selectLib.h>     /* SELECT(2) definitions.               */
#    include  <taskVarLib.h>    /* Task variable definitions.           */
#else
#    include  <sys/time.h>      /* System time definitions.             */
#    include  <sys/types.h>     /* System type definitions.             */
#endif
#include  "tv_util.h"           /* "timeval" manipulation functions.    */
#include  "vperror.h"           /* VPERROR() definitions.               */
#include  "nix_util.h"          /* Network I/O Handler definitions.     */
								
/*----------------------------------------------------------------------*/
/* I/O Source - contains information about an input/output source.      */
/*----------------------------------------------------------------------*/

typedef  struct  _NxIOSource {
    int  source;                /* File descriptor (UNIX).              */
    NxInputMask  condition;     /* Read, write, or exception (UNIX).    */
    NxInputCallback  callback;  /* Function to call when event occurs.  */
    void  *client_data;         /* Client-specified data passed to callback. */
    struct  _NxIOSource  *next;
}  _NxIOSource, *NxIOSource;

/*----------------------------------------------------------------------*/
/* Timeout Timer - contains information about a timeout timer.          */
/*----------------------------------------------------------------------*/

typedef  struct  _NxTimer {
    double  interval ;            /* Timeout in seconds. */
    struct  timeval  expiration ;    /* Absolute time of expiration. */
    NxTimerCallback  callback ;        /* Function to call upon timeout. */
    void  *client_data ;        /* Client-specified data passed to callback. */
    struct  _NxTimer  *next ;
}  _NxTimer, *NxTimer ;

/*----------------------------------------------------------------------*/
/* Background Task - contains information about a work procedure to be  */
/*                   executed in the "background".                      */
/*----------------------------------------------------------------------*/

typedef  struct  _NxBackgroundTask {
    NxWorkProc  workproc ;        /* Function to call in "background". */
    void  *client_data ;        /* Client-specified data passed to callback. */
    struct  _NxBackgroundTask  *next ;
}  _NxBackgroundTask, *NxBackgroundTask ;

/*----------------------------------------------------------------------*/
/* Application Context - gathers the information about a particular, NIX*/
/*                       event loop.                                    */
/*----------------------------------------------------------------------*/

typedef  struct  _NxAppContext {
    int  debug ;            /* Debug switch (1/0 = yes/no). */
    _NxIOSource  *IO_source_list ;    /* List of registered I/O sources. */
    _NxTimer  *timeout_list ;        /* List of registered timeout timers. */
    _NxBackgroundTask  *workproc_queue ;/* Queue of registered work procedures. */
}  _NxAppContext ;

/*----------------------------------------------------------------------*/
/* Default application context if the caller doesn't specify one.       */
/* (Under VxWorks, this pointer is automatically registered with the    */
/* operating system as a task variable.)                                */
/*----------------------------------------------------------------------*/

extern NxAppContext default_context;

#endif

