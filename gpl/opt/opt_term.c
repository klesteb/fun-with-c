
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

#include "opt_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  opt_term (

#    if __STDC__
        OptContext  scan)
#    else
        scan)

        OptContext  scan ;
#    endif

{
/*
 * Function: opt_term.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_term() terminates a command line scan. The scan context
 *    structure, created and initialized by opt_init(), is deleted.
 *
 *    Invocation:
 *
 *        status = opt_term(scan);
 *
 *    where:
 *
 *        <scan>              - I
 *            is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <status>            - O
 *            returns the status of terminating the command line scan, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    if (scan == NULL) scan = defaultContext;
    if (scan == NULL) return(EINVAL);

    if (opt_util_debug) {

        if (scan == defaultContext) {

            printf("(opt_term) Terminating default scan.\n");

        } else printf("(opt_term) Terminating scan %p.\n", scan);

    }

    /* Delete the scan context structure.                               */

    if (scan->letterOptions != NULL) free (scan->letterOptions);
    if (scan->letterIndex != NULL)   free (scan->letterIndex);

    if (scan->nameOptions != NULL) {

        for (i = 0; i < scan->numNames; i++) {

            if (scan->nameOptions[i] != NULL) free(scan->nameOptions[i]);

        }

        free(scan->nameOptions);

    }

    if (scan->nameIndex != NULL)  free(scan->nameIndex);
    if (scan->arglen != NULL)     free(scan->arglen);
    if (scan->nameString != NULL) free(scan->nameString);

    free (scan);

    if (defaultContext == scan) {

        defaultContext = NULL;
#ifdef VXWORKS
        if (taskVarDelete(0, (int *)&defaultContext) == ERROR) {

            vperror("(opt_term) Error removing the default context task variable.\ntaskVarDelete: ");
            return(errno);

        }
#endif

    }

    return(0);

}

