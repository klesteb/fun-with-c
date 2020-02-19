/*
@(#)  FILE: ansi_setup.h  RELEASE: 1.4  DATE: 12/26/97, 13:16:21
*/
/*******************************************************************************

    ansi_setup.h

    ANSI/Non-ANSI C Configuration.

*******************************************************************************/

#ifndef  ANSI_SETUP_H		/* Has the file been INCLUDE'd already? */
#define  ANSI_SETUP_H  yes


/* Configure the handling of function prototypes. */

#ifndef P_
#    if (__STDC__ == 1) || defined(__cplusplus) || defined(vaxc)
#        define  P_(s)  s
#    else
#        define  P_(s)  ()
#        define  const
#        define  volatile
#    endif
#endif

/* Supply the ANSI strerror(3) function on systems that don't support it. */

#if defined(VMS)
/*#    define  strerror(code)  strerror (code, vaxc$errno) */
#elif defined(NO_STRERROR)
    extern  int  sys_nerr ;		/* Number of system error messages. */
    extern  char  *sys_errlist[] ;	/* Text of system error messages. */
#    define  strerror(code) \
        (((code < 0) || (code >= sys_nerr)) ? "unknown error code" \
                                            : sys_errlist[code])
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifndef OK
#define OK  0
#define ERR -1
#endif

#endif				/* If this file was not INCLUDE'd previously. */

