
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

int  xqt_read (

#    if __STDC__
        XqtStream  stream,
        int  maxLength,
        char  *buffer)
#    else
        stream, maxLength, buffer)

        XqtStream  stream ;
        int  maxLength ;
        char  *buffer ;
#    endif

{
/*
 * Function: xqt_read.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_read() reads the next line of output from a shell 
 *    subprocess.
 *
 *    Invocation:
 *
 *        status = xqt_read(stream, maxLength, buffer);
 *
 *    where:
 *
 *        <stream>            - I
 *            Is the stream handle returned by xqt_open().
 *
 *        <maxLength>         - I
 *            Is the maximum number of characters to read, i.e., the size of
 *            the buffer (see the next argument).
 *
 *        <buffer>            - O
 *            Is a buffer to receive the next line of text read from the shell.
 *            At most maxLength-1 characters will be stored in the buffer,
 *            followed by a null character.
 *
 *        <status>            - O
 *            Returns the status of reading the next line of output from the
 *            shell subprocess, zero if no errors occurred and ERRNO otherwise.
 *            -1 is returned if end-of-input is detected (or is that end-of-
 *            output?!).
 *
 * Modification History
 *
 * Variables Used
 */

    int length;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(xqt_read) Null shell stream.\n");
        return(errno);

    }

    /* Input the next line of output from the shell subprocess.         */

#if defined(vms)

    length = read(stream->outputPipe[0], buffer, maxLength - 1);
    if (length <= 0) {

        if (length == 0) {

            vperror("(xqt_read) End-of-input on stream %p.\n", stream);
            return(-1);

        }

        vperror("(xqt_read) Error reading from stream %p.\nread: ", stream);
        return(errno);

    }

#else

    if (fgets(buffer, maxLength, stream->output) == NULL) {

        if (feof(stream->output)) {

            vperror("(xqt_read) End-of-input on stream %p.\n", stream);
            return(-1);

        }

        vperror("(xqt_read) Error reading from stream %p.\nfgets: ", stream);
        return(errno);

    }

    length = strlen(buffer);

#endif

    buffer[length] = '\0';             /* Trim trailing newline.        */
    if (buffer[length-1] == '\n')  buffer[length-1] = '\0';

    if (xqt_util_debug)  printf("(xqt_read) Shell %p, %d bytes:\n%s\n",
                                 stream, length, buffer);

    return(0);

}

