
/*---------------------------------------------------------------------------*/
/*              Copyright (c) 2020 by Kevin L. Esteb                         */
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

int pjl_model(

#if __STDC__
    PjlHandle handle, char *model, int *len)
#else
    handle, model, len)

    PjlHandle handle;
    char *model;
    int *len;
#endif
{
/*
 * Function: pjl_model.c
 * Version : 1.0
 * Created : 20-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return the printers model name.
 *
 * Modification History
 *
 * Variables Used
 */

    queue list;
    int stat = ERR;
    char *buffer = NULL;
    char *command = "@PJL INFO ID \r\n";

/*
 * Main part of function.
 */

    que_init(&list);

    if ((stat = _pjl_do_command(handle, command, &list)) == OK) {

        buffer = que_pop_head(&list);

        *len = strlen(buffer);
        model = buffer;

    }

    return stat;

}

