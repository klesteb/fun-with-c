
/*---------------------------------------------------------------------------*/
/*                  Copyright (c) 2020 by Kevin L. Esteb                     */
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

int pjl_get_variables(

#if __STDC__
    PjlHandle handle, char *name, char *buffer, int size)
#else
    handle, name, buffer, size)

    PjlHandle handle;
    char *name;
    char *bufffer;
    int size;
#endif

{
/*
 * Function: pjl_get_variables.c
 * Version : 1.0
 * Created : 03-Mar-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return the value for the named variable item.
 *
 *    Invocation:
 *
 *        status = pjl_get_variables(handle, name, buffer, size);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle from pjl_create().
 *
 *        <name>              - I
 *            The name of the variable item.
 *
 *        <buffer>            - O
 *            The value of the variable item..
 *
 *        <size>              - I
 *            This size of the buffer.
 *
 *        <status>            - O
 *            This function will return either OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    PjlResponse *response = NULL;

/*
 * Main part of function.
 */

    if ((handle == NULL) || (name == NULL) || (buffer == NULL) || (size < 1)) {

        vperror("(pjl_get_variables) Invalid parameters.\n");
        goto fini;

    }

    if (que_find(&handle->variables, name, _pjl_response_find) == QUE_OK) {

        stat = OK;
        response = que_get(&handle->variables);
        strncpy(buffer, response->value, size);

    }

    fini:
    return stat;

}

