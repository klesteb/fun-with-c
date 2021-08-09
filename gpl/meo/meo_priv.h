
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       meo_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the MEO package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the MEO utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef MEO_PRIV_H
#define MEO_PRIV_H 1

#include  <ctype.h>                     /* Standard character functions.*/
#include  <errno.h>                     /* System error definitions.    */
#include  <limits.h>                    /* Maximum/minimum value definitions. */
#ifndef PATH_MAX
#    include  <sys/param.h>             /* System parameters.           */
#    define  PATH_MAX  MAXPATHLEN
#endif
#include  <stdio.h>                     /* Standard I/O definitions.    */
#include  <stdlib.h>                    /* Standard C library definitions. */
#include  <string.h>                    /* Standard C string functions. */
#if defined(VMS)
#    include  <file.h>                  /* File definitions.            */
#    include  <unixio.h>                /* VMS-emulation of UNIX I/O.   */
#elif defined(VXWORKS)
#    include  <ioLib.h>                 /* I/O library definitions.     */
#elif defined(WIN32)
#    include  <io.h>                    /* Low-level I/O definitions.   */
#else
#    include  <fcntl.h>                 /* File control definitions.    */
#    include  <unistd.h>                /* UNIX I/O definitions.        */
#endif
#include  <sys/stat.h>                  /* File status definitions.     */
#ifndef isascii
#    define  isascii(c)  ((unsigned char) (c) <= 0177)
#endif

#include  "gpl/fnm_util.h"              /* Filename utilities.          */
#include  "gpl/vperror.h"               /* VPERROR() definitions.       */
#include  "gpl/meo_util.h"              /* Memory operations.           */

#endif

