
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       opt_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the OPT package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the OPT utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef OPT_PRIV_H
#define OPT_PRIV_H 1

#include  <errno.h>                     /* System error definitions.    */
#if __STDC__
#    include  <stdarg.h>                /* Variable-length argument lists. */
#else
#    include  <varargs.h>               /* Variable-length argument lists. */
#endif
#include  <stdio.h>                     /* Standard I/O definitions.    */
#include  <stdlib.h>                    /* Standard C library definitions. */
#include  <string.h>                    /* C Library string functions.  */
#ifdef VXWORKS
#    include  <taskVarLib.h>            /* Task variable definitions.   */
#endif
#include  "get_util.h"                  /* "Get Next" functions.        */
#include  "str_util.h"                  /* String manipulation functions. */
#include  "vperror.h"                   /* VPERROR() definitions.       */
#include  "opt_util.h"                  /* Option scanning definitions. */

/*----------------------------------------------------------------------*/
/* OPT Scan Context - stores information about a command line scan.     */
/*----------------------------------------------------------------------*/

typedef  struct  _OptContext {
    char  *letterOptions ;              /* String containing letter options. */
    int  *letterIndex ;                 /* Ordering of letter options in original options string. */
    int  numNames ;                     /* # of name options.           */
    char  **nameOptions ;               /* Pointer to array of name options. */
    int  *nameIndex ;                   /* Ordering of name options in original options string. */
    int  printErrors ;                  /* Should error messages be printed? */
    int  argc ;                         /* The number of arguments.     */
    char  **argv ;                      /* The arguments.               */
    int  *arglen ;                      /* The length of each argument. */
    int  endOfOptions ;                 /* Index of end-of-options marker. */
    int  current ;                      /* Index of current argument in command. */
    int  offset ;                       /* Offset within current argument. */
    enum {
        none,
        invalid_option, 
        missing_argument,
        io_redirection,
        letter, 
        letter_with_argument,
        name, 
        name_with_argument,
        non_option_argument
    } optionType ;                      /* Type of current option.      */
    char  *nameString ;                 /* Return value from OPT_NAME().*/
}  _OptContext ;

/*----------------------------------------------------------------------*/
/* OPT private data structures.                                         */
/*----------------------------------------------------------------------*/

extern OptContext defaultContext;
extern int opt_util_debug;

#endif

