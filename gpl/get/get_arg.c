
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

char  *get_arg(

#    if __STDC__
        const  char  *arg,
        int  *length)
#    else
        arg, length)

        char  *arg ;
        int  *length ;
#    endif

{    /* Local variables. */
/*
 * Function: get_arg.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function get_arg() scans a string and returns the location and length 
 *    of the next argument in the string. An "argument" is text bounded by 
 *	the white space (blanks or tabs) or commas. get_arg() uses the input 
 *	value of the LENGTH argument to advance past the previous argument; on 
 *	the first call to get_arg() for a given string, set LENGTH equal to -1:
 *
 *        extern  char  *get_arg () ;
 *        char  *arg ;
 *        int  length ;
 *        ...
 *        length = -1 ;
 *        arg = string_to_scan ;
 *        while ((arg = get_arg(arg, &length)) != NULL) {
 *            ... Process ARG[0:length-1] ...
 *        }
 *
 *    get_arg() detects empty arguments (consecutive commas, possibly with
 *    intervening white space) and returns a length of zero for the empty
 *    argument; a comma at the end of the line indicates a final empty
 *    argument.
 *
 *    NOTE that when the string has been completely scanned, get_arg() will
 *    continually return a NULL pointer.
 *
 *    Invocation:
 *
 *        argument = get_arg(string, &length);
 *
 *    where
 *
 *        <string>
 *            is a pointer to the character string to be scanned.  On the
 *            first call to get_arg() for a given string, STRING should point
 *            to the beginning of the string and LENGTH should equal -1.  On
 *            subsequent calls to get_arg() for the same string, STRING should
 *            be updated to point to the last argument processed (see the
 *            example above).
 *
 *        <length>
 *            is the address of a LENGTH variable that is, on input, the
 *            length of the last argument scanned. Set LENGTH to -1 on the
 *            first call to get_arg() for a given string; get_arg() will 
 *            then update LENGTH on subsequent calls. On returning from 
 *            get_arg(), LENGTH returns the length of the new argument; 
 *            zero is returned for empty arguments (indicated by consecutive 
 *            commas).
 *
 *        <argument>
 *            returns a pointer to the new argument.  Since ARGUMENT points
 *            to the location of the argument in the full string buffer, the
 *            argument is probably not null-terminated; the Standard C Library
 *            functions strncmp() and strncpy() can be used to compare and 
 *            copy the argument. ARGUMENT should be passed in as STRING on 
 *            the next call to get_arg() (see the example).
 *
 * Modification History
 *
 * Variables Used
 */

    int  comma = 0;

/*
 * Main part of function.
 */

    if (arg == NULL)  return(NULL);

    /* Advance past the previous argument.                              */

    if (*length >= 0) {

        arg = arg + *length;
        arg = arg + strspn(arg, " \t");    /* Skip white space that trails previous argument. */

        if (*arg == ',') {            /* Skip comma delimiter from previous argument. */

            arg++;
            comma = 1;

        }

    }

    /* Locate the next argument. A comma at the end of the string       */
    /* indicates a final empty argument.                                */

    arg = arg + strspn(arg, " \t");    /* Skip leading white space from next argument. */
    *length = strcspn(arg, " \t,");    /* Locate the next argument delimiter. */
    *length = str_trim((char *)arg,    /* Trim trailing white space. */
                       *length);

    if ((*arg == '\0') && !comma) {

        return(NULL);

    } else return((char *)arg);

}

