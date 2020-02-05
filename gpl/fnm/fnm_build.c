
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

#include "fnm_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *fnm_build(

#    if  __STDC__
        int expand,
        FnmPart  part,
        const  char  *fileSpec,
        ...)
#    else
        expand, part, fileSpec, va_alist)

        int expand;
        FnmPart  part;
        char  *fileSpec;
        va_dcl
#    endif

{
/*
 * Function: fnm_build.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_build() function builds a pathname from one or more file
 *    specifications. fnm_build() is essentially an encapsulation of
 *    the following code fragment:
 *
 *        char  pathname[PATH_MAX] ;
 *        FileName  fname = fnm_create(TRUE, fileSpec1, ..., fileSpecN, NULL) ;
 *        ...
 *        strcpy(pathname, fnm_parse(fname, FnmPath)) ;
 *        fnm_destroy(fname) ;
 *
 *    I got tired of coding up variations of this every time I needed to
 *    build a full pathname, so I made a copy of fnm_create() and modified
 *    it to return a character-string pathname instead of a FileName handle.
 *
 *    Invocation:
 *
 *        pathname = fnm_build(expand, part, [fileSpec1, ..., fileSpecN,] NULL);
 *
 *    where
 *
 *        <expand>            - I
 *            A flag to indicate wither to expand Unix environment 
 *            variables in the fileSpec.
 *
 *        <part>              - I
 *            specifies which part of the file name you want returned:
 *                FnmPath - "node:/directory(ies)/name.extension.version"
 *                FnmNode - "node:"
 *                FnmDirectory - "/directory(ies)/"
 *                FnmFile - "name[.extension[.version]]"
 *                FnmName - "name"
 *                FnmExtension - ".extension"
 *                FnmVersion - ".version"
 *            (These enumerated values are defined in "fnm_util.h".)
 *
 *        <fileSpec1>         - I
 *        <fileSpecN>         - I
 *            are the file specfications used to construct the resulting file
 *            name.  Each file specification is a UNIX pathname containing
 *            one or more of the components of a pathname (e.g., the directory,
 *            the extension, the version number, etc.).  Missing components in
 *            the result are filled in from the file specifications as they are
 *            examined in left-to-right order.  The NULL argument marks the end
 *            of the file specification list.
 *
 *        <pathname>          - O
 *            returns the pathname constructed from the file specifications;
 *            "" is returned in the event of an error.  The returned string
 *            is private to this routine and it should be used or duplicated
 *            before calling fnm_build() again.
 *
 * Modification History
 *
 *    K.Esteb 23-Apr-2002
 *        Added the "expand" parameter to signal wither to expand Unix
 *        environment variabls.
 *
 * Variables Used
 */

    va_list  ap;
    int  status;
    int  len;
    FileName  defaults;
    FileName newResult;
    FileName result;
    static char pathname[PATH_MAX];

/*
 * Main part of function.
 */

    result = NULL;

    /* Process each file specification in the argument list.            */

#if __STDC__
    va_start(ap, fileSpec);
#else
    va_start(ap);
#endif

    while (fileSpec != NULL) {

        if ((defaults = fnm_new(fileSpec, expand)) == NULL) {

            vperror("(fnm_build) Error creating defaults: %s\nfnm_new: ", fileSpec) ;
            return("");

        }

        fnm_locate_parts(defaults) ;

        newResult = fnm_fill_parts(expand, result, defaults);
        status = errno;
        if (result != NULL)  fnm_destroy(result);
        fnm_destroy(defaults);

        if (newResult == NULL) {

            errno = status;
            vperror("(fnm_build) Error creating intermediate result.\nfnm_new: ");
            return("");

        }

        result = newResult;

        fileSpec = va_arg(ap, const char *);

    }

    va_end(ap);

    /* Fill in missing fields with the system defaults.                 */

#ifdef VMS
    /* On VMS, by default, getcwd() returns the current working         */
	/* directory in VMS syntax. We need to change that to UNIX syntax.  */
    getcwd(pathname, sizeof pathname, 0);
#else
    getcwd(pathname, sizeof pathname);
#endif

    len = strlen(pathname) - 1;
    if (pathname[len] != '/') strcat(pathname, "/");

    if ((defaults = fnm_new(pathname, expand)) == NULL) {

        vperror("(fnm_build) Error creating system defaults: %s\nfnm_new: ", pathname);
        return("");

    }

    fnm_locate_parts(defaults);

    newResult = fnm_fill_parts(expand, result, defaults);
    status = errno;
    if (result != NULL)  fnm_destroy(result);
    fnm_destroy(defaults);

    if (newResult == NULL) {

        errno = status;
        vperror("(fnm_build) Error creating final result.\nfnm_new: ");
        return("");

    }

    result = newResult;

    /* Return the full pathname to the caller.                          */

    strcpy(pathname, fnm_parse(result, part));
    fnm_destroy(result);

    return(pathname);

}

