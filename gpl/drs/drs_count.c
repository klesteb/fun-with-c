
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "drs_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  drs_count (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{
/*
 * Function: drs_count.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drs_count() returns the number of files in a directory 
 *    scan that matched the wildcard file specification.
 *
 *    Invocation:
 *
 *        numFiles = drs_count(scan);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the directory scan handle returned by drs_create().
 *
 *        <count>             - O
 *            Returns the number of files that matched the wildcard file
 *            specification.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (scan == NULL) {

        errno = EINVAL;
        vperror("(drs_count) NULL scan handle: ");
        return(0);

    }

    return(scan->numFiles);

}

