
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

#include "drs_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  drs_create(

#    if __STDC__
        const  char  *pathname,
        DirectoryScan  *scan)
#    else
        pathname, scan)

        char  *pathname ;
        DirectoryScan  *scan ;
#    endif

{
/*
 * Function: drs_create.c
 * Version : 1.0
 * Created : 7-Dec-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function drs_create() creates a directory scan.
 *
 *    Invocation:
 *
 *        status = drs_create(pathname, &scan);
 *
 *    where:
 *
 *        <pathname>            - I
 *            Is the wildcard specification for the directory being scanned.
 *
 *        <scan>                - O
 *            Returns a handle for the directory scan.  This handle is used
 *            in calls to the other DRS functions.
 *
 *        <status>              - O
 *            Returns the status of initiating the directory scan, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  directoryName[PATH_MAX+1], *fileSpec, fullFileName[PATH_MAX+1], *s;
    int  status;
    struct  dirent  *d;

/*
 * Main part of function.
 */

    /* Create the directory scan context structure.                     */

    if ((*scan = (_DirectoryScan *)malloc(sizeof(_DirectoryScan))) == NULL) {

        vperror("(drs_create) Error allocating scan context for %s.\nmalloc: ",
                pathname);
        return(errno);

    }

    str_env(pathname, -1, fullFileName, sizeof fullFileName);
    (*scan)->pathname = strdup(fullFileName);

    if ((*scan)->pathname == NULL) {

        vperror("(drs_create) Error duplicating pathname: %s\nstrdup: ",
                fullFileName);
        status = errno;
        drs_destroy(*scan);
        errno = status;
        return(errno);

    }

    (*scan)->directory = NULL ;
    (*scan)->compiledRE = NULL ;
    (*scan)->numFiles = 0 ;
    (*scan)->fileName = NULL ;
    (*scan)->nextFile = 0 ;

    /* Separate the directory name from the wildcard file specification.*/

    strcpy(directoryName, (*scan)->pathname);        /* Directory name. */
    s = strrchr(directoryName, '/');
    if (s == NULL) {

        getcwd(directoryName, sizeof directoryName);

    } else *s = '\0';

    fileSpec = strrchr((*scan)->pathname, '/');        /* File specification. */
    if (fileSpec == NULL) {

        fileSpec = (*scan)->pathname;

    } else fileSpec++;

    /* Compile a regular expression (RE) for the wildcard file          */
    /* specification.                                                   */

    if (rex_compile(rex_wild(fileSpec), &(*scan)->compiledRE)) {

        vperror("(drs_create) Error compiling regular expression for \"%s\": %s\nrex_compile: ",
                 pathname, rex_error_text);
        status = errno;
        drs_destroy(*scan);
        errno = status;
        return(errno);

    }

    /* Make an initial pass over the directory to determine how many    */
    /* file names match the RE.                                         */

    (*scan)->directory = opendir(directoryName);
    if ((*scan)->directory == (DIR *)NULL) {

        vperror("(drs_create) Error opening directory: %s\nopendir: ",
                directoryName);
        status = errno;
        drs_destroy(*scan);  
        errno = status;
        return(errno);

    }

    (*scan)->numFiles = 0;

    while ((d = readdir((*scan)->directory)) != NULL) {

        /* Ignore current and parent entries.                           */

        if ((strcmp(d->d_name, ".") == 0) || 
            (strcmp (d->d_name, "..") == 0)) continue;        

        if (rex_match(d->d_name, (*scan)->compiledRE, NULL, NULL, 0))
            (*scan)->numFiles++;

    }

    /* Allocate an array to hold the list of matching file names.       */

    if ((*scan)->numFiles > 0) {

        (*scan)->fileName = (char **)calloc((*scan)->numFiles, sizeof(char *));
        if ((*scan)->fileName == NULL) {

            vperror("(drs_create) Error allocating %d-element file list for %s.\ncalloc: ",
                    (*scan)->numFiles, pathname);
            status = errno;  
            drs_destroy(*scan);  
            errno = status;
            return(errno);

        }

    }

    /* Make a second pass over the directory and construct a list of    */
    /* the matching file names.                                         */

    (*scan)->numFiles = 0;
    rewinddir((*scan)->directory);

    while ((d = readdir((*scan)->directory)) != NULL) {

        /* Ignore current and parent entries.                           */

        if ((strcmp(d->d_name, ".") == 0) || 
            (strcmp(d->d_name, "..") == 0)) continue;
        
        if (rex_match(d->d_name, (*scan)->compiledRE, NULL, NULL, 0)) {

            sprintf(fullFileName, "%s/%s", directoryName, d->d_name);
            (*scan)->fileName[(*scan)->numFiles] = strdup(fullFileName);

            if ((*scan)->fileName[(*scan)->numFiles] == NULL) {

                vperror("(drs_create) Error duplicating full file name: %s\nstrdup: ",
                        fullFileName);
                status = errno;
                drs_destroy(*scan);
                errno = status;
                return(errno);

            }

            (*scan)->numFiles++;

        }

    }

    /* Now, sort the list of matching file names.                       */

    qsort((*scan)->fileName, (*scan)->numFiles, sizeof(char *), drsCompare);

    if (drs_util_debug)  printf("(drs_create) %s  -  %d files.\n",
                                (*scan)->pathname, (*scan)->numFiles);

    return(0);

}

