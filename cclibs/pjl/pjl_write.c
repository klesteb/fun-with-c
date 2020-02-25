
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

#include <time.h>
#include "pjl_priv.h"

/*----------------------------------------------------------------------*/

int pjl_write(

#if __STDC__
    PjlHandle handle, void *buffer, int size)
#else
    handle, buffer, size)

    PjlHandle handle;
    void *buffer;
    int size;
#endif

{
/*
 * Function: pjl_write.c
 * Version : 1.0
 * Created : 24-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    int written = 0;

/*
 * Main part of function.
 */

    if ((stat = lfn_write(handle->stream, handle->timeout, size, buffer, &written)) == 0) {

        if (size == written) {

            stat = OK;

        }

    }

    return stat;

}

