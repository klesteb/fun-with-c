
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

int  xqt_fd (

#    if __STDC__
        XqtStream  stream)
#    else
        stream)

        XqtStream  stream ;
#    endif

{
/*
 * Function: xqt_fd.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_fd() returns the file descriptor used by xqt_read() to 
 *    read the output from the shell.
 *
 *    Invocation:
 *
 *        fd = xqtFd (stream) ;
 *
 *    where
 *
 *        <stream>            - I
 *            is the stream handle returned by xqtOpen().
 *
 *        <fd>                - O
 *            returns the UNIX file descriptor used to read output from 
 *            the shell.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

#if defined(vms)
    return ((stream == NULL) ? -1 : stream->outputPipe[0]);
#else
    return ((stream == NULL) ? -1 : fileno (stream->output));
#endif

}
