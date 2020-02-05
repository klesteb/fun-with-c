
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

int  fnm_destroy (

#    if  __STDC__
        FileName  fileName)
#    else
        fileName)

        FileName  fileName;
#    endif

{
/*
 * Function: fnm_destroy.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *   The fnm_destroy() function destroys a file name.
 *
 *   Invocation:
 *
 *      status = fnm_destroy(fileName);
 *
 *   where
 *
 *      <fileName>            - I
 *         is the file name handle returned by fnm_create().
 *
 *      <status>              - O
 *         returns the status of destroying the file name, zero if
 *         there were no errors and ERRNO otherwise.
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
        vperror("(fnm_destroy) NULL file handle: ");
        return(errno);

    }

    if (fileName->path != NULL)  free(fileName->path);
    if (fileName->node != NULL)  free(fileName->node);
    if (fileName->directory != NULL)  free(fileName->directory);
    if (fileName->file != NULL)  free(fileName->file);
    if (fileName->name != NULL)  free(fileName->name);
    if (fileName->extension != NULL)  free(fileName->extension);
    if (fileName->version != NULL)  free(fileName->version);

    free(fileName);

    return(0);

}

