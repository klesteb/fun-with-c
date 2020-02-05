
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

char  *str_destring (

#    if __STDC__
        char  *string,
        int  length,
        const  char  *quotes)
#    else
        string, length, quotes)

        char  *string ;
        int  length ;
        char  *quotes ;
#    endif

{
/*
 * Function: str_destring.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_destring() scans a string, replacing quote-delimited
 *    substrings by the text within the quotes.  For example, assuming
 *    that the allowed quote characters were single quotes, double quotes,
 *    and curly braces, the following conversions would be produced by
 *    str_destring():
 *
 *        ab             ==>    ab
 *        "ab cd"        ==>    ab cd
 *        ab"cd"         ==>    abcd
 *        "ab"'cd'       ==>    abcd
 *        "ab'cd"        ==>    ab'cd
 *        {ab"Hello!"cd} ==>    ab"Hello!"cd
 *
 *    Invocation:
 *
 *        result = str_destring(string, length, quotes);
 *
 *    where:
 *
 *        <string>            - I
 *            Is the string to be "destring"ed.
 *
 *        <length>            - I
 *            Is the length of the string to be "destring"ed.  If LENGTH
 *            is less than 0, the input string (STRING) is assumed to be
 *            null-terminated and the processing of the string will be done
 *            in place (i.e., the input string will be modified).  If LENGTH
 *            is greater than or equal to 0, then it specifies the number of
 *            characters of STRING that are to be processed.  In the latter
 *            case, str_destring() will dynamically allocate new storage to
 *            hold the processed string; the input string will not be touched.
 *
 *        <quotes>            - I
 *            Is a pointer to a character string that contains the allowable
 *            quote characters.  For example, single and double quotes (the UNIX
 *            shell quote characters) would be specified as "\"'".  If a left
 *            brace, bracket, or parenthesis is specified, str_destring() is
 *            smart enough to look for the corresponding right brace, bracket,
 *            or parenthesis.
 *
 *        <result>            - O
 *            Returns a pointer to the processed string.  If the LENGTH
 *            argument was less than zero, the "destring"ing was performed
 *            directly on the input string and RESULT simply returns the
 *            input STRING argument.  If the LENGTH argument was greater
 *            than or equal to zero, then the "destring"ing was performed
 *            on a copy of the input string and RESULT returns a pointer
 *            to this dynamically-allocated string.  In the latter case,
 *            the calling routine is responsible for FREE(3)ing the result
 *            string.  A static empty string ("") is returned in the event
 *            of an error.
 *
 * Modification History
 *
 * Variables Used
 */

    char *eos, rh_quote, *s;

/*
 * Main part of function.
 */

    if (string == NULL)  return("");
    if (quotes == NULL)  quotes = "";

    if (length >= 0) {                /* Make copy of input string.     */

        if ((s = strndup(string, length)) == NULL) {

            vperror("(str_destring) Error duplicating: \"%*s\"\nstrndup: ",
                    length, string);
            return(NULL);

        }

        string = s;

    }

    /* Scan the new argument and determine its length.                  */

    for (s = string;  *s != '\0';  s++) {

        /* Non-quote character?                                         */

        if (strchr(quotes, *s) == NULL) continue;

        switch (*s) {                   /* Determine right-hand quote.  */
            case '{':  
                rh_quote = '}';  
                break;

            case '[':  
                rh_quote = ']';  
                break;

            case '(':  
                rh_quote = ')';  
                break;

            default:
                rh_quote = *s;
                break ;

        }

        eos = strchr(s+1, rh_quote);    /* Locate right-hand quote.     */
        if (eos == NULL) {              /* Assume quote at null terminator. */

            eos = s + strlen(s);

        } else {

            /* Pull down one character.                                 */

            memmove(eos, eos+1, strlen(eos+1) + 1);

        }

        memmove(s, s+1, strlen(s+1) + 1); /* Pull down one character.   */
        s = eos - 2;                      /* 2 quotes gone!             */

    }

    /* Return the processed string to the calling routine.              */

    return(string);

}

