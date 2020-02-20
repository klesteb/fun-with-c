
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
 *            This function will always return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buff[1024];
    char *command = "@PJL EXECUTE %s \r\n";

/*
 * Main part of function.
 */

    if ((handle != NULL) && (operation != NULL)) {

        memset(buff, '\0', 1024);
        snprintf(buff, 1023, command, operation);

        if ((stat = lfn_putline(handle->stream, handle->timeout, buff)) != OK) {
            
            
        }

    }

    return stat;

}

