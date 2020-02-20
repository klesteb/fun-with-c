
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

#    if __STDC__
    PjlHandle handle, char *msg)
#    else
    handle, msg)

    PjlHandle handle;
    char *msg;
#    endif

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

    int stat;
    int len = 15;
    int lines;
    char *buff = NULL;
    char *wanted = "DISPLAY CHARACTER SIZE";

/*
 * Main part of function.
 */

    if (strlen(msg) > 0) {

        for (buff = que_first(&handle->configs);
             buff != NULL;
             buff = que_next(&handle->configs)) {

            if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

                len = pos(buff, "=", 0);
                lines = atoi(right(buff, len + 1));

                msg[lines] = '\0';

            }

        }

        stat = lfn_putline(handle->stream, handle->timeout,
                           "@PJL RDYMSG DISPLAY = \"  %s\" \r\n", msg);

    } else {

        stat = lfn_putline(handle->stream, handle->timeout, 
                           "@PJL RDYMSG DISPLAY = \"\" \r\n");

    }

    return(stat);

}

