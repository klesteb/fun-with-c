
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

int pjl_execute(

#if __STDC__
    PjlHandle handle, char *operation)
#else
    handle, operation)

    PjlHandle handle;
    char *operation;
#endif

{
/*
 * Function: pjl_execute.c
 * Version : 1.0
 * Created : 18-Feb-2004
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_execute() issued a command to run a printer "operation".
 *    If the operation is unknown the printer will ignore the command.
 *
 *    Invocation:
 *
 *        status = pjl_execute(handle, operation);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle for subsequent operations.
 *
 *        <operation>         - I
 *            The operation for the printer to perform.
 * 
 *        <status>            - O
 *            This function will return either OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buff[PJL_K_BUFSIZ];
    char *command = "@PJL EXECUTE %s \r\n";

/*
 * Main part of function.
 */

    if ((handle == NULL) || (operation == NULL)) {

        vperror("(pjl_execute) Invalid parameters.\n");
        goto fini;

    }

    if ((strcmp("SHUTDOWN", operation) == 0) ||
        (strcmp("DEMOPAGE", operation) == 0) ||
        (strcmp("RESIFONT", operation) == 0) ||
        (strcmp("PERMFONT", operation) == 0) ||
        (strcmp("PRTCONFIG", operation) == 0)) {

        memset(buff, '\0', PJL_K_BUFSIZ);
        snprintf(buff, PJL_K_BUFSIZ - 1, command, operation);

        if ((stat = _pjl_put(handle, buff)) != OK) {

            vperror("(pjl_execute) Unable to send the EXECUTE command \"%s\".", operation);

        }

    }

    fini:
    return stat;

}

