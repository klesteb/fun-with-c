
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

FileName  fnm_create(

#    if  __STDC__
        int expand,
        const  char  *fileSpec,
        ...)
#    else
        expand, fileSpec, va_alist)

        int expand;
        char  *fileSpec;
        va_dcl
#    endif

{
/*
 * Function: fnm_create.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_create() function creates a file name.
 *
 *    Invocation:
 *
 *        fileName = fnm_create(expand, [fileSpec1, ..., fileSpecN,] NULL);
 *
 *    where
 *
 *        <expand>        - I
 *            A flag to indicate wither Unix environment variables should
 *            be expanded in the filespec.
 *
 *        <fileSpec1>     - I
 *        <fileSpecN>     - I
 *            Are the file specfications used to construct the resulting file
 *            name.  Each file specification is a UNIX pathname containing
 *            one or more of the components of a pathname (e.g., the directory,
 *            the extension, the version number, etc.).  Missing components in
 *            the result are filled in from the file specifications as they are
 *            examined in left-to-right order. The NULL argument marks the end
 *            of the file specification list.
 *
 *        <fileName>      - O
 *            Returns a handle that can be used in other FNM_UTIL calls. NULL
 *            is returned in the event of an error.
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
    char  pathname[PATH_MAX];
    FileName  defaults, newResult, result;
    int  status;
	int len;

/*
 * Main part of function.
 */

    /* Process each file specification in the argument list.         */

    result = NULL;

#if __STDC__
    va_start(ap, fileSpec);
#else
    va_start(ap);
#endif

    while (fileSpec != NULL) {

        if ((defaults = fnm_new(fileSpec, expand)) == NULL) {

            vperror("(fnm_create) Error creating defaults: %s\nfnm_new: ", fileSpec);
            return(NULL);

        }

        fnm_locate_parts(defaults) ;

        newResult = fnm_fill_parts(expand, result, defaults);
        status = errno;
        if (result != NULL)  fnm_destroy(result);
        fnm_destroy(defaults);

        if (newResult == NULL) {

            errno = status;
            vperror("(fnm_create) Error creating intermediate result.\nfnm_new: ");
            return(NULL);

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

		vperror("(fnm_create) Error creating system defaults: %s\nfnm_new: ", pathname) ;
        return(NULL);

    }

    fnm_locate_parts(defaults);

    newResult = fnm_fill_parts(expand, result, defaults);
    status = errno;
    if (result != NULL)  fnm_destroy(result);
    fnm_destroy(defaults);

    if (newResult == NULL) {

        errno = status;
        vperror("(fnm_create) Error creating final result.\nfnm_new: ");
        return(NULL);

    }

    result = newResult;

    return(result);

}

