
/*---------------------------------------------------------------------------*/
/*               Copyright (c) 2020 by Kevin L. Esteb                        */
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

int pjl_get_model(

#if __STDC__
    PjlHandle handle, char *model, int len)
#else
    handle, model, len)

    PjlHandle handle;
    char *model;
    int len;
#endif
{
/*
 * Function: pjl_get_model.c
 * Version : 1.0
 * Created : 21-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will return the model name.
 *
 * Modification History
 *
 * Variables Used
 */


/*
 * Main part of function.
 */

    strncpy(model, handle->model, len);

    return OK;

}

