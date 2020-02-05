
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

int  lfn_read (

#    if __STDC__
    LfnStream  stream,
    double  timeout,
    int  numBytesToRead,
    char  *buffer,
    int  *numBytesRead)
#    else
    stream, timeout, numBytesToRead, buffer, numBytesRead)

    LfnStream  stream ;
    double  timeout ;
    int  numBytesToRead ;
    char  *buffer ;
    int  *numBytesRead ;
#    endif

{
/*
 * Function: lfn_read.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_read() reads a specified amount of unformatted data from
 *    a stream.
 *
 *    Invocation:
 *
 *        status = lfn_read(stream, timeout, numBytesToRead,
 *                          buffer, &numBytesRead);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the desired amount of input to be
 *            read.  A fractional time can be specified; e.g., 2.5 seconds.
 *            A negative timeout (e.g., -1.0) causes an infinite wait;
 *            a zero timeout (0.0) allows a read only if input is
 *            immediately available.
 *
 *        <numBytesToRead>    - I
 *            Has two different meanings depending on its sign.  (1) If the
 *            number of bytes to read is positive, lfn_read() will continue
 *            to read input until it has accumulated the exact number of bytes
 *            requested.  If the timeout interval expires before the requested
 *            number of bytes has been read, then lfn_read() returns with an
 *            EWOULDBLOCK status.  (2) If the number of bytes to read is
 *            negative, lfn_read() returns after reading the first "chunk"
 *            of input received; the number of bytes read from that first
 *            "chunk" is limited by the absolute value of numBytesToRead.
 *            A normal status (0) is returned if the first "chunk" of input
 *            is received before the timeout interval expires; EWOULDBLOCK
 *            is returned if no input is received within that interval.
 *
 *        <buffer>            - O
 *            Receives the input data.  This buffer should be at least
 *            numBytesToRead in size.
 *
 *        <numBytesRead>      - O
 *            Returns the actual number of bytes read.  If an infinite wait
 *            was specified (TIMEOUT < 0.0), then this number should equal
 *            (the absolute value of) numBytesToRead.  If a finite wait
 *            was specified, the number of bytes read may be less than the
 *            number requested.
 *
 *        <status>            - O
 *            Returns the status of reading the input, zero if there were
 *            no errors and ERRNO if an error occurred.
 *
 * Modification History
 *
 * Variables Used
 */

    int  bufferedInput, fixedAmount, length, numBytes;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(lfn_read) NULL stream handle: ");
        return(errno);

    }

    fixedAmount = (numBytesToRead >= 0);
    if (!fixedAmount) numBytesToRead = -numBytesToRead;

    if (numBytesRead == NULL) numBytesRead = &numBytes;

    /* Copy any buffered input to the caller's buffer.                  */

    length = stream->lastChar - stream->nextChar + 1;
    if (length > numBytesToRead) length = numBytesToRead;

    if (length > 0) {

        memcpy(buffer, &stream->inputBuffer[stream->nextChar], length);
        stream->nextChar += length;
        bufferedInput = length;

    } else {

        bufferedInput = 0;

    }

    /* Read the rest of the data from the network.                      */

    length = numBytesToRead - bufferedInput;

    if ((length > 0) && 
         tcp_read(stream->connection, timeout,
                  fixedAmount ? length : -length,
                  &buffer[bufferedInput], numBytesRead)) {

        vperror("(lfn_read) Error reading %d bytes from %s.\ntcp_read: ",
                length, lfn_name(stream));
        return(errno);

    }

    *numBytesRead += bufferedInput;

    if (lfn_util_debug || stream->debug) {

        printf("(lfn_read) From %s: %d bytes of unformatted data\n",
                lfn_name(stream), *numBytesRead);

    }

    return(0);

}

