
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

int  NxRemoveInput(

#    if __STDC__
        NxAppContext  context,
        NxInputId  source_ID)
#    else
        context, source_ID)

        NxAppContext  context ;
        NxInputId  source_ID ;
#    endif

{
/*
 * Function: NxRemoveInput.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxRemoveInput "unregisters" an I/O source previously registered
 *    with the NIX dispatcher.  NxRemoveInput() is modeled after the X Toolkit
 *    function, "XtRemoveInput()".
 *
 *    Invocation:
 *
 *        status = NxRemoveInput(context, source_ID);
 *
 *    where:
 *
 *        <context>           - I
 *            Is the application context returned by NxCreateContext().  If
 *            this argument is NULL, the default application context is used.
 *            This should be the same context used in the call to NxAddInput()
 *            that registered the I/O source.
 *
 *        <source_ID>         - I
 *            Is the ID assigned to the I/O source by NxAddInput().
 *
 *        <status>            - O
 *            Returns the status of "unregistering" the I/O source, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    NxAppContext  app ;
    NxIOSource  ios, prev ;

/*
 * Main part of function.
 */

    /* Use the desired application context.                             */

    if (context == NULL) {

        if ((default_context == NULL) && NxCreateContext(NULL)) {

            vperror("(NxRemoveInput) Error creating default application context.\nNxCreateContext: ");
            return(errno);

        }

        context = default_context;

    }

    app = context;

    /* Locate the target I/O source in the list of registered sources.  */

    for (prev = NULL, ios = app->IO_source_list; ios != NULL; ios = ios->next) {

        if (ios == source_ID) break;
        prev = ios;

    }

    if (ios == NULL) {

        errno = EINVAL;
        vperror("(NxRemoveInput) Source ID %p not found.\n", source_ID);
        return(errno);

    }

    if (app->debug)
        printf("(NxRemoveInput) I/O source %d unregistered.\n", ios->source);

    /* Unlink the I/O source structure from the list and deallocate it. */

    if (prev == NULL) {            /* Beginning of list? */

        app->IO_source_list = ios->next;

    } else {                /* Middle or end of list. */

        prev->next = ios->next;

    }

    free(ios);

    return(0);

}

