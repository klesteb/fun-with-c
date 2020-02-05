
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       net_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the NET package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the NET utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef NET_PRIV_H
#define NET_PRIV_H 1

#include  <errno.h>                     /* System error definitions. */
#include  <stdio.h>                     /* Standard I/O definitions. */
#include  <stdlib.h>                    /* Standard C Library definitions. */
#ifdef sun
#    define  strtoul  strtol            /* STRTOUL(3) is not supported. */
#endif
#include  <string.h>                    /* C Library string functions. */

#if defined(VMS)
#    include  <netdb.h>                 /* Network database definitions. */
#    include  <socket.h>                /* Socket-related definitions. */
#    define  MAXHOSTNAMELEN  64
#    define  CACHE_NAMES  0
#elif defined(VXWORKS)
#    include  <hostLib.h>               /* Host library definitions. */
#    include  <socket.h>                /* Socket-related definitions. */
#    define  CACHE_NAMES  0
#else
#    include  <netdb.h>                 /* Network database definitions. */
#    include  <sys/param.h>             /* System parameters. */
#    include  <sys/socket.h>            /* Socket-related definitions. */
#    include  <sys/types.h>             /* System type definitions. */
#    define  CACHE_NAMES  1
#endif

#include  <netinet/in.h>                /* Internet IPC domain definitions. */
#ifdef VXWORKS
#    include  <inetLib.h>               /* Internet library definitions. */
#else
#    include  <arpa/inet.h>             /* Internet library definitions. */
#endif

#if CACHE_NAMES
#    include  "hash_util.h"             /* Hash table definitions. */
#endif
#include  "str_util.h"                  /* String manipulation functions. */
#include  "vperror.h"                   /* VPERROR() definitions. */
#include  "net_util.h"                  /* Networking utilities. */

#endif

