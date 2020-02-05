
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

int xqt_util_debug = 0;          /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

/**/

int  xqt_open (

#    if __STDC__
        const  char  *shell,
        XqtStream  *stream)
#    else
        shell, stream)

        char  *shell ;
        XqtStream  *stream ;
#    endif

{
/*
 * Function: xqt_open.c
 * Version : 1.0
 * Created : 11-Dec-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function xqt_open() opens a shell execution stream so that a program 
 *    can execute host system shell commands. Under UNIX, xqt_open() forks 
 *    a subprocess, which then EXEC(2)s the specified UNIX shell; pipes 
 *    connect the parent program to the shell subprocess's standard input 
 *    and output channels. Under VMS, xqt_open() spawns a subprocess running 
 *    the VMS DCL interpreter; mailboxes connect the parent program to the 
 *    DCL subprocess's standard input and output channels.
 *
 *    The parent program can write commands to the shell subprocess using
 *    xqt_write() and read output from the shell subprocess using xqt_read().
 *
 *    Invocation:
 *
 *        status = xqt_open(shell, &stream);
 *
 *    where:
 *
 *        <shell>        - I
 *            is the name of the shell that will execute commands.  If this
 *            argument is NULL, the default shell of the host operating system
 *            is used (e.g., "/bin/sh" under UNIX and DCL under VMS).
 *
 *        <stream>    - O
 *            returns a handle for the opened shell execution stream.
 *
 *       <status>    - O
 *            returns the status of opening the shell execution stream, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int  i, status;
#if defined(vms)
    char  inputName[MAXPATHLEN], outputName[MAXPATHLEN];
    long  flags;
    struct  dsc$descriptor_s  inputNameDsc, outputNameDsc;
#endif

/*
 * Main part of function.
 */

#if defined(vms)
    if (shell == NULL)  shell = "DCL";
#else
    if (shell == NULL)  shell = "/bin/sh";
#endif

    /* Create and initialize a shell execution stream structure.        */

    if ((*stream = (XqtStream)malloc(sizeof(_XqtStream))) == NULL) {

        vperror("(xqt_open) Error allocating a shell execution stream structure.\nmalloc: ");
        return(errno);

    }

    (*stream)->shellName = strdup(shell);
    (*stream)->processID = 0;

    for (i = 0; i < 2; i++) {

        (*stream)->inputPipe[i] = -1;
        (*stream)->outputPipe[i] = -1;

    }

    (*stream)->input = NULL;
    (*stream)->output = NULL;
#if defined(vms)
    (*stream)->channel = 0;
#endif
    (*stream)->lastCommand = NULL;

    /* Create the interprocess communication entities used to           */
    /* communicate with the shell subprocess.                           */

    if ((status = pipe((*stream)->inputPipe))) {

        vperror("(xqt_open) Error creating shell's input pipe.\npipe: ");
        status = errno;  
        xqt_close(*stream);
        errno = status;
        return(errno);

    }

    if ((status = pipe((*stream)->outputPipe))) {

        vperror("(xqt_open) Error creating shell's output pipe.\npipe: ");
        status = errno;
        xqt_close(*stream);
        errno = status;
        return(errno);

    }

    /* Create a subprocess running the specified shell.                 */

#if defined(vms)

    /* Under VMS, spawn the subprocess, passing it the names of the     */
    /* input and output pipes for its SYS$INPUT and SYS$OUTPUT files.   */

    getname((*stream)->inputPipe[1], inputName);
    ASSIGN(inputNameDsc, inputName);
    getname((*stream)->outputPipe[0], outputName);
    ASSIGN(outputNameDsc, outputName);
    flags = CLI$M_NOWAIT;
    status = LIB$SPAWN(NULL, &inputNameDsc, &outputNameDsc, &flags,
                       NULL, &(*stream)->processID, NULL, NULL, NULL,
                       0, NULL, NULL);
    if (!(status & STS$M_SUCCESS)) {

        errno = EVMSERR;
        vaxc$errno = status;
        vperror("(xqt_open) Error spawning the %s shell subprocess.\nLIB$SPAWN: ",
                 (*stream)->shellName);
        status = errno;
        xqt_close(*stream);
        errno = status;
        return(errno);

    }

    /* Assign a VMS channel to the output pipe's mailbox. This channel  */
    /* is used by xqt_poll() to check for input from the shell          */
    /* subprocess.                                                      */

    status = SYS$ASSIGN(&outputNameDsc, &(*stream)->channel, 0, NULL);
    if (!(status & STS$M_SUCCESS)) {

        errno = EVMSERR;
        vaxc$errno = status;
        vperror("(xqt_open) Error assigning a channel to mailbox \"%s\".\nSYS$ASSIGN: ",
                 outputName);
        status = errno;
        xqt_close(*stream);  
        errno = status;
        return(errno);

    }

#else

    /* Under UNIX, fork a subprocess.  In the subprocess, close the     */
    /* unused pipe descriptors (the "write" side of my input pipe and   */
    /* the "read" side of my output pipe), reassign standard input and  */
    /* standard output to the remaining pipe descriptors, and start up  */
    /* the specified shell.                                             */

    (*stream)->processID = fork();

    if ((*stream)->processID == 0) {        /* Child process?           */

        close((*stream)->inputPipe[1]);     /* Write side of input pipe.*/
        close((*stream)->outputPipe[0]);    /* Read side of output pipe.*/

        /* Reassign standard input.                                     */

        if (dup2 ((*stream)->inputPipe[0], fileno (stdin)) < 0) {

            vperror("(xqtOpen) Error redirecting shell's standard input.\ndup2: ");
            exit(errno);

        }

        /* Reassign standard output.                                    */

        if (dup2 ((*stream)->outputPipe[1], fileno (stdout)) < 0) {

            vperror("(xqtOpen) Error redirecting shell's standard output.\ndup2: ");
            exit(errno);

        }

        /* Run the shell.                                               */

        execl((*stream)->shellName, (*stream)->shellName, NULL);
        vperror("(xqt_open) Error running %s shell.\nexecl: ", (*stream)->shellName);
        exit(errno);

    } else if ((*stream)->processID < 0) {    /* Error? */

        vperror("(xqt_open) Error creating shell subprocess.\nfork: ");
        status = errno;
        xqt_close(*stream);
        errno = status;
        return(errno);

    } else {                    /* Parent process? */

        /* <null> */ ;                /* No error. */

    }

#endif

    /* No matter what operating system we're running under, I'm the     */
    /* parent process when I get down here. Close the unused pipe       */
    /* descriptors, build (FILE *) pointers for the remaining pipe      */
    /* descriptors, and return a handle for the shell execution stream  */
    /* to the calling routine.                                          */

    /* Read side of input pipe.                                         */

    close((*stream)->inputPipe[0]);
    (*stream)->inputPipe[0] = -1;

    /* Write side of input pipe.                                        */

    (*stream)->input = fdopen((*stream)->inputPipe[1], "w");
    if ((*stream)->input == NULL) {

        vperror("(xqt_open) Error opening the input pipe as a file.\nfdopen: ");
        status = errno;
        xqt_close(*stream);
        errno = status;
        return(errno);

    }

    /* Read side of output pipe.                                        */

    (*stream)->output = fdopen((*stream)->outputPipe[0], "r");
    if ((*stream)->output == NULL) {

        vperror("(xqt_open) Error opening the output pipe as a file.\nfdopen: ");
        status = errno;
        xqt_close(*stream);
        errno = status;
        return(errno);

    }

    /* Don't close the output pipe under VMS.  Doing so sends an        */
    /* end-of-file message to the pipe's mailbox, which xqt_read()      */
    /* will read first, before any output from the shell!               */

    /* Write side of output pipe.                                       */

#if !defined(vms)
    close((*stream)->outputPipe[1]);        
    (*stream)->outputPipe[1] = -1;
#endif

    if (xqt_util_debug)
        printf("(xqt_open) Opened shell (\"%s\") execution stream %p.\n",
                (*stream)->shellName, *stream);

    return(0);

}

