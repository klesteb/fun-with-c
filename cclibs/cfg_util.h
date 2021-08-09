
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       cfg_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the CFG package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the CFG utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef CFG_UTIL_H
#define CFG_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */
#include "cclibs/que_util.h"

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct {
    char *filename;
    int flags;
    queue list;
} config_t;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

#define CFG_OK     0
#define CFG_ERROR -1

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern int cfg_save P_((config_t *cfgs));

extern int cfg_close P_((config_t *cfgs));

extern int cfg_destroy P_((config_t *cfgs));

extern int cfg_open P_((char *filename, config_t *configs));

extern int cfg_del P_((config_t *cfgs, char *section, char *key));

extern int cfg_set P_((config_t *cfgs, char *section, char *key, char *value));

extern int cfg_get P_((config_t *cfgs, char *section, char *key, 
                       char *def, char *result));

extern int cfg_exists P_((config_t *cfgs, char *section));

#ifdef __cplusplus
    }
#endif

#endif

