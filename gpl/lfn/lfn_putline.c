
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

int  lfn_putline (

#    if __STDC__
    LfnStream  stream,
    double  timeout,
    const  char  *format,
    ...)
#    else
    stream, timeout, format, va_alist)

    LfnStream  stream ;
    double  timeout ;
    char  *format ;
    va_dcl
#    endif

{
/*
 * Function: lfn_putline.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_putLine() formats an output line and writes it to a 
 *    network connection.  The caller is responsible for ensuring that 
 *    the length of the formatted output line does not exceed the maximum 
 *    message length specified in the call to lfn_create().
 *
 *    Invocation:
 *
 *        status = lfn_putline(stream, timeout, format, arg1, arg2, ...);
 *
 *    where
 *
 *        <stream>            - I
 *            Is the stream handle returned by lfn_create().
 *
 *        <timeout>           - I
 *            Specifies the maximum amount of time (in seconds) that the
 *            caller wishes to wait for the line to be output.  A fractional
 *            time can be specified; e.g., 2.5 seconds.  A negative timeout
 *            (e.g., -1.0) causes an infinite wait.  A zero timeout (0.0)
 *            specifies no wait: if the connection is not ready for writing,
 *            lfn_putline() returns immediately; if the connection is ready
 *            for writing, lfn_putline() returns after outputting whatever it
 *            can.  A zero timeout is strongly discouraged.
 *
 *        <format>            - I
 *            Is a normal PRINTF(3)-style format string.
 *
 *        <arg1, ..., argN>   - I
 *            Are the arguments expected by the format string.
 *
 *        <status>            - O
 *            Returns the status of writing the line, zero if there were no
 *            errors and ERRNO if an error occurred.
 *
 * Modification History
 *
 * Variables Used
 */

    va_list ap;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(lfn_putline) NULL stream handle: ");
        return(errno);

    }

    /* Format the output line. */

#if __STDC__
    va_start(ap, format);
#else
    va_start(ap);
#endif
    (void) vsprintf(stream->outputString, format, ap);
    va_end(ap);

    /* Write the output line to the network connection.                 */

    if (tcp_write(stream->connection, timeout, strlen(stream->outputString),
                  stream->outputString, NULL)) {

        vperror("(lfn_putline) Error writing %d-byte output line to %s.\ntcp_write: ",
                strlen(stream->outputString), lfn_name(stream));
        return(errno);

    }

    if (lfn_util_debug || stream->debug) {

        printf("(lfn_putline) To %s: \"%s\"\n",
               lfn_name(stream), stream->outputString);

    }

    return(0);

}

