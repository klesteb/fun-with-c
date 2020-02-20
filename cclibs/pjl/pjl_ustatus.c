
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

#    if __STDC__
    PjlHandle handle, char *type, char *action)
#    else
    handle, type, action)

    PjlHandle handle;
    char *type;
    char *action;
#    endif

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

    int stat = 0;
    char *buff = NULL;
    char *wanted = "USTATUS";

/*
 * Main part of function.
 */

    if ((stricmp(action, "ON") != 0) ||
        (stricmp(action, "OFF") != 0) ||
        (stricmp(action, "VERBOSE") != 0)) return(-1);

    for (buff = que_first(&handle->configs);
         buff != NULL;
         buff = que_next(&handle->configs)) {

        if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

            for (;
                 buff != NULL;
                 buff = que_next(&handle->configs)) {

                if (buff[0] == '\t') {

                    if (stricmp(type, buff) == 0) {

                        stat = lfn_putline(handle->stream, handle->timeout,
                                           "@PJL USTATUS %s = %s \r\n",
                                           type, action);
                        goto fini;

                    }

                } else {

                    stat = -1;
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

