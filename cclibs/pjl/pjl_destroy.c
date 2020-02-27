
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

int pjl_destroy(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_destroy.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_destroy() frees resources.
 *
 *    Invocation:
 *
 *        status = pjl_destroy(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle created with pjl_create().
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

    if (handle == NULL) {

        vperror("(pjl_destroy) Invalid parameters.\n");
        goto fini;

    }

    if (handle->model != NULL) free(handle->model);

    _pjl_clear_response(&handle->ustatus);
    _pjl_clear_response(&handle->configs);
    _pjl_clear_response(&handle->variables);

    free(handle);

    fini:
    return 0;

}

