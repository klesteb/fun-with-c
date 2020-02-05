
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

#include "str_priv.h"

/*----------------------------------------------------------------------*/

char  *strdup(

#    if __STDC__
        const  char  *string)
#    else
        string)

        char  *string ;
#    endif

{
/*
 * Function: strdup.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function strdup() duplicates a null-terminated string. strdup() is
 *    supported by many C libraries, but it is not part of the ANSI C library.
 *
 *    Invocation:
 *
 *        duplicate = strdup(string);
 *
 *    where:
 *
 *        <string>            - I
 *            is the string to be duplicated.
 *
 *        <duplicate>          - O
 *            returns a MALLOC(3)ed copy of the input string.  The caller
 *            is responsible for FREE(3)ing the duplicate string.  NULL is
 *            returned in the event of an error.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *duplicate;

/*
 * Main part of function.
 */

    if (string == NULL) {

        errno = EINVAL;
        vperror("(strdup) NULL string: ");
        return(NULL);

    }

    if ((duplicate = malloc(strlen(string) + 1)) == NULL) {

        vperror("(strdup) Error duplicating %d-byte string.\n\"%s\"\nmalloc: ",
                 strlen(string) + 1, string);
        return(NULL);

    }

    strcpy(duplicate, string);

    return(duplicate);

}

