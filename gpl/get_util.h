
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       get_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the GET package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the GET utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef GET_UTIL_H
#define GET_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  char  *get_arg P_((const char *arg,
                          int *length)) ;

extern  char  *get_field P_((const char *s,
                            int *length)) ;

extern  char  *get_string P_((const char *last_argument,
                             const char *quotes,
                             int *length)) ;

extern  char  *get_word P_((const char *string,
                           const char *delimiters,
                           int *length)) ;

#ifdef __cplusplus
    }
#endif

#endif

