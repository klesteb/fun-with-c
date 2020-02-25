
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2004 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "pjl_priv.h"

/*----------------------------------------------------------------------*/

int pjl_load_variables(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_load_variables.c
 * Version : 1.0
 * Created : 12-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_load_variables() will load the printers variables.
 *
 *    Invocation:
 *
 *        status = pjl_load_variables(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for this connection.
 *
 *        <status>            - O
 *            This function will return 0 on success or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    queue list;
    int stat = ERR;
    int toggle = 0;
    double timeout;
    char *line = NULL;
    PjlResponse *response = NULL;
    char *command = "@PJL INFO VARIABLES \r\n";

/*
 * Main part of function.
 */

    que_init(&list);
    pjl_get_timeout(handle, &timeout);
    pjl_set_timeout(handle, timeout + 20.0);

    /* Ask for the printer variables.                               */

    if ((stat = _pjl_do_command(handle, command, &list)) != OK) {

        vperror("(pjl_load_variables) Error requesting printer config.\n");
        goto fini;

    }

    while ((line = que_pop_head(&list))) {

        if (line[0] != '\t') {

            if (toggle) {

                que_push_head(&handle->variables, response);
                toggle = 0;

            }

            if ((response = xmalloc(sizeof(PjlResponse))) == NULL) {

                stat = ERR;
                goto fini;

            }

            que_init(&response->options);

            if ((stat = _pjl_parse_variables(handle, response, line)) != OK) {

                goto fini;

            }

        } else {

            que_push_head(&response->options, (void *)trim(line));
            toggle = 1;

        }

    }

    que_push_head(&handle->variables, response);

    fini:
    pjl_set_timeout(handle, timeout);

    return stat;

}

