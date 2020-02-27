
/*---------------------------------------------------------------------------*/
/*                  Copyright (c) 2020 by Kevin L. Esteb                     */
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

int  pjl_opmsg(

#if __STDC__
    PjlHandle handle, char *msg)
#else
    handle, msg)

    PjlHandle handle;
    char *msg;
#endif

{
/*
 * Function: pjl_opmsg.c
 * Version : 1.0
 * Created : 24-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the operator message on the printer front panel.
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
    char *command = "@PJL OPMSG DISPLAY = \"  %s\" \r\n";

/*
 * Main part of function.
 */

    if ((handle == NULL) && (msg == NULL)) {

        vperror("(pjl_opmsg) Invalid parameters.\n");
        goto fini;

    }

    if (que_find(&handle->configs, wanted, _pjl_response_find) == QUE_OK) {

        response = que_get(&handle->configs);
        len = atoi(response->value);

        memset(buffer, '\0', PJL_K_BUFSIZ);
        snprintf(buffer, len, command, msg);

        if ((stat = _pjl_put(handle, buffer)) != OK) {

            vperror("(pjl_opmsg) Unable to send the OPMSG command.");

        }

    }

    fini:
    return stat;

}

