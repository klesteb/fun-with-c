
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

int pjl_comment(

#if __STDC__
    PjlHandle handle, char *comment)
#else
    handle, comment)

    PjlHandle handle;
    char *comment;
    
#endif

{
/*
 * Function: pjl_comment.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will place a comment into the pjl stream.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buffer[PJL_K_BUFSIZ + 1];
    char *command = "@PJL COMMENT %s \r\n";

/*
 * Main part of function.
 */

    memset(buffer, '\0', PJL_K_BUFSIZ);
    snprintf(buffer, PJL_K_BUFSIZ, command, comment);

    if ((stat = _pjl_put(handle, buffer)) != OK) {

        vperror("(pjl_comment) Unable to send COMMENT command.\n");

    }

    return stat;

}

