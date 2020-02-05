/************************************************************************
 *  Copyright (c) 1996 by Charles A. Measday                            *
 *                                                                      *
 *  Permission to use, copy, modify, and distribute this software       *
 *  and its documentation for any purpose and without fee is hereby     *
 *  granted, provided that the above copyright notice appear in all     *
 *  copies.  The author makes no representations about the suitability  *
 *  of this software for any purpose.  It is provided "as is" without   *
 *  express or implied warranty.                                        *
 ************************************************************************/

/*
@(#)  FILE: vperror.c  RELEASE: 1.6  DATE: 01/02/97, 17:30:17
*/
/*******************************************************************************

Procedure:

    vperror ()

    Format and Output a System Error Message.


Author:    Alex Measday, ISI


Purpose:

    Function VPERROR formats and outputs a system error message.  VPERROR
    essentially combines the C Library functions FPRINTF and PERROR, allowing
    the programmer to easily construct and output an arbitrary error message.
    Global variable VPERROR_PRINT must be set to a non-zero value in order
    for the message to actually be output.  VPERROR_PRINT can be set
    explicitly by the application or by setting environment variable,
    "VPERROR_PRINT", to a non-zero value.

    NOTE:  VPERROR() is NOT reentrant under VxWorks.  Setting VPERROR_PRINT
           in one task sets it for all tasks.


    Invocation:

        vperror (format, arg1, arg2, ..., argN) ;

    where

        <format>
            is a normal PRINTF format string.
        <arg1, ..., argN>
            are the arguments referenced by the format string.

*******************************************************************************/


#include  <errno.h>			/* System error definitions. */
#if __STDC__
#    include  <stdarg.h>		/* Variable-length argument lists. */
#else
#    include  <varargs.h>		/* Variable-length argument lists. */
#endif
#include  <stdio.h>			/* Standard I/O definitions. */
#include  <stdlib.h>			/* Standard C Library definitions. */
#include  <string.h>			/* C Library string functions. */

#if !__STDC__ && !defined(vaxc)		/* No STRERROR() function? */
    extern  int  sys_nerr ;		/* Number of system error messages. */
    extern  char  *sys_errlist[] ;	/* Text of system error messages. */
#endif


/*******************************************************************************
    Global VPERROR controls:
        VPERROR_PRINT - turns VPERROR output on or off.  Set VPERROR_PRINT to
            a non-zero value to turn output on; set it to zero to turn output
            off.
        VPERROR_FILE - is the (FILE *) pointer for VPERROR's output device;
            the default destination of error output is STDERR.
*******************************************************************************/

int  vperror_print = 0 ;		/* 0 = no print, !0 = print */
FILE  *vperror_file = NULL ;		/* VPERROR output device */


void  vperror (

#    if __STDC__
        char  *format,
        ...)
#    else
        format, va_alist)

        char  *format ;
        va_dcl
#    endif

{    /* Local variables. */
    char  *s ;
    int  errno_save ;
    va_list  ap ;
    static  char  *envar_value = NULL ;




    errno_save = errno ;		/* Save the error code. */

/* On the first call to this function with the global VPERROR_PRINT flag set
   to false, check to see if environment variable, VPERROR_PRINT, is defined.
   Waiting until the flag is false before checking the environment variable
   allows this mechanism to work for programs that enable error output during
   initialization and disable it in subsequent processing. */

    if ((envar_value == NULL) & !vperror_print) {
        envar_value = getenv ("VPERROR_PRINT") ;
        if (envar_value == NULL)
            envar_value = "0" ;
        else
            vperror_print = atoi (envar_value) ;
    }

    if (!vperror_print) {		/* Printing disabled? */
        errno = errno_save ;  return ;
    }

    if (vperror_file == NULL)		/* Output file assigned? */
        vperror_file = stderr ;

/* Get the error message text for the error code. */

    if (errno_save < 0) {
        s = NULL ;
    } else {
#if __STDC__
        s = strerror (errno_save) ;
#elif defined(__DECC)
        s = strerror (errno_save, vaxc$errno) ;
#else
        if (errno_save >= sys_nerr)
            s = NULL ;
        else
            s = sys_errlist[errno_save] ;
#endif
    }

/* Format and output the user text to standard error, followed by the
   system error message. */

#if __STDC__
    va_start (ap, format) ;
#else
    va_start (ap) ;
#endif
    vfprintf (vperror_file, format, ap) ;
    fprintf (vperror_file, "%s\n", (s == NULL) ? "invalid ERRNO code" : s) ;
    va_end (ap) ;

    errno = errno_save ;		/* Restore the error code. */

    return ;

}

#ifdef  TEST

/*******************************************************************************

    Program to test VPERROR.  Compile as follows:

        % cc -g -DTEST vperror.c -I<... includes ...>

    Invocation:

        % a.out <errno_code> [ <vmserr_code> ]

*******************************************************************************/

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{

    if (argc < 2) {
        fprintf (stderr, "Usage: test <error_code>\n") ;
        exit (EINVAL) ;
    }

    vperror_print = 1 ;

    errno = atoi (argv[1]) ;
#ifdef __VMS
    if ((errno == EVMSERR) && (argc > 2))  vaxc$errno = atoi (argv[2]) ;
#endif
    vperror ("VPERROR(%d): ", errno) ;

}
#endif  /* TEST */
