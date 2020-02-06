
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

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int lpd_end_job(

#    if __STDC__
    LpdHandle handle)
#    else
    handle)

    LpdHandle handle;
#    endif

{
/*
 * Function: lpd_end_job.c
 * Version : 1.0
 * Created : 23-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_end_job() provides a logical end to a print job.
 *
 *    Invocation:
 *
 *        status = lpd_end_job(handle);
 *
 *    where
 *
 *        <handle>                - I
 *            The handle returned from lpd_open().
 *
 *        <status>                - O
 *            This function will return 0.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(0);

}

