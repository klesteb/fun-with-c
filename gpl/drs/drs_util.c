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
@(#)  FILE: drs_util.c  RELEASE: 1.7  DATE: 07/12/96, 15:36:12
*/
/*******************************************************************************

File:

    drs_util.c

    Directory Scanning Utilities.


Author:    Alex Measday, ISI


Purpose:

    The functions in the DRS_UTIL package are used to scan the names of the
    files in a directory.  Wildcards can be used to filter out unwanted files.

    The following example prints out the names of the ".c" files in a
    directory:

        #include  <stdio.h>		-- Standard I/O definitions.
        #include  "drs_util.h"		-- Directory scanning utilities.
        ...
        int  main (int argc, char *argv[])
        {
            char  *fileName ;
            DirectoryScan  scan ;

            drs_create ("*.c", &scan) ;
            fileName = drs_first (scan) ;
            while (fileName != NULL) {
                printf ("C File: %s\n", fileName) ;
                fileName = drs_next (scan) ;
            }
            drs_destroy (scan) ;
        }

    Alternatively, you can call drs_get() to get the I-th name in the directory:

            ...
            for (i = 0 ;  i < drs_count (scan) ;  i++)
                printf ("C File: %s\n", drs_get (scan, i)) ;
            ...


Notes:

    This package is derived from and supersedes my FSEARCH() routine.
    Farewell, VMS!


Public Procedures:

    drs_count() - returns the number of files in a directory scan.
    drs_create() - creates a directory scan.
    drs_destroy() - destroys a directory scan.
    drs_first() - gets the first entry in the directory.
    drs_get() - gets the I-th entry in the directory.
    drs_next() - gets the next entry in the directory.

Private Procedures:

    drsCompare() - compares two file names for sorting purposes.

*******************************************************************************/


#include  <errno.h>			/* System error definitions. */
#include  <limits.h>			/* Maximum/minimum value definitions. */
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
#    include  <stdarg.h>		/* Variable-length argument lists. */
#else
#    include  <varargs.h>		/* Variable-length argument lists. */
#endif
#include  <stdio.h>			/* Standard I/O definitions. */
#include  <stdlib.h>			/* Standard C library definitions. */
#include  <string.h>			/* C Library string functions. */
#include  <unistd.h>			/* UNIX-specific definitions. */

#include  <dirent.h>			/* Directory entry definitions. */

#ifdef VXWORKS
#    include  <ioLib.h>			/* VxWorks I/O library definitions. */
#    define  exit  return
#    define  getcwd(path,size)  getwd (path)
#endif

#include  "rex_util.h"			/* Regular expression definitions. */
#include  "str_util.h"			/* String manipulation functions. */
#include  "vperror.h"			/* VPERROR() definitions. */
#include  "drs_util.h"			/* Directory scanning utilities. */


/*******************************************************************************
    Directory Scan - contains information about a directory scan.
*******************************************************************************/

typedef  struct  _DirectoryScan {
    char  *pathname ;			/* Original wildcard pathname. */
    DIR  *directory ;			/* Directory name. */
    CompiledRE  compiledRE ;		/* Compiled wildcard specification. */
    int  numFiles ;			/* # of matching file names. */
    char  **fileName ;			/* List of matching file names. */
    int  nextFile ;			/* Index of next file in scan. */
}  _DirectoryScan ;


int  drs_util_debug = 0 ;		/* Global debug switch (1/0 = yes/no). */


/*******************************************************************************
    Private Functions.
*******************************************************************************/

static  int  drsCompare (
#    if __STDC__
        const  void  *p1,
        const  void  *p2
#    endif
    ) ;

/*******************************************************************************

Procedure:

    drs_count ()

    Get the Number of Files in a Directory Scan.


Purpose:

    Function drs_count() returns the number of files in a directory scan that
    matched the wildcard file specification.


    Invocation:

        numFiles = drs_count (scan) ;

    where:

        <scan>	- I
            is the directory scan handle returned by drs_create().
        <count>	- O
            returns the number of files that matched the wildcard file
            specification.

*******************************************************************************/


int  drs_count (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{

    if (scan == NULL) {
        errno = EINVAL ;
        vperror ("(drs_count) NULL scan handle: ") ;
        return (0) ;
    }

    return (scan->numFiles) ;

}

/*******************************************************************************

Procedure:

    drs_create ()

    Create a Directory Scan.


Purpose:

    Function drs_create() creates a directory scan.


    Invocation:

        status = drs_create (pathname, &scan) ;

    where:

        <pathname>	- I
            is the wildcard specification for the directory being scanned.
        <scan>		- O
            returns a handle for the directory scan.  This handle is used
            in calls to the other DRS functions.
        <status>	- O
            returns the status of initiating the directory scan, zero if
            no errors occurred and ERRNO otherwise.

*******************************************************************************/


int  drs_create (

#    if __STDC__
        const  char  *pathname,
        DirectoryScan  *scan)
#    else
        pathname, scan)

        char  *pathname ;
        DirectoryScan  *scan ;
#    endif

{    /* Local variables. */
    char  directoryName[PATH_MAX+1], *fileSpec, fullFileName[PATH_MAX+1], *s ;
    int  status ;
    struct  dirent  *d ;




/* Create the directory scan context structure. */

    *scan = (_DirectoryScan *) malloc (sizeof (_DirectoryScan)) ;
    if (*scan == NULL) {
        vperror ("(drs_create) Error allocating scan context for %s.\nmalloc: ",
                 pathname) ;
        return (errno) ;
    }

    str_env(pathname, -1, fullFileName, sizeof fullFileName) ;
    (*scan)->pathname = strdup (fullFileName) ;
    if ((*scan)->pathname == NULL) {
        vperror ("(drs_create) Error duplicating pathname: %s\nstrdup: ",
                 fullFileName) ;
        status = errno ;  drs_destroy (*scan) ;  errno = status ;
        return (errno) ;
    }

    (*scan)->directory = NULL ;
    (*scan)->compiledRE = NULL ;
    (*scan)->numFiles = 0 ;
    (*scan)->fileName = NULL ;
    (*scan)->nextFile = 0 ;


/* Separate the directory name from the wildcard file specification. */

    strcpy (directoryName, (*scan)->pathname) ;		/* Directory name. */
    s = strrchr (directoryName, '/') ;
    if (s == NULL)
        getcwd (directoryName, sizeof directoryName) ;
    else
        *s = '\0' ;

    fileSpec = strrchr ((*scan)->pathname, '/') ;	/* File specification. */
    if (fileSpec == NULL)
        fileSpec = (*scan)->pathname ;
    else
        fileSpec++ ;


/* Compile a regular expression (RE) for the wildcard file specification. */

    if (rex_compile (rex_wild (fileSpec), &(*scan)->compiledRE)) {
        vperror ("(drs_create) Error compiling regular expression for \"%s\": %s\nrex_compile: ",
                 pathname, rex_error_text) ;
        status = errno ;  drs_destroy (*scan) ;  errno = status ;
        return (errno) ;
    }


/* Make an initial pass over the directory to determine how many file names
   match the RE. */

    (*scan)->directory = opendir (directoryName) ;
    if ((*scan)->directory == (DIR *) NULL) {
        vperror ("(drs_create) Error opening directory: %s\nopendir: ",
                 directoryName) ;
        status = errno ;  drs_destroy (*scan) ;  errno = status ;
        return (errno) ;
    }

    (*scan)->numFiles = 0 ;
    while ((d = readdir ((*scan)->directory)) != NULL) {
        if ((strcmp (d->d_name, ".") == 0) || (strcmp (d->d_name, "..") == 0))
            continue ;		/* Ignore current and parent entries. */
        if (rex_match (d->d_name, (*scan)->compiledRE, NULL, NULL, 0))
            (*scan)->numFiles++ ;
    }


/* Allocate an array to hold the list of matching file names. */

    if ((*scan)->numFiles > 0) {
        (*scan)->fileName = (char **) calloc ((*scan)->numFiles,
                                              sizeof (char *)) ;
        if ((*scan)->fileName == NULL) {
            vperror ("(drs_create) Error allocating %d-element file list for %s.\ncalloc: ",
                     (*scan)->numFiles, pathname) ;
            status = errno ;  drs_destroy (*scan) ;  errno = status ;
            return (errno) ;
        }
    }


/* Make a second pass over the directory and construct a list of the matching
   file names. */

    (*scan)->numFiles = 0 ;
    rewinddir ((*scan)->directory) ;
    while ((d = readdir ((*scan)->directory)) != NULL) {
        if ((strcmp (d->d_name, ".") == 0) || (strcmp (d->d_name, "..") == 0))
            continue ;		/* Ignore current and parent entries. */
        if (rex_match (d->d_name, (*scan)->compiledRE, NULL, NULL, 0)) {
            sprintf (fullFileName, "%s/%s", directoryName, d->d_name) ;
            (*scan)->fileName[(*scan)->numFiles] = strdup (fullFileName) ;
            if ((*scan)->fileName[(*scan)->numFiles] == NULL) {
                vperror ("(drs_create) Error duplicating full file name: %s\nstrdup: ",
                         fullFileName) ;
                status = errno ;  drs_destroy (*scan) ;  errno = status ;
                return (errno) ;
            }
            (*scan)->numFiles++ ;
        }
    }


/* Now, sort the list of matching file names. */

    qsort ((*scan)->fileName, (*scan)->numFiles, sizeof (char *), drsCompare) ;


    if (drs_util_debug)  printf ("(drs_create) %s  -  %d files.\n",
                                 (*scan)->pathname, (*scan)->numFiles) ;

    return (0) ;

}

/*******************************************************************************

Procedure:

    drs_destroy ()

    Destroy a Directory Scan.


Purpose:

    Function drs_destroy() destroys a directory scan.


    Invocation:

        status = drs_destroy (scan) ;

    where:

        <scan>		- I
            is the directory scan handle returned by drs_create().
        <status>	- O
            returns the status of terminating the directory scan, zero
            if no errors occurred and ERRNO otherwise.

*******************************************************************************/


int  drs_destroy (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{    /* Local variables. */
    int  i ;




    if (scan == NULL) {
        errno = EINVAL ;
        vperror ("(drs_destroy) NULL scan handle: ") ;
        return (errno) ;
    }

    if (drs_util_debug)
        printf ("(drs_destroy) Terminating scan of %s.\n", scan->pathname) ;

/* Delete it. */

    if (scan->pathname != NULL)  free (scan->pathname) ;
    if (scan->directory != NULL)  closedir (scan->directory) ;
    if (scan->compiledRE != NULL)  rex_delete (scan->compiledRE) ;

/* Delete it. */

    if (scan->fileName != NULL) {
        for (i = 0 ;  i < scan->numFiles ;  i++)
            free (scan->fileName[i]) ;
        free (scan->fileName) ;
    }

/* Delete it. */

    free (scan) ;

    return (0) ;

}

/*******************************************************************************

Procedure:

    drs_first ()

    Get the First File in a Directory Scan.


Purpose:

    Function drs_first() returns the first matching file in a directory scan.


    Invocation:

        fileName = drs_first (scan) ;

    where:

        <scan>		- I
            is the directory scan handle returned by drs_create().
        <fileName>	- O
            returns the full pathname of the first matching file.  The file
            name is stored in memory belonging to the DRS_UTIL package and
            it should not be overwritten or deallocated by the caller.  NULL
            is returned if there are no matching files.

*******************************************************************************/


char  *drs_first (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{
    return (drs_get (scan, 0)) ;
}

/*******************************************************************************

Procedure:

    drs_get ()

    Get the I-th File in a Directory Scan.


Purpose:

    Function drs_get() returns the I-th matching file in a directory scan.


    Invocation:

        fileName = drs_get (scan, index) ;

    where:

        <scan>		- I
            is the directory scan handle returned by drs_create().
        <index>		- I
            is the index, 0 .. N-1, of the desired file.
        <fileName>	- O
            returns the full pathname of the indicated matching file.
            The file name is stored in memory belonging to the DRS_UTIL
            package and it should not be overwritten or deallocated by
            the caller.  NULL is returned if an invalid index is specified.

*******************************************************************************/


char  *drs_get (

#    if __STDC__
        DirectoryScan  scan,
        int  index)
#    else
        scan, index)

        DirectoryScan  scan ;
        int  index ;
#    endif

{

    if (scan == NULL) {
        errno = EINVAL ;
        vperror ("(drs_get) NULL scan handle: ") ;
        return (NULL) ;
    }

    if ((index < 0) || (index >= scan->numFiles)) {
        return (NULL) ;
    } else {
        scan->nextFile = index + 1 ;
        return (scan->fileName[index]) ;
    }

}

/*******************************************************************************

Procedure:

    drs_next ()

    Get the Next File in a Directory Scan.


Purpose:

    Function drs_next() returns the next matching file in a directory scan.


    Invocation:

        fileName = drs_next (scan) ;

    where:

        <scan>		- I
            is the directory scan handle returned by drs_create().
        <fileName>	- O
            returns the full pathname of the next matching file.  The file
            name is stored in memory belonging to the DRS_UTIL package and
            it should not be overwritten or deallocated by the caller.  NULL
            is returned if there are no matching files.

*******************************************************************************/


char  *drs_next (

#    if __STDC__
        DirectoryScan  scan)
#    else
        scan)

        DirectoryScan  scan ;
#    endif

{
    return (drs_get (scan, (scan == NULL) ? 0 : scan->nextFile)) ;
}

/*******************************************************************************

Procedure:

    drsCompare ()

    Compare File Names.


Purpose:

    Function drsCompare() is called by the QSORT(3) function to compare
    two file names.


    Invocation:

        comparison = drsCompare (p1, p2) ;

    where:

        <p1>		- I
        <p2>		- I
            are (VOID *) pointers to the (CHAR *) pointers that point to
            the two file names being compared.
        <comparison>	- O
            returns -1, 0, or +1 if the first file name is lexicographically
            less than, equal to, or greater than the second file name.

*******************************************************************************/


static  int  drsCompare (

#    if __STDC__
        const  void  *p1,
        const  void  *p2)
#    else
        p1, p2)

        void  *p1 ;
        void  *p2 ;
#    endif

{
    return (strcmp (*((char **) p1), *((char **) p2))) ;
}

#ifdef  TEST

/*******************************************************************************

    Program to test the DRS_UTIL() functions.

    Under UNIX,
        compile and link as follows:
            % cc -g -DTEST drs_util.c -I<... includes ...> <libraries ...>
        and run with the following command line:
            % a.out <wildcard_file_spec>

    Under VxWorks,
        compile and link as follows:
            % cc -g -c -DTEST -DVXWORKS drs_util.c -I<... includes ...> \
                       -o testDRS.o
            % ld -r testDRS.o <libraries ...> -o testDRS.vx.o
        load as follows:
            -> ld <testDRS.vx.o
        and run with the following command line:
            -> testDRS.vx.o "<wildcard_file_spec>"

*******************************************************************************/

#ifdef VXWORKS

testDRS (fs)
    char  *fs ;
{    /* Local variables. */

#else

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{    /* Local variables. */
    char  *fs = argv[1] ;

#endif

    char  *s ;
    DirectoryScan  scan ;


    drs_util_debug = 1 ;
    vperror_print = 1 ;

    if (drs_create (fs, &scan))  exit (errno) ;

    s = drs_first (scan) ;
    while (s != NULL) {
        printf ("%s\n", s) ;
        s = drs_next (scan) ;
    }

    drs_destroy (scan) ;

}
#endif
