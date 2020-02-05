
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

int  xqt_poll (

#    if __STDC__
        XqtStream  stream)
#    else
        stream)

        XqtStream  stream ;
#    endif

{
/*
 * Function: xqt_poll.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_poll() checks to see if output from a shell execution
 *    stream is available for reading by xqt_read().
 *
 *    Invocation:
 *
 *        num_chars = xqt_poll(stream);
 *
 *    where:
 *
 *        <stream>            - I
 *            is the stream handle returned by xqt_open().
 *
 *        <num_chars>         - O
 *            returns the number of characters waiting to be read from the
 *            shell execution stream. Because of operating system dependencies,
 *            this may or may not be a true reflection of how many characters
 *            can be read. The calling routine can assume that a positive
 *            number indicates outstanding input, zero indicates no available
 *            input, and a negative number indicates an error.
 *
 * Modification History
 *
 * Variables Used
 */

#if defined(vms)
    int  status;
    short  iosb[4], length;
    long  numMessages;
    struct  ItemList {          /* Item list structure.                 */
        short  size;            /* Size of buffer.                      */
        short  itemCode;        /* DVI parameter.                       */
        long  *buffer;          /* Buffer to receive information.       */
        short  *length;         /* Returned length of information.      */
        int  endOfList;         /* Indicates end of item list.          */
    }  itemList ;
#else
    long  length ;
#endif

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(xqt_poll) Null shell stream.\n");
        return(-1);

    }

#if defined(vms)

    /* Find out how many messages are sitting in the pipe's mailbox.    */

    itemList.size = sizeof numMessages;
    itemList.itemCode = DVI$_DEVDEPEND;
    itemList.buffer = &numMessages;
    itemList.length = &length;
    itemList.endOfList = 0;

    status = sys$getdviw(0, stream->channel, NULL, &itemList, iosb,
                         NULL, 0, NULL);
    if (status & STS$M_SUCCESS)  status = iosb[0];
    if (!(status & STS$M_SUCCESS)) {

        errno = EVMSERR;
        vaxc$errno = status;
        vperror("(xqt_poll) Error polling stream %p.\nSYS$GETDVIW: ", stream);
        length = -1;

    } else {

        length = numMessages & 0x0FFFF;    /* Ignore high 16 bits. */

    }

#else

    /* Find out how many characters of input are waiting to be read     */
    /* from the pipe.                                                   */

    while (ioctl(stream->outputPipe[0], FIONREAD, &length) == -1) {

        if (errno == EINTR) continue;        /* Retry on signal interrupt. */
        vperror("(xqt_poll) Error polling shell %p.\nioctl: ", stream);
        length = -1;

    }

#endif

    if (xqt_util_debug)  printf("(xqt_poll) %ld characters from shell %p.\n",
                                (long) length, stream);

    return(length);

}

