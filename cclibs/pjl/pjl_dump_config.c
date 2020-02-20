
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

#    if __STDC__
    PjlHandle handle)
#    else
    handle)

    PjlHandle handle;
#    endif

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
 *            This function will return 0 or errno.
 *
 * Modification History
 *
 * Variables Used
 */

    char *option = NULL;
    char *fmt2 = "%s=%s\n";
    char *fmt1 = "%s [%s %s]\n";
    PjlResponse *response = NULL;

/*
 * Main part of function.
 */

    printf("\nPrinter Config:\n\n");

    for (response = que_first(&handle->ustatus);
         response != NULL;
         response = que_next(&handle->ustatus)) {

        if (response->items == NULL) {

            printf(fmt2, response->name, response->value);

        } else {

            printf(fmt1, response->name, response->items, response->type);

            for (option = que_first(&response->options);
                 option != NULL;
                 option = que_next(&response->options)) {

                printf("    %s\n", option);

            }

        }

    }

    return(0);

}

