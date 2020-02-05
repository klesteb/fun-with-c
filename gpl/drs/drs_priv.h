
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       drs_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the DRS package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the DRS utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef DRS_PRIV_H
#define DRS_PRIV_H 1

#include  <errno.h>                 /* System error definitions.         */
#include  <limits.h>                /* Maximum/minimum value definitions.*/
#ifndef PATH_MAX
#    if defined(vms)					
#        include  <nam.h>           /* RMS name block (NAM) definitions.*/
#        define  PATH_MAX  NAM$C_MAXRSS
#    else
#        include  <sys/param.h>     /* System parameters.               */
#        define  PATH_MAX  MAXPATHLEN
#    endif
#endif
#if __STDC__
#    include  <stdarg.h>            /* Variable-length argument lists.  */
#else
#    include  <varargs.h>           /* Variable-length argument lists.  */
#endif
#include  <stdio.h>                 /* Standard I/O definitions.        */
#include  <stdlib.h>                /* Standard C library definitions.  */
#include  <string.h>                /* C Library string functions.      */
#include  <unistd.h>                /* UNIX-specific definitions.       */

#include  <dirent.h>                /* Directory entry definitions.     */

#ifdef VXWORKS
#    include  <ioLib.h>             /* VxWorks I/O library definitions. */
#    define  exit  return
#    define  getcwd(path,size)  getwd (path)
#endif

#include  "rex_util.h"              /* Regular expression definitions.  */
#include  "str_util.h"              /* String manipulation functions.   */
#include  "vperror.h"               /* VPERROR() definitions.           */
#include  "drs_util.h"              /* Directory scanning utilities.    */

/*----------------------------------------------------------------------*/
/* Directory Scan - contains information about a directory scan.        */
/*----------------------------------------------------------------------*/

typedef  struct  _DirectoryScan {
    char  *pathname;                /* Original wildcard pathname.      */
    DIR  *directory;                /* Directory name.                  */
    CompiledRE  compiledRE;         /* Compiled wildcard specification. */
    int  numFiles;                  /* # of matching file names.        */
    char  **fileName;               /* List of matching file names.     */
    int  nextFile;                  /* Index of next file in scan.      */
}  _DirectoryScan;

/*----------------------------------------------------------------------*/
/* Priviate routines.                                                   */
/*----------------------------------------------------------------------*/

extern int drsCompare(
#    if __STDC__
        const  void  *p1,
        const  void  *p2
#    endif
    );

#endif

