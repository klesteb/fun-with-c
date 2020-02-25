
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

int  pjl_stmsg(

#if __STDC__
    PjlHandle handle, char *msg, char *status, int length)
#else
    handle, msg, status, length)

    PjlHandle handle;
    char *msg;
    char *status;
    char *length;
#endif

{
/*
 * Function: pjl_stmsg.c
 * Version : 1.0
 * Created : 24-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the operator message on the printer front panel.
 *    It will wait until the "operator" clears the message. This message will
 *    be returned.
 *
 * Modification History
 *
 * Variables Used
 */

    queue list;
    int len = 15;
    int stat = ERR;
    double timeout = 0;
    char *line = NULL;
    char buffer[PJL_K_BUFSIZ];
    PjlResponse *response = NULL;
    char *wanted = "DISPLAY CHARACTER SIZE";
    char *command = "@PJL STMSG DISPLAY = \"  %s\" \r\n";

/*
 * Main part of function.
 */

    que_init(&list);

    if ((response != NULL) && (length > 0)) {
        
        for (response = que_first(&handle->configs);
             response != NULL;
             response = que_next(&handle->configs)) {

            if (strcmp(response->name, wanted) == 0) {

                len = atoi(response->value);

            }

        }

        memset(buffer, '\0', PJL_K_BUFSIZ);
        snprintf(buffer, len, command, msg);

        pjl_get_timeout(handle, &timeout);
        pjl_set_timeout(handle, -1.0);

        if ((stat = _pjl_do_command(handle, buffer, &list)) != OK) {

            vperror("(pjl_stmsg) Unable to send the STMSG command.");
            goto fini;

        }

        if ((line = que_pop_head(&list)) != NULL) {

            stat = OK;
            int a = (strlen(line) > length) ? length : strlen(line); 
            strncpy(status, line, a);

        }

    }

    fini:
    pjl_set_timeout(handle, timeout);

    return stat;

}

