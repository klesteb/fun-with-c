
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

int cmd_destroy(

#    if __STDC__
    void)
#    else
    )

#    endif

{
/*
 * Function: cmd_destroy.c
 * Version : 1.0
 * Created : 23-Apr-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cmd_destroy() will free the memory used by the CMD routines.
 *
 *    Invocation:
 *
 *        status = cmd_destroy();
 *
 *    where
 *
 *        <status>            - O
 *            This function will always return CMD_OK.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = CMD_OK;

/*
 * Main part of function.
 */

    if (handle != NULL) {

        hash_destroy(handle->table);
        hash_destroy(handle->qualifiers);

        free(handle->buffer);
        free(handle);

    }

    return(stat);

}

