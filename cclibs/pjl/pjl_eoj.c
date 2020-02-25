
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

int pjl_eoj(

#if __STDC__
    PjlHandle handle, char *jobname)
#else
    handle, jobname)

    PjlHandle handle;
    char *jobname;
    
#endif

{
/*
 * Function: pjl_eoj.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will signal the pjl stream that the current job is
 *    finished.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buffer[PJL_K_BUFSIZ];
    char *attention = "\033%%-12345X@PJL \r\n";
    char *command = "@PJL EOJ NAME = \"%s\" \r\n";

/*
 * Main part of function.
 */

    if (strlen(jobname) > 80) {

        jobname[80] = '\0';

    }

    memset(buffer, '\0', PJL_K_BUFSIZ);
    snprintf(buffer, PJL_K_BUFSIZ - 1, command, jobname);

    if ((stat = _pjl_put(handle, attention)) != OK) {

        vperror("(pjl_eoj) Unable to send the ATTENTION command\n");
        goto fini;

    }

    if ((stat = _pjl_put(handle, buffer)) != OK) {

        vperror("(pjl_eoj) Unable to send the EOJ command.\n");

    }

    fini:
    return stat;

}

