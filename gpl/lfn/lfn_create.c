
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

int lfn_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

int  lfn_create (

#    if __STDC__
    TcpEndpoint  dataPoint,
    const  char  *options,
    LfnStream  *stream)
#    else
    dataPoint, options, stream)

    TcpEndpoint  dataPoint ;
    char  *options ;
    LfnStream  *stream ;
#    endif

{
/*
 * Function: lfn_create.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function lfn_create() creates a LF-terminated network stream on top of
 *    a previously-created network connection.
 *
 *    The options argument passed into this function is a string containing
 *    zero or more of the following UNIX command line-style options:
 *
 *        "-buffer <size>"
 *            specifies the size of the internal input buffer; the default
 *            is 1024 bytes.
 *
 *        "-maximum <length>"
 *            specifies the maximum message size; the default is 1023 bytes.
 *
 *    Invocation:
 *
 *        status = lfn_create(dataPoint, options, &stream);
 *
 *    where
 *
 *        <dataPoint>     - I
 *            is the previously-created network endpoint for the underlying
 *            network connection.  (See "tcp_util.c" for more information
 *            about network endpoints.)  NOTE that the "dataPoint" endpoint
 *            is automatically destroyed (i.e., the socket is closed) when
 *            the LFN stream is destroyed.
 *
 *        <options>       - I
 *            is a string containing zero or more of the UNIX command line-style
 *            options described above.
 *
 *        <stream>        - O
 *            returns a handle for the new LFN stream.  This handle is used
 *            in calls to the other LFN functions.
 *
 *        <status>        - O
 *            returns the status of creating the stream, zero if there were
 *            no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *argument, **argv ;
    int  argc, errflg, option ;
    int  bufferSize, debug, maxLength ;
    OptContext  context ;

    static  char  *optionList[] = {
    "{buffer:}", "{debug}", "{maximum:}", NULL
    } ;

/*
 * Main part of function.
 */

    *stream = NULL;

    /* Convert the options string into an ARGC/ARGV array and scan the  */
    /* arguments.                                                       */

    bufferSize = INPUT_BUFFER_SIZE;
    debug = 0;
    maxLength = MAX_STRING_LENGTH;

    if (options != NULL) {

        opt_create_argv("lfn_create", options, &argc, &argv);
        opt_init(argc, argv, optionList, &context);
        opt_errors(context, 0);

        errflg = 0;

        while ((option = opt_get(context, &argument))) {
            switch (option) {
                case 1:                     /* "-buffer <size>" */
                    bufferSize = atoi(argument);
                    break;

                case 2:                     /* "-debug" */
                    debug = 1;
                    break;

                case 3:                     /* "-maximum <length>" */
                    maxLength = atoi(argument);
                    break;

                case NONOPT:
                case OPTERR:
                default:
                    errflg++;
                    break;

            }

        }

        opt_term(context);
        opt_delete_argv(argc, argv);

        if (errflg) {

            errno = EINVAL;
            vperror("(lfn_create) Invalid option/argument in %s's options string: \"%s\"\n",
                    tcp_name(dataPoint), options);
            return(errno);

        }

    }

    /* Create and initialize an LFN stream structure for the network    */
    /* connection.                                                      */

    if ((*stream = (_LfnStream *)malloc(sizeof(_LfnStream))) == NULL) {

        vperror("(lfn_create) Error allocating stream structure for \"%s\".\nmalloc: ",
                tcp_name(dataPoint));
        return(errno);

    }

    (*stream)->connection = dataPoint;
    (*stream)->bufferSize = bufferSize;
    (*stream)->nextChar = 0;
    (*stream)->lastChar = -1;
    (*stream)->inputBuffer = NULL;
    (*stream)->maxLength = maxLength;
    (*stream)->inputString = NULL;
    (*stream)->outputString = NULL;
    (*stream)->debug = debug;

    if (((*stream)->inputBuffer = malloc(bufferSize)) == NULL) {

        vperror("(lfn_create) Error allocating %d-byte input buffer for %s.\nmalloc: ",
                bufferSize, lfn_name(*stream));
        lfn_destroy(*stream);
        return(errno);

    }

    if (((*stream)->inputString = malloc(maxLength + 1)) == NULL) {

        vperror("(lfn_create) Error allocating %d-byte input string for %s.\nmalloc: ",
                maxLength, lfn_name(*stream));
        lfn_destroy(*stream);
        return(errno);

    }

    if (((*stream)->outputString = malloc(maxLength + 1)) == NULL) {

        vperror("(lfn_create) Error allocating %d-byte output string for %s.\nmalloc: ",
                maxLength, lfn_name(*stream));
        lfn_destroy(*stream);  
        return(errno);

    }

    if (debug)  printf("(lfn_create) Created formatted network stream %s, socket %d\n",
            lfn_name(*stream), lfn_fd(*stream));

    return(0);

}

