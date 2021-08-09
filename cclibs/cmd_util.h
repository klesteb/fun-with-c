
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       cmd_util.h                                                   */
/*                                                                           */
/* FACILITY:    Global variables and structures for the CMD package.         */
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

#ifndef CMD_UTIL_H
#define CMD_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include  "gpl/ansi_setup.h"        /* ANSI or non-ANSI C?              */

/*----------------------------------------------------------------------*/
/*    CMD Structures and Definitions.                                   */
/*----------------------------------------------------------------------*/

typedef int (input_proc) P_((char *prompt, char *buffer, int buf_size));
typedef int (command_proc) P_((void *data)) ;

typedef struct CommandTable {
    char *command;                          /* Command name.            */
	char *qualifiers;                       /* Command qualifiers.      */
    command_proc *procedure;                /* Procedure to execute.    */
} CommandTable;    

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int cmd_util_debug;       /* Global debug switch (1/0 = yes/no). */

#define CMD_OK     0             /* Command was processed OK.           */
#define CMD_END    0             /* Qualifier processing is done.       */
#define CMD_ERROR -1             /* An error has occured.               */
#define CMD_NOOPT -2             /* No qualifier parameter.             */
#define CMD_EOF   -3             /* No command input.                   */

#define SET_TABLE(x,a,b,c) {(x).command=(a);\
                            (x).qualifiers=(b);\
                            (x).procedure=(c);}

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

extern int cmd_create P_((CommandTable *table, input_proc *routine, 
                          char *prompt));

extern int cmd_input P_((void));

extern int cmd_dispatch P_((void *data));

extern int cmd_destroy P_((void));

extern int cmd_get P_((char **argument));

#ifdef __cplusplus
    }
#endif

#endif

