
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

char  *drs_get (

#    if __STDC__
        DirectoryScan  scan,
        int  index)
#    else
        scan, index)

        DirectoryScan  scan ;
        int  index ;
#    endif

{
/*
 * Function: drs_get.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drs_get() returns the I-th matching file in a directory scan.
 *
 *    Invocation:
 *
 *        fileName = drs_get(scan, index);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the directory scan handle returned by drs_create().
 *
 *        <index>             - I
 *            Is the index, 0 .. N-1, of the desired file.
 *
 *        <fileName>          - O
 *            Returns the full pathname of the indicated matching file.
 *            The file name is stored in memory belonging to the DRS_UTIL
 *            package and it should not be overwritten or deallocated by
 *            the caller. NULL is returned if an invalid index is specified.
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
        vperror("(drs_get) NULL scan handle: ");
        return(NULL);

    }

    if ((index < 0) || (index >= scan->numFiles)) {

        return(NULL);

    } else {

        scan->nextFile = index + 1;

        return(scan->fileName[index]);

    }

}

