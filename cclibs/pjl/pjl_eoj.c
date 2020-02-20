
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2004 by Kevin L. Esteb                                     */
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

int pjl_eoj(
#if __STDC__
    PjlHandle handle, char *jobname)
#else
    handle, jobname)

    PjlHandle handle;
    char *jobname;
    
#endif
{
/*
 * Function: pjl_eoj.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will signal the pjl stream that the current job is
 *    finished.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;

/*
 * Main part of function.
 */

    stat = lfn_putline(handle->stream, handle->timeout, "@PJL EOJ NAME = \"%s\" \r\n", jobname);

    return(stat);

}

