
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

#include "tcp_priv.h"

/*----------------------------------------------------------------------*/

int  tcp_write (

#    if __STDC__
        TcpEndpoint  dataPoint,
        double  timeout,
        int  numBytesToWrite,
        const  char  *buffer,
        int  *numBytesWritten)
#    else
        dataPoint, timeout, numBytesToWrite, buffer, numBytesWritten)

        TcpEndpoint  dataPoint ;
        double  timeout ;
        int  numBytesToWrite ;
        const  char  *buffer ;
        int  *numBytesWritten ;
#    endif

{
/*
 * Function: tcp_write.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_write() writes data to a network connection.  Because
 *    of the way network I/O works, attempting to output a given amount
 *    of data to a network connection may require multiple system
 *    WRITE(2)s.  For example, when called to output 100 bytes of data,
 *    WRITE(2) may return after only outputting 64 bytes of data; the
 *    application is responsible for re-calling WRITE(2) to output the
 *    other 36 bytes.  tcp_write() takes this into account and, if you
 *    ask it to output 100 bytes, it will call WRITE(2) as many times
 *    as necessary to output the full 100 bytes of data to the connection.
 *
 *    A timeout interval can be specified that limits how long tcp_write()
 *    waits to output the desired amount of data.  If the timeout interval
 *    expires before all the data has been written, tcp_write() returns the
 *    number of bytes actually output in the numBytesWritten argument.
 *
 *    Invocation:
 *
 *        status = tcp_write(dataPoint, timeout, numBytesToWrite, buffer,
 *                           &numBytesWritten);
 *
 *    where
 *
 *        <dataPoint>             - I
 *            Is the endpoint handle returned by tcp_answer() or tcp_call().
 *
 *        <timeout>               - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the data to be output.  A fractional
 *            time can be specified; e.g., 2.5 seconds.   A negative timeout
 *            (e.g., -1.0) causes an infinite wait; tcp_write() will wait as
 *            long as necessary to output all of the data.  A zero timeout
 *            (0.0) specifies no wait: if the socket is not ready for writing,
 *            tcp_write() returns immediately; if the socket is ready for
 *            writing, tcp_write() returns after outputting whatever it can.
 *
 *        <numBytesToWrite>       - I
 *            Is the number of bytes to write.  If the timeout argument
 *            indicates an infinite wait, tcp_write() won't return until
 *            it has output the entire buffer of data.  If the timeout
 *            argument is greater than or equal to zero, tcp_write() will
 *            output as much as it can in the specified time interval,
 *            up to a maximum of numBytesToWrite.
 *
 *        <buffer>                - O
 *            Is the data to be output.
 *
 *        <numBytesWritten>       - O
 *            Returns the actual number of bytes written.  If an infinite wait
 *            was specified (TIMEOUT < 0.0), then this number should equal
 *            numBytesToWrite.  If a finite wait was specified, the number
 *            of bytes written may be less than the number requested.
 *
 *        <status>                - O
 *            Returns the status of writing to the network connection:
 *            zero if no errors occurred, EWOULDBLOCK if the timeout
 *            interval expired before the entire buffer of data was
 *            output, and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    fd_set  writeMask;
    int length;
    int numActive;
    struct  timeval  deltaTime, expirationTime;

/*
 * Main part of function.
 */

    if (dataPoint == NULL) {

        errno = EINVAL;
        vperror("(tcp_write) NULL endpoint handle: ");
        return(errno);

    }

    if (dataPoint->fd < 0) {

        errno = EINVAL;
        vperror("(tcp_write) %d file descriptor: ", dataPoint->fd);
        return(errno);

    }

    /* If a timeout interval was specified, then compute the expiration */
    /* time of the interval as the current time plus the interval.      */

    if (timeout >= 0.0)
        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

    /* While the timeout interval has not expired, attempt to write the */
    /* entire buffer of data to the network connection.                 */

    if (numBytesWritten != NULL) *numBytesWritten = 0;

    while (numBytesToWrite > 0) {

        /* Wait for the connection to be ready for writing.             */

        for (;;) {

            if (timeout >= 0.0)
                deltaTime = tv_subtract(expirationTime, tv_tod());

            FD_ZERO(&writeMask);  
            FD_SET(dataPoint->fd, &writeMask);

            numActive = select(dataPoint->fd+1, NULL, &writeMask, NULL,
                               (timeout < 0.0) ? NULL : &deltaTime);

            if (numActive >= 0)  break;
            if (errno == EINTR)  continue;  /* Retry on signal interrupt. */

            vperror("(tcp_write) Error waiting to write data to %s.\nselect: ",
                    dataPoint->name);
            return(errno);

        }

        if (numActive == 0) {

            errno = EWOULDBLOCK;
            vperror("(tcp_write) Timeout while waiting to write data to %s.\n",
                    dataPoint->name);
            return(errno);

        }

        /* Write the next chunk of data to the network.                 */

        length = write(dataPoint->fd, (char *) buffer, numBytesToWrite);
        if (length < 0) {

            vperror("(tcp_write) Error writing to %s.\nwrite: ",
                     dataPoint->name);
            return(errno);

        }

        if (tcp_util_debug) {

            printf("(tcp_write) Wrote %d bytes to %s, socket %d.\n",
                    length, dataPoint->name, dataPoint->fd);
            meo_dumpx(stdout, "    ", 0, buffer, length);

        }

        if (numBytesWritten != NULL)  *numBytesWritten += length;
        numBytesToWrite -= length;
        buffer += length;

    }

    return(0);

}

