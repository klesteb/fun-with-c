
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

int pjl_set_debug(

#    if __STDC__
    PjlHandle handle, int toggle)
#    else
    handle, toggle)

    PjlHandle handle;
    int toggle;
#    endif

{
/*
 * Function: pjl_set_debug.c
 * Version : 1.0
 * Created : 12-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_set_debug toggles the debug flag.
 *
 *    Invocation:
 *
 *        status = pjl_set_debug(handle, toggle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <toggle>            - I
 *            The toggle for setting debug mode.
 *
 *        <status>            - O
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (handle != NULL) {

        handle->debug = toggle;

    }

    return(0);

}

