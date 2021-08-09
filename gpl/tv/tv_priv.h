
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       tv_priv.h                                                    */
/*                                                                           */
/* FACILITY:    Private variables and structures for the TV package.         */
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

#ifndef TV_PRIV_H
#define TV_PRIV_H 1

#include  <limits.h>                    /* Maximum/minimum value definitions. */
#include  <stdio.h>                     /* Standard I/O definitions.    */
#include  <stdlib.h>                    /* Standard C library definitions.*/
#include  <string.h>
#if defined(VMS)
#    include  <socket.h>                /* VMS "timeval" definitions.   */
#    include  <time.h>
#    define  gmtime  localtime          /* VMS/UCX GMTIME() returns NULL. */
#elif defined(VXWORKS)
#    include  <time.h>                  /* Time definitions.            */
#    include  <timers.h>                /* POSIX timer definitions.     */
#    include  <sys/times.h>             /* System time definitions.     */
#else
#    include  <time.h>                  /* Time definitions.            */
#    include  <sys/time.h>              /* System time definitions.     */
#endif

#include  "gpl/tv_util.h"               /* "timeval" manipulation functions. */

#endif

