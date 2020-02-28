
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

int  tcp_read (

#    if __STDC__
        TcpEndpoint  dataPoint,
        double  timeout,
        int  numBytesToRead,
        char  *buffer,
        int  *numBytesRead)
#    else
        dataPoint, timeout, numBytesToRead, buffer, numBytesRead)

        TcpEndpoint  dataPoint ;
        double  timeout ;
        int  numBytesToRead ;
        char  *buffer ;
        int  *numBytesRead ;
#    endif

{
/*
 * Function: tcp_read.c
 * Version : 1.0
 * Created : 18-Feb-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function tcp_read() reads data from a network connection.  Because of
 *    the way network I/O works, a single record written to a connection
 *    by one task may be read in multiple "chunks" by the task at the other
 *    end of the connection.  For example, a 100-byte record written by a
 *    client may be read by the server in two chunks, one of 43 bytes and
 *    the other of 57 bytes.  tcp_read() takes this into account and, if
 *    you ask it for 100 bytes, it will automatically perform however many
 *    network reads are necessary to collect the 100 bytes.  You can also
 *    ask tcp_read() to return the first chunk received, whatever its length;
 *    see the numBytesToRead argument.
 *
 *    A timeout can be specified that limits how long tcp_read() waits for the
 *    first data to arrive.  If a complete record must be read (see above),
 *    tcp_read() will then wait as long as necessary for the remainder of the
 *    record to be received.  This ensures that a partial record will NOT be
 *    returned at the end of the timeout interval.
 *
 *    Invocation:
 *
 *        status = tcp_read(dataPoint, timeout, numBytesToRead,
 *                          buffer, &numBytesRead);
 *
 *    where
 *
 *        <dataPoint>         - I
 *            Is the endpoint handle returned by tcp_answer() or tcp_call().
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the caller
 *            wishes to wait for the first data to arrive.  A fractional time 
 *            can be specified; e.g., 2.5 seconds.  A negative timeout (e.g., 
 *            -1.0) causes an infinite wait; a zero timeout (0.0) allows a 
 *            read only if input is immediately available.
 *
 *        <numBytesToRead>    - I
 *            Has two different meanings depending on its sign:
 *
 *                (1) If the number of bytes to read is positive, tcp_read()
 *                    will continue to read input until it has accumulated
 *                    the exact number of bytes requested.  If the timeout
 *                    interval expires before ANY data has been read, then
 *                    tcp_read() returns with an EWOULDBLOCK status.  If some
 *                    of the data is read before the timeout interval expires,
 *                    tcp_read() will wait as long as necessary to read the
 *                    remaining data.  This ensures that a partial record is
 *                    not returned to the caller at the end of the timeout
 *                    interval.
 *
 *                (2) If the number of bytes to read is negative, tcp_read()
 *                    returns after reading the first "chunk" of input received;
 *                    the number of bytes read from that first "chunk" is 
 *                    limited by the absolute value of numBytesToRead.  A 
 *                    normal status (0) is returned if the first "chunk" of 
 *                    input is received before the timeout interval expires; 
 *                    EWOULDBLOCK is returned if no input is received within 
 *                    that interval.
 *
 *        <buffer>            - O
 *            Receives the input data.  This buffer should be at least
 *            numBytesToRead in size.
 *
 *        <numBytesRead>      - O
 *            Returns the actual number of bytes read.
 *
 *        <status>            - O
 *            Returns the status of reading from the network connection:
 *            zero if no errors occurred, EWOULDBLOCK if the timeout
 *            interval expired before the requested amount of data was
 *            input, and ERRNO otherwise.  (See the numBytesToRead
 *            argument for a description of how that argument affects
 *            the returned status code.)
 *
 * Modification History
 *
 * Variables Used
 */

    fd_set  readMask;
    int  firstInputOnly, numActive;
    int length;
    struct  timeval  deltaTime, expirationTime;

/*
 * Main part of function.
 */

    if (dataPoint == NULL) {

        errno = EINVAL;
        vperror("(tcp_read) NULL endpoint handle: ");
        return(errno);

    }

    if (dataPoint->fd < 0) {

        errno = EINVAL;
        vperror("(tcp_read) %d file descriptor: ", dataPoint->fd);
        return(errno);

    }

    if (numBytesRead != NULL)  *numBytesRead = 0;

    /* If a timeout interval was specified, then wait until the         */
    /* expiration of the interval for data to be received.              */

    if (timeout >= 0.0) {

        /* Compute the expiration time as the current time plus the     */
        /* interval.                                                    */

        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

        /* Wait for data to arrive.                                     */

        for (;;) {

            deltaTime = tv_subtract(expirationTime, tv_tod());

            FD_ZERO (&readMask);  
            FD_SET (dataPoint->fd, &readMask);

            numActive = select(dataPoint->fd+1, &readMask, NULL, NULL,
                               &deltaTime);

            if (numActive >= 0)  break;
            if (errno == EINTR)  continue;  /* Retry on signal interrupt. */

            vperror("(tcp_read) Error waiting for input on %s, socket %d.\nselect: ",
                    dataPoint->name, dataPoint->fd);
            return(errno);

        }

        if (numActive == 0) {

            errno = EWOULDBLOCK;
#ifdef REPORT_TIMEOUT
            vperror("(tcp_read) Timeout while waiting for input on %s, socket %d.\n",
                    dataPoint->name, dataPoint->fd);
#endif
            return(errno);

        }

    }

    /* Read the requested amount of data from the network connection.   */

    firstInputOnly = (numBytesToRead < 0);
    if (firstInputOnly) numBytesToRead = abs(numBytesToRead);

    while (numBytesToRead > 0) {

        length = read(dataPoint->fd, buffer, numBytesToRead);
        if (length < 0) {

            vperror("(tcp_read) Error reading from connection %d.\nread: ",
                     dataPoint->fd);
            return(errno);

        } else if (length == 0) {

            errno = EPIPE;
            vperror("(tcp_read) Broken connection on %s, socket %d.\nread: ",
                     dataPoint->name, dataPoint->fd);
            return(errno);

        }

        if (tcp_util_debug) {

            printf("(tcp_read) Read %d bytes from %s, socket %d.\n",
                    length, dataPoint->name, dataPoint->fd);
            meo_dumpd(stdout, "    ", 0, buffer, length);

        }

        if (numBytesRead != NULL)  *numBytesRead += length;
        numBytesToRead -= length;
        buffer += length;

        if (firstInputOnly) break;

    }

    return(0);

}

