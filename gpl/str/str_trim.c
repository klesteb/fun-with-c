
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

size_t  str_trim (

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
 * Function: str_trim.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_trim() trims trailing white space (blanks, tabs, and 
 *    new-line characters) from a string.  If the length argument is less 
 *    than zero, the string is assumed to be null-terminated; after trimming 
 *    trailing white space, the null terminator is relocated to the new end 
 *    of the string.  If the length argument is greater than or equal to 
 *    zero, the string does NOT need to be null-terminated; after trimming 
 *    trailing white space, the null terminator is NOT relocated.  In either 
 *    case, str_trim() returns the length of the new string.
 *
 *    Invocation:
 *
 *        trimmedLength = str_trim(string, length);
 *
 *    where
 *
 *        <string>           - I/O
 *            Is the string to be trimmed.  If the length argument is less
 *            than zero, STRING is assumed to be null-terminated and strTrim()
 *            will ***** relocate the null terminator *****.  If LENGTH is
 *            greater than or equal to zero, strTrim() will not relocate the
 *            null terminator; it simply computes the trimmed length.
 *
 *        <length>            - I
 *            Is the length, before trimming, of STRING.  If LENGTH is less
 *            than zero, STRING is assumed to be null-terminated.
 *
 *        <trimmedLength>     - O
 *            Returns the length of STRING with trailing blanks, tabs, and
 *            new-line characters removed.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *s;
    int  newLength;

/*
 * Main part of function.
 */

    newLength = (length < 0) ? strlen(string) : length;
    s = string + newLength;

    while ((s-- != string) &&
           ((*s == ' ') || (*s == '\t') || (*s == '\n')))
        newLength--;

    if (length < 0)  *++s = '\0';

    return(newLength);

}

