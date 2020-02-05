
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

#include "port_priv.h"

/*----------------------------------------------------------------------*/

int port_destroy(

#    if __STDC__
        ListeningPort  port)
#    else
        port)

        ListeningPort  port ;
#    endif

{
/*
 * Function: port_destroy.c
 * Version : 1.0
 * Created : 01-Dec-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function port_destroy() destroys a listening port object.
 *
 *    Invocation:
 *
 *        status = port_destroy(port);
 *
 *    where:
 *
 *        <port>              - I
 *            Is the listening port handle returned by port_create().
 *
 *        <status>            - O
 *            Returns the status of destroying the listening port, zero
 *            if there were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (port == NULL) {

        errno = EINVAL;
        vperror("(port_destroy) NULL port handle: ");
        return(errno);

    }

    if (port_util_debug)  printf("(port_destroy) Closing %s.\n", port->name);

    /* Remove the listening port from the group of input sources        */
    /* monitored by the NIX Dispatcher.                                 */

    if (port->sourceID != NULL) NxRemoveInput(port->context, port->sourceID);

    /* Close the listening port's socket.                               */

    tcp_destroy(port->endpoint);

    /* Deallocate the listening port structure.                         */

    if (port->name != NULL) free(port->name);
    free(port);

    return(0);

}

