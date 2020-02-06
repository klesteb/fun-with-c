
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

extern char *strupr(char *);

/*----------------------------------------------------------------------*/

/**/

int cmd_dispatch(

#    if __STDC__
    void *data)
#    else
    data)

    void *data;
#    endif

{
/*
 * Function: cmd_dispatch.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cmd_dispatch() will parse the user input and call the 
 *    callback defined for that command, passing the optional data to
 *    the called routine.
 *
 *    Invocation:
 *
 *        status = cmd_dispatch(data);
 *
 *    where
 *
 *        <data>              - I
 *            Optional data to be sent to the callback procedure.
 *
 *        <status>            - O
 *            Returns the status of the process, This can be one of the
 *            following: CMD_OK, CMD_ERROR, CMD_EOF, or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int argc;
    int stat = CMD_ERROR;
    char **argv;
    void *do_cmd = NULL;
    void *qualifiers = NULL;

/*
 * Main part of function.
 */

    /* Check for an empty buffer. If one is found then return.          */

    if (strlen(handle->buffer) == 0) return(0);

    /* Parse the command line.                                          */

    if ((stat = opt_create_argv(NULL, handle->buffer, &argc, &argv)) == 0) {

        /* The hash utilities are case sensitive. Uppercase the command.*/

        strupr(argv[0]);

        /* Find the desired procedure.                                  */

        if (hash_search(handle->table, argv[0], &do_cmd) && (do_cmd != NULL)) {

            /* Retrieve any command qualifiers and setup the command    */
            /* line parsing.                                            */

            hash_search(handle->qualifiers, argv[0], &qualifiers);
            opt_init(argc, argv, qualifiers, &handle->scan);

            /* Execute the command.                                     */

            stat = ((command_proc *)do_cmd)(data);

            /* Terminate the command line parsing.                      */

            opt_term(handle->scan);

        } else {

            vperror("(cmd_dispatch) Error %s is an unregconized command.\n", argv[0]);

        }

        opt_delete_argv(argc, argv);

    }

    return(stat);

}

