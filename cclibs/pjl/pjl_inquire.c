
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

int pjl_inquire(

#    if __STDC__
    PjlHandle handle, char *variable, char *value, int length)
#    else
    handle, varialbe, value, length)

    PjlHandle handle;
    char *variable;
    char *value;
    int length;
#    endif

{
/*
 * Function: pjl_inquire.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Inquire the value of a variable from the "PJL Current Environment".
 * 
 * Modification History
 *
 * Variables Used
 */

    int stat;
    int len;
    char *buff = NULL;
    char cmd[1024];
    queue junk;

/*
 * Main part of function.
 */

    que_init(&junk);

    /* Send the inquiry to the printer.                                 */

    sprintf(cmd, "@PJL INQUIRE %s \r\n", variable);
    stat = lfn_putline(handle->stream, handle->timeout, cmd);
    if (stat != 0) return(stat);

    len = (strlen(cmd) - 3);
    cmd[len] = '\0';

    /* Read the response back from the printer.                         */

    stat = _pjl_get_response(handle, &junk);
    if (stat != 0) return(stat);

    /* Parse the information from the returned list. The first line    */
    /* will be the pjl command. The second the information wanted.     */

    for (buff = que_first(&junk);
         buff != NULL;
         buff = que_next(&junk)) {

        if (strnicmp(buff, cmd, strlen(cmd)) == 0) {

            buff = que_next(&junk);
            strncpy(value, buff, length);
            value[length] = '\0';
            break;

        }

    }

    /* Free list.                                                       */

    if (!que_empty(&junk)) {

        while ((buff = que_pop_tail(&junk)) != NULL) {

            free(buff);

        }

    }

    return(stat);

}

