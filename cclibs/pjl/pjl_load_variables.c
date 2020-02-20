
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

#    if __STDC__
    PjlHandle handle)
#    else
    handle)

    PjlHandle handle;
#    endif

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

    int stat = -1;
    char *command = "@PJL INFO VARIABLES \r\n";

/*
 * Main part of function.
 */

    if ((stat = _pjl_clear_list(&handle->variables))) {

        vperror("(pjl_load_variables) Error unable to clear the config list.\n");
        goto fini;

    }

    /* Ask for the printer configuration.                               */

    if ((stat = _pjl_do_command(handle, command, &handle->variables)) != OK) {

        vperror("(pjl_load_variables) Error requesting printer variables.\n");

    }

    fini:
    return stat;

}

