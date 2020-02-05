
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

int  xqt_close (

#    if __STDC__
        XqtStream  stream)
#    else
        stream)

        XqtStream  stream ;
#    endif

{
/*
 * Function: xqt_close.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_close() terminates a shell execution stream.
 *
 *    Invocation:
 *
 *        status = xqt_close(stream);
 *
 *    where:
 *
 *        <stream>            - I
 *            Is the stream handle returned by xqt_open().
 *
 *        <status>            - O
 *            Always returns zero.
 *
 * Modification History
 *
 * Variables Used
 */

    int i;

/*
 * Main part of function.
 */

    if (stream == NULL) {

        errno = EINVAL;
        vperror("(xqt_close) Null shell stream.\n");
        return(errno);

    }

    /* Close the input and output pipes and deallocate the execution    */
    /* stream structure.                                                */

    if (xqt_util_debug)  printf("(xqt_close) Closing shell (\"%s\") execution stream %p.\n",
                                stream->shellName, stream);

    for (i = 0; i < 2; i++) {

        if (stream->inputPipe[i] >= 0)  close(stream->inputPipe[i]);
        if (stream->outputPipe[i] >= 0)  close(stream->outputPipe[i]);

    }

    if (stream->input != NULL)  fclose(stream->input);
    if (stream->output != NULL)  fclose(stream->output);
    if (stream->shellName != NULL)  free(stream->shellName);
    if (stream->lastCommand != NULL)  free(stream->lastCommand);
#if defined(vms)
    if (stream->channel > 0)  sys$dassgn(stream->channel);
#endif
    free(stream);

    return(0);

}

