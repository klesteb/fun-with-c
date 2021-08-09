
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       skt_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the SKT package.        */
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

#ifndef SKT_PRIV_H
#define SKT_PRIV_H 1

#include  <errno.h>                     /* System error definitions.    */
#include  <stdio.h>                     /* Standard I/O definitions.    */
#include  <stdlib.h>                    /* Standard C Library definitions. */
#include  <string.h>                    /* C Library string functions.  */

#if defined(VMS)
#    include  <netdb.h>                 /* Network database definitions.*/
#    include  <socket.h>                /* Socket-related definitions.  */
#    include  <ioctl.h>                 /* UNIX ioctl() definations.    */
#    include  "fd.h"                    /* File descriptor set definitions. */
#elif defined(VXWORKS)
#    include  <ioLib.h>                 /* I/O library definitions.     */
#    include  <selectLib.h>             /* SELECT(2) definitions.       */
#    include  <socket.h>                /* Socket-related definitions.  */
#    include  <sockLib.h>               /* Socket library definitions.  */
#else
#    include  <netdb.h>                 /* Network database definitions.*/
#    include  <unistd.h>                /* UNIX I/O definitions.        */
#    include  <sys/types.h>             /* System type definitions.     */
#    include  <sys/ioctl.h>             /* I/O control definitions.     */
#    include  <sys/socket.h>            /* Socket-related definitions.  */
#    include  <sys/time.h>              /* System time definitions.     */
#endif

#include  <netinet/in.h>                /* Internet IPC domain definitions. */

#include  "gpl/net_util.h"              /* Networking utilities.        */
#include  "gpl/vperror.h"               /* VPERROR() definitions.       */
#include  "gpl/skt_util.h"              /* TCP/IP networking utilities. */

#endif

