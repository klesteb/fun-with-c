
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

#include "get_priv.h"

/*----------------------------------------------------------------------*/

/**/

char  *get_word(

#    if __STDC__
        const  char  *string,
        const  char  *delimiters,
        int  *length)
#    else
        string, delimiters, length)

        char  *string ;
        char  *delimiters ;
        int  *length ;
#    endif

{
/*
 * Function: get_word.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Functon get_word() scans a string and returns the location and length 
 *    of the next word in the string. A "word" is text bounded by the 
 *    specified delimiters. The LENGTH value on input is added to the string 
 *    pointer to determine the start of the scan;  prior to the first call 
 *    to get_word() for a given string, set LENGTH equal to zero:
 *
 *        extern  char  *get_word();
 *        char  *delimiters = " \t," ;    -- Blanks and tabs.
 *        char  *word ;
 *        int  length ;
 *        ...
 *        length = 0 ;
 *        word = string_to_scan ;
 *        while (strlen (word) > 0) {
 *            word = get_word(word, delimiters, &length);
 *            ... Process STRING(word:word+length-1) ...
 *        }
 *
 *    NOTE that when the string has been completely scanned, get_word() will
 *    continually return a pointer to the null terminator in the string and
 *    a length of zero.
 *
 *    Invocation:
 *
 *        word = get_word(string, delimiters, &length);
 *
 *    where
 *
 *        <string>
 *            is a pointer to the character string to be scanned.  The
 *            value of LENGTH on input is added to this pointer to determine
 *            the first character to be scanned.  NOTE that the pointer
 *            passed in for STRING should initially point to the full string
 *            being scanned and should be updated on each call to get_word()
 *            (see the example above).
 *
 *        <delimiters>
 *            is a pointer to a character string that contains the characters
 *            that are possible delimiters. For example, if commas and blanks
 *            are your delimiters, pass in " ," as the delimiter string. The
 *            delimiter string can be changed between calls to get_word().
 *
 *        <length>
 *            is the address of a LENGTH variable that is, on input, the
 *            length of the last word scanned. Initially zero, LENGTH is
 *            added to STRING to determine the start of the scan. On output,
 *            LENGTH returns the length of the new "word".
 *
 *        <word>
 *            is a pointer to the new "word". Since WORD points to the
 *            location of the "word" in the full string buffer, the "word"
 *            is probably not null-terminated; the Standard C Library
 *            functions strncmp() and strncpy() can be used to compare and
 *            copy the "word".  WORD should be passed in as STRING on the
 *            next call to get_word() (see the example).
 *
 * Modification History
 *
 * Variables Used
 */

    char *s;

/*
 * Main part of function.
 */

    s = (char *)string + *length;
    s = s + strspn(s, delimiters);
    *length = strcspn(s, delimiters);

    return(s);

}

