
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

int  pjl_rdymsg(

#if __STDC__
    PjlHandle handle, char *msg)
#else
    handle, msg)

    PjlHandle handle;
    char *msg;
#endif

{
/*
 * Function: pjl_rdymsg.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the ready message on the printer front panel.
 *    It will search the configuration to find the display size. If one is
 *    not found it will default to 15 characters.
 *
 * Modification History
 *
 * Variables Used
 */

    int len = 15;
    int stat = ERR;
    char buffer[PJL_K_BUFSIZ];
    PjlResponse *response = NULL;
    char *wanted = "DISPLAY CHARACTER SIZE";
    char *command = "@PJL RDYMSG DISPLAY = \"  %s\" \r\n";

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_rdymsg) Invalid parameters.\n");
        goto fini;

    }

    if (que_find(&handle->configs, wanted, _pjl_response_find) == QUE_OK) {

        response = que_get(&handle->configs);
        len = atoi(response->value);

        memset(buffer, '\0', PJL_K_BUFSIZ);
        snprintf(buffer, len, command, msg);

        if ((stat = _pjl_put(handle, buffer)) != OK) {

            vperror("(pjl_rdymsg) Unable to send the RDYMSG command.\n");

        }

    }

    fini:
    return(stat);

}

