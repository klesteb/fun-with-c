
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

char  *get_string (

#    if __STDC__
        const  char  *last_argument,
        const  char  *quotes,
        int  *length)
#    else
        last_argument, quotes, length)

        char  *last_argument ;
        char  *quotes ;
        int  *length ;
#    endif

{
/*
 * Function: get_string.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function get_string() scans the next argument in a string and returns 
 *    the length of the raw argument. get_string() is intended for stepping 
 *    through the arguments in a command line string, taking into account 
 *    the quoting conventions of the command line. For example, get_string() 
 *    will treat the following as single arguments:
 *
 *                    ab
 *                    "ab cd"
 *                    'ab cd'
 *                    ab"cd"
 *                    "ab"'cd'
 *
 *    The LENGTH value on input is added to the LAST_ARGUMENT pointer to
 *    determine the start of the scan. Prior to the first call to get_string()
 *    for a given command line string, LENGTH must be set to zero, as shown in
 *    the following example:
 *
 *        char  *quotes = "\"'";    -- Single and double quotes.
 *        char  *arg, *command_line;
 *        int  length;
 *        ...
 *        length = 0;
 *        arg = get_string(command_line, quotes, &length);
 *        while (length > 0) {
 *            ... Process COMMAND_LINE(arg:arg+length-1) ...
 *            arg = get_string(command_line, quotes, &length);
 *        }
 *
 *    When the command line string has been completely scanned, get_string()
 *    will continually return a pointer to the null terminator at the end of
 *    the string and a length of zero.
 *
 *    Invocation:
 *
 *        argument = get_string(last_argument, quotes, &length);
 *
 *    where:
 *
 *        <last_argument>    - I
 *            is a pointer to the last argument scanned. Initially, this 
 *            should point to the beginning of the command line string 
 *            and the LENGTH argument (see below) should be zero.
 *
 *        <quotes>    - I
 *            is a pointer to a character string that contains the allowable
 *            quote characters.  For example, single and double quotes (the 
 *            UNIX shell quote characters) would be specified as "\"'". If 
 *            a left brace, bracket, or parenthesis is specified, get_string() 
 *            is smart enough to look for the corresponding right brace, 
 *            bracket, or parenthesis. The quote string can be changed in 
 *            between calls to get_string().
 *
 *        <length>    - I/O
 *            is the address of a LENGTH variable that is, on input, the
 *            length of the last argument scanned. Initially zero, LENGTH 
 *            is added to LAST_ARGUMENT to determine the start of the scan. 
 *            On output, LENGTH returns the length of the next argument.
 *
 *        <argument>    - O
 *            returns a pointer to the next argument.  Since ARGUMENT points
 *            to the location of the argument in the full string buffer, 
 *            the argument is probably not null-terminated; the Standard C 
 *            Library functions strncmp() and strncpy() can be used to 
 *            compare and copy the argument. ARGUMENT should be passed in 
 *            as LAST_ARGUMENT on the next call to get_string().
 *
 * Modification History
 *
 * Variables Used
 */

    char *arg, *s;
    char rh_quote;

/*
 * Main part of function.
 */

    if (last_argument == NULL) {

        *length = 0;
        return(NULL);

    }

    if (quotes == NULL)  quotes = "";

    /* Advance past the previous argument to the start of the next      */
    /* argument.                                                        */

    arg = (char *)last_argument;
    if (*length > 0)  arg = arg + *length;
    arg = arg + strspn(arg, " \t");           /* Skip white space that
                                              trails previous argument. */

    /* Scan the new argument and determine its length.                  */

    for (s = arg;  *s != '\0';  s++) {

        if ((*s == ' ') || (*s == '\t'))    /* Argument-terminating white space? */
            break;

        if (strchr(quotes, *s) == NULL)     /* Non-quote character? */
            continue ;

        switch (*s) {                /* Determine right-hand quote. */
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

        s = strchr(++s, rh_quote);        /* Locate right-hand quote.     */
        if (s == NULL)  s = arg + strlen(arg) - 1;

    }


    /* Return the new argument and its length to the calling routine.   */

    *length = s - arg;

    return(arg);

}

