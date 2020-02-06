
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

int lpd_close(

#    if __STDC__
    LpdHandle handle)
#    else
    handle)

	LpdHandle handle;
#    endif

{
/*
 * Function: lpd_close.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_close() closes the network connection to the LPD 
 *    server and frees any used resources.
 *
 *    Invocation:
 *
 *		status = lpd_close(handle);
 *
 *    where
 *
 *		<handle>                - I
 *			The handle returned from lpd_open().
 *
 *		<status>                - O
 *			This function will return 0.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (handle != NULL) {

        lfn_destroy(handle->stream);
        free(handle);

    }

    return(0);

}

