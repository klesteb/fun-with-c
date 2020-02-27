
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
    PjlHandle handle, char *host, char *port, double timeout)
#else
    handle, host, port, timeout)

    PjlHandle handle;
    char *host;
    char *port;
    double timeout;
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
 *        status = pjl_open(handle, host, port, timeout);
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

    int stat = ERR;
    TcpEndpoint connection;
    char service[PJL_K_BUFSIZ];

/*
 * Main part of function.
 */

    if ((handle == NULL) || (port == NULL) || 
        (host == NULL) || (timeout == 0)) {

        vperror("(pjl_open) Invalid parameters.\n");
        goto fini;

    }

    handle->timeout = timeout;
    memset(service, '\0', PJL_K_BUFSIZ);
    snprintf(service, PJL_K_BUFSIZ - 1, "%s@%s", port, host);

    if ((stat = tcp_call(service, 0, 0, &connection)) != 0) {

        vperror("(pjl_open) Error connecting to %s on %s.\n", host, port);
        goto fini;

    }

    if ((stat = lfn_create(connection, NULL, &handle->stream)) != 0) {

        vperror("(pjl_open) Error creating LFN connection.\n");

    }

    fini:
    return(stat);

}

