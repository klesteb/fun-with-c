
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

#include "skt_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  skt_iswriteable (

#    if __STDC__
        const  char  *name,
        int  fd)
#    else
        name, fd)

        char  *name ;
        int  fd ;
#    endif

{
/*
 * Function: skt_iswriteable.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function skt_iswriteable() checks to see if data can be written
 *    to a connection.
 *
 *    Invocation:
 *
 *        isWriteable = skt_iswriteable(name, fd);
 *
 *    where
 *
 *        <name>              - I
 *            Is the name of the socket for debugging purposes; this argument
 *            can be NULL.
 *
 *        <fd>                - I
 *            Is the UNIX file descriptor for the socket.
 *
 *        <isWriteable>       - O
 *            Returns true (a non-zero value) if data connection is ready
 *            for writing and false (zero) otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    fd_set  writeMask;
    struct  timeval  timeout;

/*
 * Main part of function.
 */

    if (name == NULL)  name = "?";

    /* Poll the data socket for output.                                 */

    for (;;) {

        FD_ZERO(&writeMask);  
        FD_SET(fd, &writeMask);
        timeout.tv_sec = timeout.tv_usec = 0;   /* No wait.             */

        if (select(fd+1, NULL, &writeMask, NULL, &timeout) >= 0)  break;
        if (errno == EINTR)  continue;         /* Retry on signal interrupt. */

        vperror("(skt_iswriteable) Error polling %s, socket %d.\nselect: ",
                 name, fd);
        return(0);

    }

    /* Ready for output?                                                */

    return(FD_ISSET(fd, &writeMask));

}

