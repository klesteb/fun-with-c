
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

int pjl_reset(
#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif
{
/*
 * Function: pjl_reset.c
 * Version : 1.0
 * Created : 14-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_reset() resets the printer to "User Default" state.
 *
 *    Invocation:
 *
 *        status = pjl_reset(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char *reset = "@PJL RESET \r\n";
    char *command = "\033%%-12345X@PJL \r\n";

/*
 * Main part of function.
 */

    /* Put the printer into command mode. */

    if ((stat = _pjl_put(handle, command)) != OK) {

        vperror("(pjl_reset) Error putting printer into command mode.\n");
        goto fini:

    }

    /* Send the PJL RESET command. */

    if ((stat = _pjl_put(handle, reset)) != OK) {

        vperror("(pjl_reset) Error resetting printer environment.\n");

    }

    fini:
    return stat;

}

