
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

int pjl_dump_config(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_dump_config.c
 * Version : 1.0
 * Created : 14-Feb-2004
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_dump_config() will dump the current stored printer
 *    configuration for this stream.
 *
 *    Invocation:
 *
 *        status = pjl_dump_config(handle);
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

    int stat = OK;
    char *option = NULL;
    char *fmt1 = "%s\n";
    char *fmt2 = "%s=%s\n";
    char *fmt3 = "%s [%s %s]\n";
    PjlResponse *response = NULL;

/*
 * Main part of function.
 */

    if (handle == NULL) {

        stat = ERR;
        vperror("(pjl_dump_config) Invalid parameters.\n");
        goto fini;

    }

    printf("\nPrinter config:\n\n");

    for (response = que_first(&handle->configs);
         response != NULL;
         response = que_next(&handle->configs)) {

        if (response->items == NULL) {

            if (response->value == NULL) {

                printf(fmt1, response->name);

            } else {

                printf(fmt2, response->name, response->value);

            }

        } else {

            printf(fmt3, response->name, response->items, response->type);

            for (option = que_first(&response->options);
                 option != NULL;
                 option = que_next(&response->options)) {

                printf("    %s\n", option);

            }

        }

    }

    fini:
    return stat;

}

