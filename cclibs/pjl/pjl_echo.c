
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

#include <time.h>
#include "pjl_priv.h"

/*----------------------------------------------------------------------*/

int pjl_echo(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_echo.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will send a pjl echo command to syncronize the job stream
 *    with the printer. An echo command will cause the printer to flush the
 *    pjl command buffer. This information can be ignored.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buff[1024];
    time_t t = time(NULL);
    struct tm *tb = localtime(&t);
    strftime(buff, 127, "@PJL ECHO VMS %d-%b-%Y %T \r\n", tb);

/*
 * Main part of function.
 */

    if (handle == NULL) {

        vperror("(pjl_echo) Invalid parameters.\n");
        goto fini;

    }

    if ((stat = _pjl_send_command(handle, buff)) != OK) {

        vperror("(pjl_echo) Unable to send the ECHO command.\n");

    }

    fini:
    return stat;

}

