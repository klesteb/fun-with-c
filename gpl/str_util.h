
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       str_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the STR package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the STR utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef STR_UTIL_H
#define STR_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */
#include  <stddef.h>			        /* Standard C definitions.      */
#if !defined(NO_STRCASECMP) || !defined(NO_STRDUP)
#    include  <string.h>		        /* Standard C string functions. */
#endif

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int str_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  char  *str_cat P_((const char *source,
                          int length,
                          char destination[],
                          size_t maxLength)) ;

extern  char  *str_convert P_((char *string)) ;

extern  char  *str_copy P_((const char *source,
                           int length,
                           char destination[],
                           size_t maxLength)) ;

extern  char  *str_destring P_((char *string,
                               int length,
                               const char *quotes)) ;

extern  size_t  str_detab P_((char *stringWithTabs,
                             int length,
                             int tabStops,
                             char *stringWithoutTabs,
                             size_t maxLength)) ;

extern  void  str_env P_((const char *string,
                         int length,
                         char *translation,
                         size_t maxLength)) ;

extern  char  *str_etoa P_((char *string,
                           int length)) ;

extern  size_t  str_insert P_((const char *substring,
                              int subLength,
                              size_t offset,
                              char *string,
                              size_t maxLength)) ;

extern  int  str_match P_((const char *target,
                          const char *model)) ;

extern  size_t  str_remove P_((size_t numChars,
                              size_t offset,
                              char *string)) ;

extern  char  *str_tolower P_((char *string,
                              int length)) ;

extern  char  *str_toupper P_((char *string,
                              int length)) ;

extern  size_t  str_trim P_((char *string,
                            int length)) ;

#ifdef NO_STRCASECMP
    extern  int  strcasecmp P_((const char *thisString,
                                const char *thatString)) ;

    extern  int  strncasecmp P_((const char *thisString,
                                 const char *thatString,
                                 size_t length)) ;
#endif

#ifdef NO_STRDUP
    extern  char  *strdup P_((const char *string)) ;
#endif

extern  char  *strndup P_((const char *string,
                           size_t length)) ;

#ifdef __cplusplus
    }
#endif

#endif

