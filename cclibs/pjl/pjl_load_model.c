
/*---------------------------------------------------------------------------*/
/*              Copyright (c) 2020 by Kevin L. Esteb                         */
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

int pjl_load_model(

#if __STDC__
    PjlHandle handle)
#else
    handle, model, len)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_load_model.c
 * Version : 1.0
 * Created : 20-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will load the printers model name.
 *
 *    Invocation:
 *
 *        status = pjl_load_model(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for this connection.
 *
 *        <status>            - O
 *            This function will return 0 on success or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    queue list;
    int stat = ERR;
    char *model = NULL;
    char *header = NULL;
    char *command = "@PJL INFO ID \r\n";

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_load_model) Invalid parameters.\n");
        goto fini;

    }

    que_init(&list);

    if ((stat = _pjl_do_command(handle, command, &list)) == OK) {

        if (que_size(&list) > 0) {

            header = que_pop_head(&list);
            model = que_pop_head(&list);
            handle->model = strdup(model);

        }

    }

    fini:
    return stat;

}

