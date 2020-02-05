
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

#include "crlf_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *nl2crlf(

#    if __STDC__
        char  *string,
        int  length,
        int  maxLength)
#    else
        string, length, maxLength)

        char  *string ;
        int  length ;
        int  maxLength ;
#    endif

{    /* Local variables. */
/*
 * Function: nl2crlf.c
 * Version : 1.0
 * Created : 07-Jul-1996
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function nl2crlf() scans a string, replacing each newline ("\n")
 *    character with a carriage return/line feed sequence.
 *
 *    Invocation
 *
 *        string = nl2crlf(string, length, maxLength);
 *
 *    where
 *
 *        <string>            - I/O
 *            is the string to be converted.  The conversion is done in
 *            place.  STRING should be sized to allow the insertion of
 *            "nlCount(string)" extra characters.
 *
 *        <length>            - I
 *            specifies the length of the string.  If LENGTH is less than
 *            zero, nl2crlf() determines the length by scanning the string
 *            for a null terminator.
 *
 *        <maxLength>         - I
 *            is the maximum length to which the string can grow.
 *
 *        <string>            - O
 *            returns a pointer to the converted string; i.e., STRING.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    char  *s;

/*
 * Main part of function.
 */

    if (string == NULL)  return(NULL);
    if (length < 0)  length = strlen(string);
    string[length] = '\0';
    if (length == 0)  return(string);

    /* Replace each newline character by a carriage return/line feed    */
    /* sequence.                                                        */

    for (s = string;  *s != '\0';  s++, length--) {

        if ((s - string + 1) >= (maxLength - 1))  break;

        if (*s == '\n') {

            memmove(s+1, s, length+1);  
            *s++ = '\r';

        }

    }

    return(string);

}

