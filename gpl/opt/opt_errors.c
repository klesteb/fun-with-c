
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

int  opt_errors (

#    if __STDC__
        OptContext  scan,
        int  displayFlag)
#    else
        scan, displayFlag)

        OptContext  scan ;
        int  displayFlag ;
#    endif

{
/*
 * Function: opt_errors.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function opt_errors() enables/disables the display of error messages 
 *    when a command line error is detected.  Messages for invalid options 
 *    and missing options are normally written to standard error when 
 *    detected.
 *
 *    Invocation:
 *
 *        status = opt_errors(scan, displayFlag);
 *
 *    where:
 *
 *        <scan>              - I
 *            Is the command line scan context originally created by opt_init().
 *            If this argument is NULL, the default scan context is used.
 *
 *        <displayFlag>       - I
 *            If true (a non-zero value), the display of error messages is
 *            enabled.  If this argument is false (zero), error messages will
 *            not be displayed.  This flag only affects the specified scan
 *            context.
 *
 *        <status>            - O
 *            Returns the status of enabling or disabling the display of error
 *            messages, zero if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (scan == NULL) scan = defaultContext;
    if (scan == NULL) return(EINVAL);

    /* Set the error display flag in the scan context structure.        */

    scan->printErrors = displayFlag;

    if (opt_util_debug) printf("(opt_errors) Error messages are %s.\n",
                               displayFlag ? "enabled" : "disabled") ;

    return(0);

}

