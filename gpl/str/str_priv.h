
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       cmd_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the CMD package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the CMD utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef CMD_PRIV_H
#define CMD_PRIV_H 1


#include  <ctype.h>            /* Standard character functions. */
#include  <errno.h>            /* System error definitions. */
#include  <stdio.h>            /* Standard I/O definitions. */
#include  <stdlib.h>            /* Standard C Library definitions. */
#include  <string.h>            /* Standard C string functions. */
#if !__STDC__ && defined(sun)
#    define  memmove(dest,src,length)  bcopy(src,dest,length)
#endif

#ifdef VMS
#include  "fnm_util.h"            /* for converting VMS directory syntax */
#endif
#include  "get_util.h"            /* "Get Next" functions. */
#include  "vperror.h"            /* VPERROR() definitions. */
#include  "str_util.h"            /* String manipulation functions. */


#endif

