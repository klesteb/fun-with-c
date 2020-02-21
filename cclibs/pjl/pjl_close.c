
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

#    if __STDC__
    PjlHandle handle)
#    else
    handle)

    PjlHandle handle;
#    endif

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
 *            The handle for subsequent operations.
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

        /* Terminate the connection to the printer.                     */

        if (handle->stream != NULL) {

            lfn_destroy(handle->stream);

        }

        /* Free up resources                                           */

        if (handle->model != NULL) free(handle->model);

        _pjl_clear_response(&handle->ustatus);
        _pjl_clear_response(&handle->configs);
        _pjl_clear_response(&handle->variables);

    }

    return(0);

}

