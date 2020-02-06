
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

#define  _BSD_SOURCE  1

#include  <errno.h>                 /* System error definitions.        */
#include  <limits.h>                /* Maximum/minimum value definitions.*/
#include  <stdio.h>                 /* Standard I/O definitions.        */
#include  <stdlib.h>                /* Standard C Library definitions.  */
#include  <string.h>                /* C Library string functions.      */
#include  "vperror.h"
#include  "opt_util.h"              /* Option scanning definitions.     */
#include  "hash_util.h"             /* Hash table definitions.          */
#include  "cmd_util.h"              /* CMD definitions.                 */

/*----------------------------------------------------------------------*/
/* CMD Handle - contains information for the CMD parsing routines.      */
/*----------------------------------------------------------------------*/

typedef  struct  _CmdHandle {
	HashTable table;                /* Hash table for commands/procedures */
	HashTable qualifiers;           /* Hash table for commands/qualifiers */
	OptContext scan;                /* Command line scan context.       */
	char *prompt;					/* Input prompt.					*/
    char *buffer;                   /* Input buffer.                    */
	input_proc *routine;			/* Input routine.					*/
}  _CmdHandle ;

typedef struct _CmdHandle *CmdHandle; /* CMD handle.                    */

extern int cmd_util_debug;          /* Global debug switch (1/0 = yes/no).*/
extern CmdHandle handle;            /* Global command table.            */

#define CMD_K_BUFSIZ 1024           /* Input buffer size of 1024 bytes  */

#endif

