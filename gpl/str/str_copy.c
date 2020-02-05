
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

char  *str_copy (

#    if __STDC__
        const  char  *source,
        int  length,
        char  destination[],
        size_t  maxLength)
#    else
        source, length, destination, maxLength)

        char  *source ;
        int  length ;
        char  destination[] ;
        size_t  maxLength ;
#    endif

{
/*
 * Function: str_copy.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_copy() copies strings by length. The source string length
 *    specifies the length of the source string; the string need not be
 *    null-terminated. The destination string length specifies the size of
 *    the character array that is to receive the copied string; even if the
 *    source string must be truncated, the destination string is always
 *    null-terminated.
 *
 *    Invocation:
 *
 *        string = str_copy(source, length, destination, maxLength);
 *
 *    where
 *
 *        <source>            - I
 *            Is the string to be copied to the destination string.
 *
 *        <length>            - I
 *            Specifies the length of the source string.  If LENGTH is less
 *            than zero, str_copy() determines the length by scanning the
 *            string for a null terminator.
 *
 *        <destination>       - O
 *            Receives the copy of the source string.
 *
 *        <maxLength>         - I
 *            Specifies the maximum size of the destination string.
 *
 *        <string>            - O
 *            Returns a pointer to the copied string, i.e., the destination
 *            string. The copied string is always null-terminated, even if
 *            the source string has to be truncated.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (destination == NULL) {

        errno = EINVAL;
        vperror("(str_copy) Attempted copy to a NULL destination.\n");
        return(NULL);

    }

    if (source == NULL) return(strcpy(destination, ""));

    if (length < 0) length = strlen(source);

    length = (length < (int) maxLength) ? length : maxLength - 1;
    strncpy(destination, source, length);
    destination[length] = '\0';

    return(destination);

}

