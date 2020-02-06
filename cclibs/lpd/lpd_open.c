
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

int lpd_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

/**/

int lpd_open(

#    if __STDC__
    char *host, char *port, double timeout, LpdHandle *handle)
#    else
    host, port, timeout,handle)

    char *host;
    char *port;
    double timeout;
    LpdHandle *handle;
#    endif

{
/*
 * Function: lpd_open.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_open() will open a connection to a LPD server on the
 *    desired host. RFC-1179 says the originating port must be between 
 *    721 and 731.
 *
 *    Invocation:
 *
 *        status = lpd_open(host, port, timeout, &handle);
 *
 *    where
 *
 *        <host>              - I
 *            The host system running the LPD server.
 *
 *        <port>              - I
 *            The port the server is listening on.
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

    int x;
    int stat = -1;
    char service[LPD_K_BUFSIZ];
    TcpEndpoint connection;

/*
 * Main part of function.
 */

    if ((*handle = (_LpdHandle *)malloc(sizeof(_LpdHandle))) == NULL) {

        vperror("(lpd_open) Error allocating session structure.\n");
        return(errno);

    }

    (*handle)->timeout = timeout;

    sprintf(service, "%s@%s", port, host);

    for (x = 721; x < 732; x++) {

        stat = tcp_call(service, x, 0, &connection);
        if (stat == 0) {

            stat = lfn_create(connection, NULL, &(*handle)->stream) ;
            break;

        }

    }
    
    return(stat);

}

