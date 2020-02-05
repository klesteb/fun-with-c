
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

char  *str_cat(

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
 * Function: str_cat.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_cat() catenates strings by length.
 *
 *    Invocation
 *
 *        string = str_cat(source, length, destination, maxLength);
 *
 *    where
 *
 *        <source>            - I
 *            Is the string to be added onto the end of the destination string.
 *
 *        <length>            - I
 *            Specifies the length of the source string.  If LENGTH is
 *            less than zero, strCat() determines the length by scanning
 *            the string for a null terminator.
 *
 *        <destination>       - I/O
 *            Is the string onto which the source string is appended.  The
 *            destination string must be null-terminated.
 *
 *        <maxLength>         - I
 *            Specifies the maximum size of the destination string that is
 *            to be extended by the source string.
 *
 *        <string>            - O
 *            Returns a pointer to the catenated string, i.e., the destination
 *            string.  The catenated string is always null-terminated, even if
 *            the source string has to be truncated (see str_copy()).
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    if (source == NULL)  return(destination);
    if (length < 0)  length = strlen(source);

    str_copy(source, length,
             &destination[strlen(destination)],
             maxLength - strlen(destination));

    return(destination);

}

