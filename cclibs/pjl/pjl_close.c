
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

int pjl_close(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_close.c
 * Version : 1.0
 * Created : 14-Feb-2004
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_close() closes the network connection to the
 *    printer and frees any used resources.
 *
 *    Invocation:
 *
 *        status = pjl_close(handle);
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

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_close) Invalid parameters.\n");
        goto fini;

    }

    if (handle->stream != NULL) {

        if ((stat = lfn_destroy(handle->stream)) != 0) {

            vperror("(pjl_close) Unable to close network connection.\n");

        }

    }

    fini:
    return stat;

}

