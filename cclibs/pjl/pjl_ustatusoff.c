
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

int pjl_ustatusoff(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_ustatusoff.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will turn off the pjl ustatus.
 *
 *    Invocation:
 *
 *        status = pjl_ustatusoff(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle from pjl_create().
 *
 *        <status>            - O
 *            This function will return either OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char *command = "@PJL USTATUSOFF \r\n";

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_ustatusoff) Invalid parameters.\n");
        goto fini;

    }

    if ((stat = _pjl_put(handle, command)) != OK) {

        vperror("(pjl_ustatusoff) Unable to send the USTATUSOFF command.\n");

    }

    fini:
    return stat;

}

