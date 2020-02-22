
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

int pjl_start(
#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif
{
/*
 * Function: pjl_start.c
 * Version : 1.0
 * Created : 13-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_start() sends the UEL to the printer.
 *
 *    Invocation:
 *
 *        status = pjl_start(handle);
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
    char *command = "\033%%-12345X@PJL \r\n";

/*
 * Main part of function.
 */

    if (handle != NULL) {

        /* Put the printer into command mode. */

        if ((stat = _pjl_put(handle, command)) != 0) {

            vperror("(pjl_start) Error placing printer into command mode.\n");
            goto fini;

        }

        /* Sync with the printer. */

        if ((stat = pjl_echo(handle)) != 0) {

            vperror("(pjl_start) Error syncing with the printer.\n");

        }

    }

    fini:
    return stat;

}

