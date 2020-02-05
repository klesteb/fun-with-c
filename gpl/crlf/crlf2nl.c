
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

char  *crlf2nl(

#    if __STDC__
        char  *string,
        int  length,
        char  *lastChar)
#    else
        string, length, lastChar)

        char  *string ;
        int  length ;
        char  *lastChar ;
#    endif

{
/*
 * Function: crlf2nl.c
 * Version : 1.0
 * Created : 07-Jul-1996
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function crlf2nl() scans a string, converting each occurrence of a
 *    carriage return/line feed sequence to a newline ("\n") character.
 *    Take note of the following subtleties:
 *
 *        (1) A carriage return that is NOT followed by a line feed is
 *            left in the string as-is.
 *
 *        (2) A lone carriage return at the very end of the string is
 *            stripped from the string.  If you are scanning buffers
 *            with successive calls to crlf2nl(), the carriage return
 *            will be restored in the next call to crlf2nl() (via the
 *            LASTCHAR argument) and handled normally.  Of course, a
 *            carriage return at the very end of the very last buffer
 *            (i.e., you won't be calling crlf2nl() again) must be
 *            restored manually by you.
 *
 *
 *    Invocation
 *
 *        string = crlf2nl(string, length, &lastChar);
 *
 *    where
 *
 *        <string>            - I/O
 *            is the string to be converted.  The conversion is done in
 *            place.  If you may have CR/LF sequences spanning buffers,
 *            STRING should be sized to allow the insertion of one extra
 *            character.
 *
 *        <length>            - I
 *            specifies the length of the string.  If LENGTH is less than
 *            zero, crlf2nl() determines the length by scanning the string
 *            for a null terminator.
 *
 *        <lastChar>          - I/O
 *            was/is the last character examined.  On input, this argument
 *            is the last character examined in the previous string that
 *            was converted.  On output, this argument returns the last
 *            character examined in STRING.  This argument is provided for
 *            cases in which a carriage return/line feed sequence spans a
 *            buffer boundary.  If this is not a concern for you, this
 *            argument can be NULL.
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

    char  prevChar, *s ;

/*
 * Main part of function.
 */

    if (string == NULL)  return(NULL);
    if (length < 0)  length = strlen(string);
    string[length] = '\0';
    if (length == 0)  return(string);

    /* Save the last character in the string for the next call to       */
    /* crlf2nl(). If the last character is a carriage return, strip it  */
    /* from the string; it will be restored if necessary on the next    */
    /* call to crlf2nl().                                               */

    prevChar = (lastChar != NULL) ? *lastChar : '\0';
    if (lastChar != NULL)  *lastChar = string[length-1];
    if (string[length-1] == '\r')  string[--length] = '\0';

    /* If the last character in the previous string was a carriage      */
    /* return (always stripped by crlf2nl()), then insert it at the     */
    /* beginning of this string.                                        */

    if ((prevChar == '\r') && (string[0] != '\n')) {

        memmove(string+1, string, ++length);
        string[0] = '\r';

    }

    /* It's a beautiful spring day outside and I'm stuck in here putzing*/
    /* around with carriage returns and line feeds.                     */

    for (s = string;  *s != '\0';  s++, length--) {

        if ((s[0] == '\r') && (s[1] == '\n'))  memmove(s, s+1, length--);

    }

    return(string);

}

