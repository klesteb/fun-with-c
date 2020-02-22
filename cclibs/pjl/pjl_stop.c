
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

int pjl_stop(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_stop.c
 * Version : 1.0
 * Created : 13-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_stop() resets the printer to "User Default" state.
 *
 *    Invocation:
 *
 *        status = pjl_stop(handle);
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
    char *uel = "\033%%-12345X";

/*
 * Main part of function.
 */

    /* Send the UEL. */

    if ((stat = _pjl_put(handle, uel)) != OK) {

        vperror("(pjl_stop) Error sending RESET command.\n");

    }

    return stat;

}

