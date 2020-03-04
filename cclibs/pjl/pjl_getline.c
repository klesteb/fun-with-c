
/*---------------------------------------------------------------------------*/
/*               Copyright (c) 2020 by Kevin L. Esteb                        */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "pjl_priv.h"
#include "lfn/lfn_priv.h"

/*----------------------------------------------------------------------*/

int  pjl_getline (

#if __STDC__
    PjlHandle handle,
    char  **string)
#else
    handle, string)

    PjlHandle handle ;
    char  **string ;
#endif

{
/*
 * Function: pjl_getline.c
 * Version : 1.0
 * Created : 26-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function pjl_getline() reads the next, CR/LF-delimited line of input 
 *    from the stream. Adapted from lfn_getline().
 *
 *    Invocation:
 *
 *        status = pjl_getline(handle, &string);
 *
 *    where
 *
 *        <handle>           - I
 *            The handle from pjl_open().
 * 
 *        <string>           - O
 *            returns a pointer to the null-terminated string that was read;
 *            the string does NOT include the trailing CR/LF.  The string is
 *            stored in memory private to the LFN stream and, although the
 *            caller can modify the string, it should be used or duplicated
 *            before calling pjl_getline() again.
 *
 *        <status>            - O
 *            returns the status of reading the input line, zero if there
 *            were no errors and ERRNO if an error occurred.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = OK;
    char  *buf, *line;
    int  last, length, next;
    double timeout;
    struct  timeval  expirationTime;

/*
 * Main part of function.
 */

    if ((handle == NULL) || (handle->stream == NULL)) {

        errno = stat = EINVAL;
        vperror("(pjl_getline) Invalid parameters.\n");
        goto fini;

    }

    timeout = handle->timeout;

    /* If a timeout interval was specified, then compute the expiration */
    /* time of the interval as the current time plus the interval.      */

    if (handle->timeout >= 0.0) {

        expirationTime = tv_add(tv_tod(), tv_createf(timeout));

    }

    /* Construct the next line of input.                                */

    buf = handle->stream->inputBuffer;
    next = handle->stream->nextChar;
    last = handle->stream->lastChar;
    line = handle->stream->inputString;

    length = 0;
    memset(line, '\0', handle->stream->maxLength);

    for (;;) {

        /* Copy buffered input to the input string until the LF or      */
        /* CR/LF terminator is reached.                                 */

        while ((next <= last) && (buf[next] != '\n')) {

            if (length < handle->stream->maxLength) {

                if (buf[next] == '\f') {

                    line[length++] = buf[next];
                    break;

                }

                if (buf[next] == '\r') {

                    next++;
                    continue;

                }

                line[length++] = buf[next++];

            } else {

                errno = stat = EMSGSIZE;
                vperror("(pjl_getline) Input line from %s exceeds %d bytes.\n",
                        lfn_name(handle->stream), handle->stream->maxLength);
                goto fini;

            }

        }

        line[length] = '\0';

        if ((next <= last) && (buf[next] == '\f')) break;
        if ((next <= last) && (buf[next] == '\n')) break;

        /* If the buffered input has been exhausted before completing   */
        /* the input line, then read more data from the socket          */
        /* connection.                                                  */

        if (handle->timeout > 0.0) {

            timeout = tv_float(tv_subtract(expirationTime, tv_tod()));

        }

        if (tcp_read(handle->stream->connection, timeout, 
                     -handle->stream->bufferSize,
                     handle->stream->inputBuffer, &last)) {

            vperror("(pjl_getline) Error reading %d bytes from %s stream.\ntcp_read: ",
                    handle->stream->bufferSize, lfn_name(handle->stream));
            stat = errno;
            goto fini;

        }

        next = 0;
        last--;            /* Reset the indices. */

    }

    handle->stream->nextChar = next + 1;
    handle->stream->lastChar = last;

    /* Return the input string to the caller.                           */

    *string = handle->stream->inputString;

    if (handle->debug) {

        printf("(pjl_getline) From %s: \"%s\"\n",
               lfn_name(handle->stream), handle->stream->inputString);
        meo_dumpd(stdout, "    ", 0, handle->stream->inputString, strlen(*string));

    }

    fini:
    return stat;

}

