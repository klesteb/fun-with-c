
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

int  NxCreateContext(

#    if __STDC__
        NxAppContext  *context)
#    else
        context)

        NxAppContext  *context ;
#    endif

{
/*
 * Function: NxCreateContext.c
 * Version : 1.0
 * Created : 27-Apr-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function NxCreateContext creates a new application context for the
 *    NIX dispatcher.
 *
 *    Invocation:
 *
 *        status = NxCreateContext(&context);
 *
 *    where:
 *
 *        <context>           - O
 *            Returns the address of the new application context structure.
 *            If this argument is NULL, NxCreateContext() creates the default
 *            application context.
 *
 *        <status>            - O
 *            Returns the status of creating the application context, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    /* Creating the default context?                                    */

    if ((context == NULL) || (context == &default_context)) {

#ifdef VXWORKS
        if (taskVarAdd(0, (int *)&default_context) == ERROR) {

            vperror("(NxCreateContext) Error declaring the default context as a task variable.\ntaskVarAdd: ");
            return(errno);

        }
#endif

        context = &default_context;

    }

    if ((*context = (NxAppContext)malloc(sizeof(_NxAppContext))) == NULL) {

        vperror("(NxCreateContext) Error allocating application context structure.\nmalloc: ");
        return(errno);

    }

    (*context)->debug = nix_util_debug;
    (*context)->IO_source_list = NULL;
    (*context)->timeout_list = NULL;
    (*context)->workproc_queue = NULL;

    if (nix_util_debug)  
        printf("(NxCreateContext) Created context %p.\n", *context);

    return(0);

}

