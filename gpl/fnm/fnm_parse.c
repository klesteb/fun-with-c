
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

char  *fnm_parse (

#    if  __STDC__
        const  FileName  fileName,
        FnmPart  part)
#    else
        fileName, part)

        FileName  fileName;
        FnmPart  part;
#    endif

{
/*
 * Function: fnm_parse.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_parse() function returns the requested part of a file name, 
 *    e.g., the directory, the name, the extension, etc.
 *
 *    Invocation:
 *
 *        value = fnm_parse(fileName, part);
 *
 *    where
 *
 *        <fileName>      - I
 *            is the file name handle returned by fnm_create().
 *
 *        <part>          - I
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
 *        <value>         - O
 *            returns the requested part of the file name; "" is returned
 *            in the event of an error or if the requested part is missing.
 *            The returned string is private to the file name and it should
 *            not be modified or deleted; it should not be used after the
 *            file name is deleted.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (fileName == NULL) {

        errno = EINVAL;
        vperror("(fnm_parse) NULL file handle: ");
        return("");

    }

    switch (part) {
        case FnmPath:
            return ((fileName->path == NULL) ? "" : fileName->path);

        case FnmNode:
            return ((fileName->node == NULL) ? "" : fileName->node);

        case FnmDirectory:
            return ((fileName->directory == NULL) ? "" : fileName->directory);

        case FnmFile:
            return ((fileName->file == NULL) ? "" : fileName->file);

        case FnmName:
            return ((fileName->name == NULL) ? "" : fileName->name);

        case FnmExtension:
            return ((fileName->extension == NULL) ? "" : fileName->extension);

        case FnmVersion:
            return ((fileName->version == NULL) ? "" : fileName->version);

        default:
            return ("");

    }

}

