
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int lpd_eof(

#    if __STDC__
    LpdHandle handle)
#    else
    handle)

    LpdHandle handle;
#    endif

{
/*
 * Function: lpd_eof.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_eof() will send a LPD EOF.
 *
 *    Invocation:
 *
 *        status = lpd_eof(handle);
 *
 *    where
 *
 *        <handle>                - I
 *            The handle returned from lpd_open().
 *
 *        <status>                - O
 *            This function will return 0 or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(handle->stream, handle->timeout, "\000");

    return(stat);

}

