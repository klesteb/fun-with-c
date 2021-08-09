
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       drs_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the DRS package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the DRS utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef DRS_UTIL_H
#define DRS_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _DirectoryScan *DirectoryScan;       /* Scan handle.     */

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int drs_util_debug;       /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  drs_count P_((DirectoryScan scan));

extern  int  drs_create P_((const char *pathname,
                           DirectoryScan *scan));

extern  int  drs_destroy P_((DirectoryScan scan));

extern  char  *drs_first P_((DirectoryScan scan));

extern  char  *drs_get P_((DirectoryScan scan,
                          int which));

extern  char  *drs_next P_((DirectoryScan scan));

#ifdef __cplusplus
    }
#endif

#endif

