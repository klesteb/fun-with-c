
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       opt_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the OPT package.         */
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

#ifndef OPT_UTIL_H
#define OPT_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef  struct  _OptContext  *OptContext;      /* Option scan context. */

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/* Values returned by OPT_GET().                                        */

#define OPTEND  0                   /* End of command line.             */
#define OPTERR -1                   /* Invalid option or missing argument.*/
#define NONOPT -2                   /* Non-option argument.             */
									
extern int opt_util_debug;          /* Global debug switch (1/0 = yes/no).*/

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  opt_create_argv P_((const char *program,
				 const char *command,
				 int *argc,
				 char *(*argv[])));

extern  int  opt_delete_argv P_((int argc,
				 char *argv[]));

extern  int  opt_errors P_((OptContext scan,
			    int display_flag));

extern  int  opt_get P_((OptContext scan,
			 char **argument));

extern  int  opt_index P_((OptContext scan));

extern  int  opt_init P_((int argc,
			  char *argv[],
			  char *optionString,
			  OptContext *scan));

extern  char  *opt_name P_((OptContext scan,
			    int index));

extern  int  opt_reset P_((OptContext scan,
			   int argc,
			   char *argv[]));

extern  int  opt_set P_((OptContext scan,
			 int new_index));

extern  int  opt_term P_((OptContext scan));

#ifdef __cplusplus
    }
#endif

#endif

