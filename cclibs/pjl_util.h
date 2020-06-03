
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       pjl_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the PJL package.         */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the PJL utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      11-Feb-2020 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef PJL_UTILS_H
#define PJL_UTILS_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include "que_util.h"
#include "ansi_setup.h"                 /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _PjlHandle *PjlHandle;
typedef struct _PjlJobInfo *PjlJobInfo;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

#define PJL_K_BUFSIZ 1024           /* Input buffer size of 1024 bytes  */
#define PJL_K_MODEL  256            /* the size of the model name       */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern int  pjl_eoj P_((PjlHandle));
extern int  pjl_echo P_((PjlHandle));
extern int  pjl_stop P_((PjlHandle));
extern int  pjl_close P_((PjlHandle));
extern int  pjl_start P_((PjlHandle));
extern int  pjl_destroy P_((PjlHandle));
extern int  pjl_create P_((PjlHandle *));
extern int  pjl_initialize P_((PjlHandle));
extern int  pjl_load_model P_((PjlHandle));
extern int  pjl_ustatusoff P_((PjlHandle));
extern int  pjl_dump_model P_((PjlHandle));
extern int  pjl_ustatusoff P_((PjlHandle));
extern int  pjl_load_config P_((PjlHandle));
extern int  pjl_dump_config P_((PjlHandle));
extern int  pjl_load_ustatus P_((PjlHandle));
extern int  pjl_dump_ustatus P_((PjlHandle));
extern int  pjl_enter P_((PjlHandle, char *));
extern int  pjl_opmsg P_((PjlHandle, char *));
extern int  pjl_set_debug P_((PjlHandle, int));
extern int  pjl_load_variables P_((PjlHandle));
extern int  pjl_dump_variables P_((PjlHandle));
extern int  pjl_rdymsg P_((PjlHandle, char *));
extern int  pjl_comment P_((PjlHandle, char *));
extern int  pjl_set_timeout P_((PjlHandle, double));
extern int  pjl_get_timeout P_((PjlHandle, double *));
extern int  pjl_getline P_((PjlHandle, char **string));
extern int  pjl_get_model P_((PjlHandle, char *, int));
extern int  pjl_ustatus P_((PjlHandle, char *, char *));
extern int  pjl_stmsg P_((PjlHandle, char *, char *, int));
extern int  pjl_open P_((PjlHandle, char *, char *, double));
extern int  pjl_get_config P_((PjlHandle, char *, char *, int));
extern int  pjl_get_ustatus P_((PjlHandle, char *, char *, int));
extern int  pjl_job P_((PjlHandle, char *, int, int, int, char *));
extern int  pjl_get_variables P_((PjlHandle, char *, char *, int));
extern int  pjl_print P_((PjlHandle, char *, 
                          int (*file_read)(FILE *, void *, int),
                          int (*status_handler)(PjlHandle, queue *)));

#ifdef __cplusplus
    }
#endif

#endif

