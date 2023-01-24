
/*---------------------------------------------------------------------------*/
/*                 Copyright (c) 2004 by Kevin L. Esteb                      */
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

int pjl_set_timeout(

#if __STDC__
    PjlHandle handle, double timeout)
#else
    handle, timeout)

    PjlHandle handle;
    double timeout;
#endif

{
/*
 * Function: pjl_set_timeout.c
 * Version : 1.0
 * Created : 24-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_set_timeout() sets the timeout for the network.
 *
 *    Invocation:
 *
 *        status = pjl_set_timeout(handle, timeout);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <timeout>           - I
 *            The timeout value.
 *
 *        <status>            - O
 *            This function will return OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = OK;

/*
 * Main part of function.
 */

    if ((handle == NULL) || (timeout == 0)) {

        stat = ERR;
        vperror("(pjl_set_timeout) Invalid parameters.\n");
        goto fini;

    }

    handle->timeout = timeout;

    fini:
    return stat;

}

