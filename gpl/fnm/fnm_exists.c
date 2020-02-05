
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

int  fnm_exists (

#    if  __STDC__
        const  FileName  fileName)
#    else
        fileName)

        FileName  fileName ;
#    endif

{
/*
 * Function: fnm_exists.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_exists() function checks to see if the file referenced by 
 *    a file name actually exists.
 *
 *    Invocation:
 *
 *        exists = fnm_exists(fileName);
 *
 *    where
 *
 *        <fileName>          - I
 *            Is the file name handle returned by fnm_create().
 *
 *        <exists>            - O
 *            Returns true (1) if the referenced file exists and false (0)
 *            if it doesn't exist.
 *
 * Modification History
 *
 * Variables Used
 */

    struct  stat  fileInfo;

/*
 * Main part of function.
 */

    if (fileName == NULL) {

        errno = EINVAL;
        vperror("(fnm_exists) NULL file handle: ");
        return(0);

    }

    if (stat(fileName->path, &fileInfo)) {

        switch (errno) {
            case EACCES:                        /* Expected errors.     */
            case ENOENT:
            case ENOTDIR:
                break;

            default:                            /* Unexpected errors.   */
                vperror("(fnm_exists) Error getting information for %s.\nstat: ",
                        fileName->path);
        }

        return(0);                              /* Not found.           */

    }

    return(1);                                  /* Found.               */

}

