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

int pjl_dump_variables(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_dump_variables.c
 * Version : 1.0
 * Created : 14-Feb-2004
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_dump_variables() will dump the current stored printer
 *    environment variables for this stream.
 *
 *    Invocation:
 *
 *        status = pjl_dump_variables(handle);
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
    PjlResponse *response = NULL;
    char *format = "%s=%s [%s %s]\n";

/*
 * Main part of function.
 */

    printf("\nPrinter Variables:\n\n");

    for (response = que_first(&handle->variables);
         response != NULL;
         response = que_next(&handle->variables)) {

        printf(format, response->name, response->value, 
               response->items, response->type);
        
        for (option = que_first(&response->options);
             option != NULL;
             option = que_next(&response->options)) {

            printf("    %s\n", option);

        }

    }

    return(0);

}

