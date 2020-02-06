
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "cmd_priv.h"

/*----------------------------------------------------------------------*/

int cmd_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */
CmdHandle handle;                /* Global command table.               */

/*----------------------------------------------------------------------*/

extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

/**/

static int get_input(

#    if __STDC__
    char *prompt, char *buffer, int bufsize)
#    else
    prompt, buffer, bufsize)

    char *prompt;
    char *buffer;
    int bufsize
#    endif

{
/*
 * Function: get_input.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function get_input() is the default procedure to receive user input.
 *
 *    Invocation:
 *
 *        status = get_input(prompt, buffer, bufsiz);
 *
 *    where
 *
 *        <prompt>            - I
 *            A prompt to display before user input.
 *
 *        <buffer>            - O
 *            The buffer to accept user input.
 *
 *        <bufsiz>            - I
 *            The size of the buffer.
 *
 *        <status>            - O
 *            Returns the status of the process, This can be one of the
 *            following: ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int len;
    int stat;

/*
 * Main part of function.
 */

    /* Print the prompt.                                                */

    printf("%s", prompt);

    /* Get the response.                                                */

    errno = 0;
    fgets(buffer, bufsize, stdin);
    stat = errno;

    /* Trim the trailing \n.                                            */

    len = strlen(buffer);
    buffer[len - 1] = '\0';

    return(stat);

}

/**/

static int command_register(

#    if __STDC__
    HashTable table,
    char *command,
    void *data)
#    else
    table, command, data)

    HashTable table;
    char *command;
    void *data;
#    endif

{
/*
 * Function: command_register.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will load the command tables.
 *
 *    Invocation:
 *
 *        status = command_register(handle, command, data)
 *
 *    where
 *
 *        <table>             - I
 *            A HashTable created by cmd_create().
 *
 *        <command>           - I
 *            A command.
 *
 *        <data>              - I
 *            The data to be returned when that command is used.
 *
 *        <status>            - O
 *            Returns the status of the process, This can be one of the
 *            following: CMD_OK, CMD_ERROR, or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = CMD_OK;

/*
 * Main part of function.
 */

    if (data == NULL) {

        if (hash_search(table, command, NULL)) {

            stat = hash_delete(table, command) ;

        }

    } else {

        stat = hash_add(table, command, (void *)data);

    }

    return(stat);

}

/**/

int cmd_create(

#    if __STDC__
    CommandTable *table,
    input_proc *routine,
    char *prompt)
#    else
    table, routine, prompt)

    CommandTable *table;
    input_proc *routine;
    char *prompt;
#    endif

{
/*
 * Function: cmd_create.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cmd_create() will allocate the neccessary data structures
 *    needed for the rest of the CMD routines.
 *
 *    Invocation:
 *
 *        status = cmd_create(table, routine, prompt);
 *
 *    where
 *
 *        <table>             - I
 *            A command table that defines commands and procedures to be executed.
 *
 *        <routine>           - I
 *            A routine to read user input.
 *
 *        <prompt>            - I
 *            The prompt for the input routine.
 *
 *        <status>            - O
 *            Returns the status of the process, This can be one of the
 *            following: CMD_OK, CMD_ERROR, or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int x;
    int i;
    int stat = CMD_ERROR;

/*
 * Main part of function.
 */

    if ((handle = malloc(sizeof(_CmdHandle))) == NULL) {

        vperror("(cmd_create) Error allocating data structure.\n");
        return(errno);

    }

    handle->prompt = prompt;
    handle->buffer = xmalloc(CMD_K_BUFSIZ + 1);
    handle->routine = ((routine == NULL) ? get_input : routine);

    /* Count how many items are in the command table.                */

    for (x = 0; table[x].command != NULL; x++);

    /* Create the command hash table.                               */

    if ((stat = hash_create(x, 0, &handle->table)) != 0) {

        vperror("(cmd_create) Error creating command tables.\n");
        return(stat);

    }

    /* Load the table with the commands.                                */

    for (i = 0;  i < x;  i++) {

        stat = command_register(handle->table, table[i].command, (void *)table[i].procedure);

    }

    /* Create the command qualifier hash table.                         */

    if ((stat = hash_create(x, 0, &handle->qualifiers)) != 0) {

        vperror("(cmd_create) Error creating command qualifiers table.\n");
        return(stat);

    }

    /* Load the table with the command qualifiers.                      */

    for (i = 0;  i < x;  i++) {

        stat = command_register(handle->qualifiers, table[i].command, table[i].qualifiers);

    }

    return(stat);

}

