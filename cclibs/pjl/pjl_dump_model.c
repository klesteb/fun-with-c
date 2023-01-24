
/*---------------------------------------------------------------------------*/
/*               Copyright (c) 2020 by Kevin L. Esteb                        */
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

int pjl_dump_model(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_dump_model.c
 * Version : 1.0
 * Created : 21-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_dump_model() will dump the printers model.
 *
 *    Invocation:
 *
 *        status = pjl_dump_model(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <status>            - O
 *            This function will return OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char model[256];

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_dump_model) Invalid parameters.\n");
        goto fini;

    }

    if ((stat = pjl_get_model(handle, model, 255)) == OK) {

        printf("\nPrinter model: %s\n", model);

    }

    fini:
    return stat;

}

