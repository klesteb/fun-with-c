
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       pjl_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the PJL package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the PJL utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      14-Apr-2004 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef PJL_PRIV_H
#define PJL_PRIV_H 1

#define  _BSD_SOURCE  1

#include <pcre.h>                   /* perl compatible regex            */
#include <errno.h>                  /* System error definitions.        */
#include <limits.h>                 /* Maximum/minimum value definitions.*/
#include <stdio.h>                  /* Standard I/O definitions.        */
#include <stdlib.h>                 /* Standard C Library definitions.  */
#include <string.h>                 /* C Library string functions.      */
#include "vperror.h"
#include "tcp_util.h"               /* TCP/IP networking utilities.     */
#include "lfn_util.h"               /* LFN network.                     */
#include "que_util.h"               /* linked list utilities            */
#include "pjl_util.h"               /* PJL definations.                 */
#include "hash_util.h"
#include "ansi_setup.h"
#include "misc/misc.h"

/*----------------------------------------------------------------------*/
/* PJL Handle - contains information for the PJL routines.              */
/*----------------------------------------------------------------------*/

typedef struct _PjlHandle {
    LfnStream stream;           /* Underlaying LFN data stream.         */
    char *model;                /* The printer model.                   */
    double timeout;             /* Timeout for network operations.      */
    queue ustatus;              /* The ustatus of the printer.          */
    queue variables;            /* Info variables from the printer.     */
    queue configs;              /* Config info from the printer.        */
    int debug;                  /* Global debug switch (1/0 = yes/no).  */
    pcre *rconfig1;             /* regex to parse config detail line    */
    pcre *rconfig2;             /* regex to parse config detail line    */
    pcre *rconfig3;             /* regex to parse config detail line    */
    pcre *rustatus;             /* regex to parse ustatus detai line.   */
    pcre *rvariable;            /* regex to parse variable detail line. */
} _PjlHandle;

typedef struct _PjlResponse {       
    char *name;
    char *value;
    char *items;
    char *type;
    queue options;
} PjlResponse;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

#define PJL_K_BUFSIZ 1024           /* Input buffer size of 1024 bytes  */
#define PJL_K_MODEL  256            /* the size of the model name       */

/*----------------------------------------------------------------------*/
/* External routines needed by the package.                             */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/* Internal routines needed by the package.                             */
/*----------------------------------------------------------------------*/

extern int _pjl_clear_list P_((queue *));
extern int _pjl_clear_response P_((queue *));
extern int _pjl_send_command P_((PjlHandle, char *));
extern int _pjl_get_response P_((PjlHandle, queue *));
extern int _pjl_do_command P_((PjlHandle, char *, queue *));
extern int _pjl_parse_config P_((PjlHandle, PjlResponse *, char *));
extern int _pjl_parse_ustatus P_((PjlHandle, PjlResponse *, char *));
extern int _pjl_parse_variables P_((PjlHandle, PjlResponse *, char *));

#endif

