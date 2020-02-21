
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2004 by Kevin L. Esteb                                     */
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

int pjl_open(
#if __STDC__
    char *host, char *port, double timeout, PjlHandle *handle)
#else
    host, port, timeout,handle)

    char *host;
    char *port;
    double timeout;
    PjlHandle *handle;
#endif
{
/*
 * Function: pjl_open.c
 * Version : 1.0
 * Created : 14-Feb-2004
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_open() will open a connection to a printer that supports
 *    the HP Printer Job Language.
 *
 *    Invocation:
 *
 *        status = pjl_open(host, port, timeout, &handle);
 *
 *    where
 *
 *        <host>              - I
 *            The printer.
 *
 *        <port>              - I
 *            The port the printer is listening on.
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the next line to be read. A
 *            fractional time can be specified; e.g., 2.5 seconds. A
 *            negative timeout (e.g., -1.0) causes an infinite wait; a
 *            zero timeout (0.0) allows a read only if input is immediately
 *            available.
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

    int len = 255;
    int stat = ERR;
    int offset = 0; 
    char model[256];
    const char *error;
    TcpEndpoint connection;
    char service[PJL_K_BUFSIZ];
    char *config1 = "^(\\w+)$";
    char *config2 = "^(.+)\\b=(.+)";
    char *config3 = "^(.+)\\b\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    char *ustatus = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 
    char *variable = "^(\\w+)=(\\w+)\\s+\\[(\\d+)\\s+(\\w+)\\]"; 

/*
 * Main part of function.
 */

    if ((*handle = (_PjlHandle *)malloc(sizeof(_PjlHandle))) == NULL) {

        vperror("(pjl_open) Error allocating session structure.\n");
        stat = errno;
        goto fini;

    }

    (*handle)->model = strdup("unknown");
    (*handle)->timeout = timeout;
    (*handle)->debug = 0;

    sprintf(service, "%s@%s", port, host);

    if ((stat = tcp_call(service, 0, 0, &connection)) != 0) {

        vperror("(pjl_open) Error connecting to %s on %s.\n", host, port);
        goto fini;

    }

    if ((stat = lfn_create(connection, NULL, &(*handle)->stream)) != 0) {

        vperror("(pjl_open) Error creating LFN connection.\n");
        goto fini;

    }

    que_init(&(*handle)->configs);
    que_init(&(*handle)->ustatus);
    que_init(&(*handle)->variables);

    if (((*handle)->rustatus = pcre_compile(ustatus, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_core) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rvariable = pcre_compile(variable, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_core) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig1 = pcre_compile(config1, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_core) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig2 = pcre_compile(config2, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_core) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if (((*handle)->rconfig3 = pcre_compile(config3, 0, &error, &offset, NULL)) == NULL) {

        vperror("(pjl_core) Parsing error: %s, offset: %d\n", error, offset);
        goto fini;

    }

    if ((stat = pjl_start((*handle))) == OK) {

        if ((stat = pjl_load_model((*handle), model, len)) == OK) {

            (*handle)->model = strndup(model, len);

        } else {

            (*handle)->model = strdup("unknown");

        }

        stat = pjl_stop((*handle));

    }

    fini:
    return(stat);

}

