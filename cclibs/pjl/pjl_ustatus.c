
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
    char buffer[PJL_K_BUFSIZ];
    PjlResponse *response = NULL;
    char *command = "@PJL USTATUS %s = %s \r\n";

/*
 * Main part of function.
 */

    if ((handle == NULL) || (type == NULL) || (action == NULL)) {

        vperror("(pjl_ustatus) Invalid parameters.\n");
        goto fini;

    }

    if (que_find(&handle->ustatus, type, _pjl_response_find) == QUE_OK) {

        response = que_get(&handle->ustatus);

        if (que_find(&response->options, action, _pjl_option_find) == QUE_OK) {

            memset(buffer, '\0', PJL_K_BUFSIZ);
            snprintf(buffer, PJL_K_BUFSIZ - 1, command, type, action);
            stat = _pjl_put(handle, buffer);

        }

    }

    fini:
    return(stat);

}

