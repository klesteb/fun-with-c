
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

#include "xqt_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  xqt_write (

#    if __STDC__
        XqtStream  stream,
        const  char  *format,
        ...)
#    else
        stream, format, va_alist)

        XqtStream  stream ;
        char  *format ;
        va_dcl
#    endif

{
/*
 * Function: xqt_write.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_write() outputs a command for execution to a shell 
 *    subprocess.
 *
 *    Invocation:
 *
 *        status = xqt_write(stream, format, arg1, ..., argN);
 *
 *    where:
 *
 *        <stream>            - I
 *            Is the stream handle returned by xqt_open().
 *
 *        <format>            - I
 *            Is a PRINTF(3)-style format specification for the command. 
 *            Do NOT add a trailing newline character to the command; the 
 *            need for a newline is operating system-dependent and 
 *            xqt_write() will take care of appending it if needed. If the 
 *            FORMAT argument is NULL, xqt_write() sends an end-of-input 
 *            indication to the shell subprocess, thus terminating the 
 *            subprocess.
 *
 *        <arg1> ...
 *        <argN>              - I
 *            Are the arguments expected by the format statement.
 *
 *        <status>            - O
 *            Returns the status of formatting and writing the command to 
 *            the shell subprocess, zero if no errors occurred and ERRNO 
 *            otherwise. This status does NOT indicate the completion status 
 *            of the command.
 *
 * Modification History
 *
 * Variables Used
 */

    char  command[1024];
    va_list  ap;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(xqt_write) Null shell stream.\n");
        return(errno);

    }

    /* If a format string was NOT specified, then notify the shell      */
    /* subprocess of end-of-input by closing its input pipe.            */

    if (format == NULL) {

        if (xqt_util_debug)
            printf("(xqt_write) Shell %p: <end-of-input>\n", stream);

        if (close(stream->inputPipe[1])) {

            vperror("(xqt_write) Error signalling end-of-input to the shell subprocess.\nclose: ");
            return(errno);

        }

        stream->inputPipe[1] = -1;
        return(0);

    }

    /* Format the command. (VSPRINTF(3) is used rather than VFPRINTF(3) */
    /* so that debug output can be generated.)                          */

#if __STDC__
    va_start(ap, format);
#else
    va_start(ap);
#endif

    if (vsprintf(command, format, ap) == EOF) {

        vperror("(xqt_write) Error formatting command: \"%s\"\nvsprintf: ",
                 format);
        return(errno);

    }

    /* Output the command to the shell subprocess.                      */

    if (xqt_util_debug)
        printf("(xqt_write) Shell %p: %s\n", stream, command);

#if defined(vms)

    if (write(stream->inputPipe[1], command, strlen(command)) < 0) {

        vperror("(xqt_write) Error writing to stream %p.\nwrite: ", stream);
        return(errno);

    }

#else

    if (fprintf(stream->input, "%s\n", command) < 0) {

        vperror("(xqt_write) Error writing to stream %p.\nfprintf: ", stream);
        return(errno);

    }

    if (fflush(stream->input) == EOF) {

        vperror("(xqt_write) Error flushing output to stream %p.\nfflush: ",
                 stream);
        return(errno);

    }

#endif

    return(0);

}

