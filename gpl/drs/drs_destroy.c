
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

int  drs_destroy (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{
/*
 * Function: drs_destroy.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drs_destroy() destroys a directory scan.
 *
 *   Invocation:
 *
 *        status = drs_destroy(scan);
 *
 *    where:
 *
 *        <scan>              - I
 *            is the directory scan handle returned by drs_create().
 *
 *        <status>            - O
 *            returns the status of terminating the directory scan, zero
 *            if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    if (scan == NULL) {

        errno = EINVAL;
        vperror("(drs_destroy) NULL scan handle: ");
        return(errno);

    }

    if (drs_util_debug)
        printf("(drs_destroy) Terminating scan of %s.\n", scan->pathname);

    /* Delete it.                                                       */

    if (scan->pathname != NULL)  free(scan->pathname);
    if (scan->directory != NULL)  closedir(scan->directory);
    if (scan->compiledRE != NULL)  rex_delete(scan->compiledRE);

    /* Delete it.                                                       */

    if (scan->fileName != NULL) {

        for (i = 0; i < scan->numFiles; i++) {

            free(scan->fileName[i]);

        }

        free(scan->fileName);

    }

    /* Delete it.                                                       */

    free(scan);

    return(0);

}

