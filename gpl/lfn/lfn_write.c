
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

#include "lfn_priv.h"

/*----------------------------------------------------------------------*/

int  lfn_write (

#    if __STDC__
    LfnStream  stream,
    double  timeout,
    int  numBytesToWrite,
    const  char  *buffer,
    int  *numBytesWritten)
#    else
    stream, timeout, numBytesToWrite, buffer, numBytesWritten)

    LfnStream  stream ;
    double  timeout ;
    int  numBytesToWrite ;
    char  *buffer ;
    int  *numBytesWritten ;
#    endif

{
/*
 * Function: lfn_write.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_write() writes a specified amount of unformatted data to
 *    a stream.
 *
 *    Invocation:
 *
 *        status = lfn_write(stream, timeout, numBytesToWrite, buffer,
 *                           &numBytesWritten);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the data to be output.  A fractional
 *            time can be specified; e.g., 2.5 seconds.   A negative timeout
 *            (e.g., -1.0) causes an infinite wait; lfn_write() will wait as
 *            long as necessary to output all of the data.  A zero timeout
 *            (0.0) specifies no wait: if the socket is not ready for writing,
 *            lfn_write() returns immediately; if the socket is ready for
 *            writing, lfn_write() returns after outputting whatever it can.
 *
 *        <numBytesToWrite>   - I
 *            Specifies how much data to write.  If the timeout interval
 *            expires before the requested number of bytes has been written,
 *            then lfn_write() returns with an EWOULDBLOCK status.
 *
 *        <buffer>            - I
 *            Is the data to be output.
 *
 *        <numBytesWritten>   - O
 *            Returns the actual number of bytes written.  If an infinite wait
 *            was specified (TIMEOUT < 0.0), then this number should equal
 *            numBytesToWrite.  If a finite wait was specified, the number
 *            of bytes written may be less than the number requested.
 *
 *        <status>            - O
 *            Returns the status of outputting the data, zero if there were
 *            no errors and ERRNO if an error occurred.
 *
 * Modification History
 *
 * Variables Used
 */

    int  numBytes;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(lfn_write) NULL stream handle: ");
        return(errno);

    }

    if (numBytesWritten == NULL) numBytesWritten = &numBytes;

    /* Output the data to the network.                                  */

    if (tcp_write(stream->connection, timeout, numBytesToWrite, buffer,
          numBytesWritten)) {

        vperror("(lfn_write) Error writing %d bytes to %s.\ntcp_write: ",
                numBytesToWrite, lfn_name(stream));
        return(errno);

    }

    if (lfn_util_debug || stream->debug) {

        printf("(lfn_write) To %s: %d bytes of unformatted data\n",
               lfn_name(stream), *numBytesWritten);

    }

    return(0);

}

