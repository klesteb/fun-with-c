
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
#include "misc/misc.h"

/*----------------------------------------------------------------------*/

int pjl_load_ustatus(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif
{
/*
 * Function: pjl_load_ustatus.c
 * Version : 1.0
 * Created : 12-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_load_ustatus() will load the printers status.
 *
 *    Invocation:
 *
 *        status = pjl_load_ustatus(handle);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle from pjl_create().
 *
 *        <status>            - O
 *            This function will return OK on success or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    queue list;
    int stat = ERR;
    int toggle = 0;
    char *line = NULL;
    char *header = NULL;
    PjlResponse *response = NULL;
    char *command = "@PJL INFO USTATUS \r\n";

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_load_ustatus) Invalid parameters.\n");
        goto fini;

    }

    que_init(&list);

    /* Ask for the printer configuration.                               */

    if ((stat = _pjl_do_command(handle, command, &list)) != OK) {

        vperror("(pjl_load_ustatus) Error requesting printer ustatus.\n");
        goto fini;

    }

    if (que_size(&list) > 0) {

        header = que_pop_head(&list);

        while ((line = que_pop_head(&list))) {

            if (line[0] != '\t') {

                if (toggle) {

                    que_push_head(&handle->ustatus, response);
                    toggle = 0;

                }

                if ((response = xmalloc(sizeof(PjlResponse))) == NULL) {

                    stat = ERR;
                    goto fini;

                }

                que_init(&response->options);

                if ((stat = _pjl_parse_ustatus(handle, response, line)) != OK) {

                    goto fini;

                }

            } else {

                que_push_head(&response->options, (void *)trim(line));
                toggle = 1;

            }

        }

        que_push_head(&handle->ustatus, response);

    }

    fini:
    return stat;

}

