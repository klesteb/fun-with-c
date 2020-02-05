
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

char  *drs_next (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{
/*
 * Function: drs_next.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drs_next() returns the next matching file in a directory scan.
 *
 *    Invocation:
 *
 *        fileName = drs_next(scan);
 *
 *    where:
 *
 *        <scan>              - I
 *            is the directory scan handle returned by drs_create().
 *
 *        <fileName>          - O
 *            returns the full pathname of the next matching file.  The file
 *            name is stored in memory belonging to the DRS_UTIL package and
 *            it should not be overwritten or deallocated by the caller.  NULL
 *            is returned if there are no matching files.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return(drs_get(scan, (scan == NULL) ? 0 : scan->nextFile));

}

