
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "cmd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int cmd_input(

#    if __STDC__
    void)
#    else
    )

#    endif

{
/*
 * Function: cmd_input.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cmd_input() will retrive input from the user. It will either
 *    call the supplied callback for input or use the default get_input().
 *
 *    Invocation:
 *
 *        status = cmd_input();
 *
 *    where
 *
 *        <status>            - O
 *            Returns the status of the process, This can be one of the
 *            following: CMD_OK, CMD_ERROR, CMD_EOF, or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = CMD_ERROR;

/*
 * Main part of function.
 */

    stat = ((input_proc *)handle->routine)(handle->prompt, handle->buffer, CMD_K_BUFSIZ);

    return(stat);

}

