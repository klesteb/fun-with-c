
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

char  *str_tolower (

#    if __STDC__
        char  *string,
        int  length)
#    else
        string, length)

        char  *string ;
        int  length ;
#    endif

{
/*
 * Function: str_tolower.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_tolower() converts the characters in a string to lower
 *    case.  If the length argument is zero, the string is assumed to be
 *    null-terminated; otherwise, only LENGTH characters are converted.
 *
 *    Invocation:
 *
 *        result = str_tolower(string, length);
 *
 *    where
 *
 *        <string>            - I/O
 *            points to the string to be converted; the conversion is
 *            done in-place.
 *
 *        <length>            - I
 *            is the number of characters to be converted.  If LENGTH is less
 *            than zero, the entire string is converted up to the null terminator.
 *
 *        <result>            - O
 *            returns a pointer to the converted string; i.e., STRING.
 *
 * Modification History
 *
 * Variables Used
 */

    char *s;

/*
 * Main part of function.
 */

    if (length < 0)  length = strlen(string);

    for (s = string;  length-- > 0;  s++) {

        if (isupper(*s))  *s = tolower(*s);

    }

    return(string);

}

