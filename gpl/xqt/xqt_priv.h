
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       xqt_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the XQT package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the XQT utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef XQT_PRIV_H
#define XQT_PRIV_H 1

#ifndef  VMS
#    define  _SVID_SOURCE  1
#endif

#include  <errno.h>             /* System error definitions. */
#if __STDC__
#    include  <stdarg.h>            /* Variable-length argument lists.  */
#else
#    include  <varargs.h>           /* Variable-length argument lists.  */
#endif
#include  <stdio.h>                 /* Standard I/O definitions.        */
#include  <stdlib.h>                /* Standard C library definitions.  */
#include  <string.h>                /* C Library string functions.      */
#if defined(VMS)
#    include  <clidef.h>            /* VMS CLI definitions.             */
#    include  <descrip.h>           /* Descriptor definitions.          */
#    include  <dvidef.h>            /* VMS device information definitions.*/
#    include  <nam.h>               /* RMS name block (NAM) definitions.*/
#    include  <starlet.h>           /* VMS system services.             */
#    include  <lib$routines.h>      /* VMS runtime libraries.           */
#    define  MAXPATHLEN  NAM$C_MAXRSS
#    include  <stsdef.h>            /* VMS status code structure.       */
#    include  <unixio.h>            /* VMS-emulation of UNIX I/O.       */
#    include  <unistd.h>            /* VMS-emulation of UNIX STD        */
#elif defined(VXWORKS)
#    include  <ioLib.h>             /* I/O library definitions.         */
#    include  <unistd.h>            /* UNIX I/O definitions.            */
#else
#    include  <unistd.h>            /* UNIX I/O definitions.            */
#    include  <sys/ioctl.h>     	/* I/O control definitions.         */
#endif

#include  "str_util.h"              /* String manipulation functions.   */
#include  "vperror.h"               /* VPERROR() definitions.           */
#include  "xqt_util.h"              /* Shell execution utilities.       */

/*----------------------------------------------------------------------*/
/* XQT Stream Descriptor - describes an established execution stream.   */
/* In particular, a stream's descriptor identifies the interprocess     */
/* communications channels connected to the shell subprocess's standard */
/* input and output streams.                                            */
/*----------------------------------------------------------------------*/

typedef  struct  _XqtStream {
    char  *shellName ;              /* Name of shell executing commands.*/
    int  processID ;                /* Subprocess's process ID.         */
    int  inputPipe[2] ;             /* Subprocess's standard input stream.*/
    FILE  *input ;                  /* File pointer for input pipe.     */
    int  outputPipe[2] ;            /* Subprocess's standard output stream.*/
    FILE  *output ;                 /* File pointer for output pipe.    */
#if defined(vms)
    int  channel ;                  /* VMS channel assigned to output pipe's mailbox. */
#endif
    char  *lastCommand ;            /* Last command written to subprocess.*/
}  _XqtStream ;

/* C Preprocessor macro definitions.                                    */

#define  ASSIGN(desc, string) \
    { desc.dsc$w_length = (string == NULL) ? 0 : strlen (string) ; \
      desc.dsc$b_dtype = DSC$K_DTYPE_T ; \
      desc.dsc$b_class = DSC$K_CLASS_S ; \
      desc.dsc$a_pointer = (string == NULL) ? "" : string ; \
    }

#endif

