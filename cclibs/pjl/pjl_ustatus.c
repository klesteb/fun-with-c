
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

int pjl_ustatus(

#if __STDC__
    PjlHandle handle, char *type, char *action)
#else
    handle, type, action)

    PjlHandle handle;
    char *type;
    char *action;
#endif

{
/*
 * Function: pjl_ustatus.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will check the ustatus type and action against the printer
 *    configuration. If the type is not in the configuration a -1 will be
 *    returned. If the action is not defined then a -1 will be returned.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char *option = NULL;
    char buffer[PJL_K_BUFSIZ];
    PjlResponse *response = NULL;
    char *command = "@PJL USTATUS %s = %s \r\n";

/*
 * Main part of function.
 */

    for (response = que_first(&handle->ustatus);
         response != NULL;
         response = que_next(&handle->ustatus)) {

        if (strcmp(response->name, type) == 0) {

            for (option = que_first(&response->options);
                 option != NULL;
                 option = que_next(&response->options)) {

                if (strcmp(option, action) == 0) {

                    memset(buffer, '\0', PJL_K_BUFSIZ);
                    snprintf(buffer, PJL_K_BUFSIZ - 1, command, type, action);
                    stat = _pjl_put(handle, buffer);
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

