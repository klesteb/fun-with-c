
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

#include "nix_priv.h"

/*----------------------------------------------------------------------*/

int  NxSetDebug(

#    if __STDC__
        NxAppContext  context,
        int  enable)
#    else
        context, enable)

        NxAppContext  context ;
        int  enable ;
#    endif

{
/*
 * Function: NxSetDebug.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxSetDebug enabled/disables debug output (to standard output)
 *    for an application context.
 *
 *    Invocation:
 *
 *        status = NxSetDebug(context, enable);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *
 *        <enable>            - I
 *            If true (a non-zero value), enables debug output for the
 *            application context; false (zero) disables debug output for
 *            the application context.
 *
 *        <status>            - O
 *            Returns the status of enabling or disabling debug output, zero
 *            if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxSetDebug) Error creating default application context.\nNxCreateContext: ");
            return(errno);

        }

        context = default_context;

    }

    /* Enable or disable debug output for this application context.     */

    context->debug = enable;

    return(0);

}

