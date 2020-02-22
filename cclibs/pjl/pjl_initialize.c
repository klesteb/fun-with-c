
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

int pjl_initialize(

#if __STDC__
    PjlHandle handle)
#else
    handle)

    PjlHandle handle;
#endif

{
/*
 * Function: pjl_initialize.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will initialize the printer and reset it back to
 *    "PJL Current Environment" and "User Default Environment".
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char *command = "@PJL INITIALIZE \r\n";

/*
 * Main part of function.
 */

    if ((stat = _pjl_put(handle, command)) != OK) {

        vperror("(pjl_initialize) Unable to send INITIALIZE command.");

    }

    return stat;

}

