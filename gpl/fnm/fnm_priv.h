
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       fnm_priv.h                                                   */
/*                                                                           */
/* FACILITY:    Private variables and structures for the FNM package.        */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the FNM utilities.                     */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef FNM_PRIV_H
#define FNM_PRIV_H 1

#include  <errno.h>                     /* System error definitions. */
#include  <limits.h>                    /* Maximum/minimum value definitions. */
#ifndef PATH_MAX
#    if defined(vms)
#        include  <nam.h>               /* RMS name block (NAM) definitions. */
#        define  PATH_MAX  NAM$C_MAXRSS
#    else
#        include  <sys/param.h>         /* System parameters. */
#        define  PATH_MAX  MAXPATHLEN
#    endif
#endif
#if __STDC__
#    include  <stdarg.h>                /* Variable-length argument lists. */
#else
#    include  <varargs.h>               /* Variable-length argument lists. */
#endif
#include  <stdio.h>                     /* Standard I/O definitions. */
#include  <stdlib.h>                    /* Standard C Library definitions. */
#include  <string.h>                    /* C Library string functions. */
#if defined(VMS)
#    include  <file.h>                  /* File definitions. */
#    include  <unixio.h>                /* VMS-emulation of UNIX I/O. */
#    include  <unistd.h>
#elif defined(VXWORKS)
#    include  <ioLib.h>                 /* I/O library definitions. */
#    define  getcwd(path,size)  getwd (path)
#elif defined(WIN32)
#    include  <direct.h>                /* Directory control functions. */
#    include  <io.h>                    /* Low-level I/O definitions. */
#else
#    include  <fcntl.h>                 /* File control definitions. */
#    include  <unistd.h>                /* UNIX I/O definitions. */
#endif
#include  <sys/stat.h>                  /* File status definitions. */

#include  "gpl/str_util.h"              /* String manipulation functions. */
#include  "gpl/vperror.h"               /* VPERROR() definitions. */
#include  "gpl/fnm_util.h"              /* Filename utilities. */

/*----------------------------------------------------------------------*/
/*  File Name - contains the fully-expanded file specification as well  */
/*  as the individual components of the file specification.             */
/*----------------------------------------------------------------------*/

typedef  struct  _FileName {
    char  *path;                       /* Fully-expanded file specification. */
    char  *node;                       /* "node:" */
    char  *directory;                  /* "/directory(ies)/" */
    char  *file;                       /* "name.extension.version" */
    char  *name;                       /* "name" */
    char  *extension;                  /* ".extension" */
    char  *version;                    /* ".version" */
}  _FileName;

/*----------------------------------------------------------------------*/
/*  Private Functions.                                                  */
/*----------------------------------------------------------------------*/

extern FileName  fnm_fill_parts(
#    if __STDC__
        int expand,
        const  FileName  name1,
        const  FileName  name2
#    endif
    );

extern int  fnm_locate_parts(
#    if __STDC__
        FileName  fileSpec
#    endif
    );

extern FileName  fnm_new(
#    if __STDC__
        const  char  *pathname,
        int expand
#    endif
    );

#endif

