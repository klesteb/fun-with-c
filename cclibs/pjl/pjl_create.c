
/*---------------------------------------------------------------------------*/
/*                 Copyright (c) 2020 by Kevin L. Esteb                      */
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

int pjl_create(

#if __STDC__
    PjlHandle *handle)
#else
    handle)

    PjlHandle *handle;
#endif

{
/*
 * Function: pjl_create.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Allocate resources.
 *
 *    Invocation:
 *
 *        status = pjl_create(&handle);
 *
 *    where
 *
 *        <handle>            - O
 *            The handle for subsequent operations.
 *
 *        <status>            - O
 *            This function will return 0 on success or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    int offset = 0; 
    const char *error;
    char *config1 = "^(\\w+)$";
    char *config2 = "^(.+)\\b=(.+)";
    char *config3 = "^(.+)\\b\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    char *ustatus = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    char *variable = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 

/*
 * Main part of function.
 */

    if ((*handle = (_PjlHandle *)xmalloc(sizeof(_PjlHandle))) == NULL) {

        vperror("(pjl_create) Error allocating session structure.\n");
        stat = errno;
        goto fini;

    }

    (*handle)->debug = 0;
    (*handle)->stream = NULL;
    (*handle)->timeout = 2.0;
    (*handle)->model = strdup("unknown");

    que_init(&(*handle)->configs);
    que_init(&(*handle)->ustatus);
    que_init(&(*handle)->variables);

    if (((*handle)->rustatus = pcre_compile(ustatus, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_create) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rvariable = pcre_compile(variable, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_create) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig1 = pcre_compile(config1, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_create) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig2 = pcre_compile(config2, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_create) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig3 = pcre_compile(config3, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_create) Parsing error: %s, offset: %d\n", error, offset);

    }

    stat = OK;

    fini:
    return stat;

}

