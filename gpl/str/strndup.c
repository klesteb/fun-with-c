
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

char  *strndup(

#    if __STDC__
        const  char  *string,
        size_t  length)
#    else
        string, length)

        char  *string ;
        size_t  length ;
#    endif

{
/*
 * Function: strndup.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function strndup() duplicates a string of a specified length. strndup()
 *    is the "n" counterpart of strdup(), as in strncmp(3) and strcmp(3).
 *
 *    Invocation:
 *
 *        duplicate = strndup(string, length);
 *
 *    where:
 *
 *        <string>            - I
 *            Is the string to be duplicated.
 *
 *        <length>            - I
 *            Is the number of characters to be duplicated.
 *
 *        <duplicate>         - O
 *            Returns a MALLOC(3)ed copy of the input string; the duplicate
 *            is null terminated.  The caller is responsible for FREE(3)ing
 *            the duplicate string.  NULL is returned in the event of an error.
 *
 * Modification History
 *
 * Variables Used
 */

    char *duplicate;

/*
 * Main part of function.
 */

    if ((duplicate = malloc(length + 1)) == NULL) {

        vperror("(strndup) Error duplicating %d-byte string.\n\"%*s\"\nmalloc: ",
                length, length, string);
        return(NULL);

    }

    if (string == NULL) {

        *duplicate = '\0';

    } else {

        strncpy(duplicate, string, length);
        duplicate[length] = '\0';

    }

    return(duplicate);

}

