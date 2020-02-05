
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

int  lfn_getline (

#    if __STDC__
    LfnStream  stream,
    double  timeout,
    char  **string)
#    else
    stream, timeout, string)

    LfnStream  stream ;
    double  timeout ;
    char  **string ;
#    endif

{
/*
 * Function: lfn_getline.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_getline() reads the next, CR/LF-delimited line of input 
 *    from the stream.
 *
 *    Invocation:
 *
 *        status = lfn_getline(stream, timeout, &string);
 *
 *    where
 *
 *        <stream>           - I
 *            is the stream handle returned by lfn_create().
 *
 *        <timeout>          - I
 *            specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the next line to be read.  A
 *            fractional time can be specified; e.g., 2.5 seconds.  A
 *            negative timeout (e.g., -1.0) causes an infinite wait; a
 *            zero timeout (0.0) allows a read only if input is immediately
 *            available.
 *
 *        <string>           - O
 *            returns a pointer to the null-terminated string that was read;
 *            the string does NOT include the trailing CR/LF.  The string is
 *            stored in memory private to the LFN stream and, although the
 *            caller can modify the string, it should be used or duplicated
 *            before calling lfn_getline() again.
 *
 *        <status>            - O
 *            returns the status of reading the input line, zero if there
 *            were no errors and ERRNO if an error occurred.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *buf, *line;
    int  last, length, next;
    struct  timeval  expirationTime;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(lfn_getline) NULL stream handle: ");
        return(errno);

    }

    /* If a timeout interval was specified, then compute the expiration */
    /* time of the interval as the current time plus the interval.      */

    if (timeout >= 0.0) {

        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

    }

    /* Construct the next line of input.                                */

    buf = stream->inputBuffer;
    next = stream->nextChar;
    last = stream->lastChar;
    line = stream->inputString;
    length = 0;

    for (;;) {

        /* Copy buffered input to the input string until the LF or      */
        /* CR/LF terminator is reached.                                 */

        while ((next <= last) && (buf[next] != '\n')) {

            if (length < stream->maxLength) {

                if ((line[length++] = buf[next++]) == '\r') length--;

            } else {

                errno = EMSGSIZE;
                vperror("(lfn_getline) Input line from %s exceeds %d bytes.\n",
                        lfn_name(stream), stream->maxLength);
                return(errno);

            }

        }
    
        line[length] = '\0';

        if ((next <= last) && (buf[next] == '\n')) break;

        /* If the buffered input has been exhausted before completing   */
        /* the input line, then read more data from the socket          */
        /* connection.                                                  */

        if (timeout > 0.0) {

            timeout = tv_float(tv_subtract(expirationTime, tv_tod()));

        }

        if (tcp_read(stream->connection, timeout, -stream->bufferSize,
                     stream->inputBuffer, &last)) {

            vperror("(lfn_getline) Error reading %d bytes from %s stream.\ntcp_read: ",
                    stream->bufferSize, lfn_name(stream));
            return(errno);

        }

        next = 0;
        last--;            /* Reset the indices. */

    }

    stream->nextChar = next + 1;
    stream->lastChar = last;

    /* Return the input string to the caller.                           */

    *string = stream->inputString;

    if (lfn_util_debug || stream->debug) {

        printf("(lfn_getline) From %s: \"%s\"\n",
               lfn_name(stream), stream->inputString);

    }

    return(0);

}

