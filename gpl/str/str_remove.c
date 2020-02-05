
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

size_t  str_remove(

#    if __STDC__
        size_t  numToRemove,
        size_t  offset,
        char  *string)
#    else
        numToRemove, offset, string)

        size_t  numToRemove ;
        size_t  offset ;
        char  *string ;
#    endif

{
/*
 * Function: str_remove.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_remove() removes N characters of text from any position in
 *    a string.
 *
 *    Invocation:
 *
 *        length = str_remove(numToRemove, offset, string);
 *
 *    where
 *
 *        <numToRemove>       - I
 *            Is the number of characters to delete from the string.
 *
 *        <offset>            - I
 *            Is the character offset (0..N-1) in STRING at which the
 *            deletion will take place.
 *
 *        <string>            - I/O
 *            Points to the string from which text will be deleted.
 *
 *        <length>            - O
 *            Returns the length of the string after the deletion.
 *
 * Modification History
 *
 * Variables Used
 */

    int length;

/*
 * Main part of function.
 */

    /* Validate the arguments.                                          */

    if (string == NULL)  return(0);
    length = strlen(string);
    if ((int) offset >= length) return(0);
    if (numToRemove > (length - offset))  numToRemove = length - offset;

    /* Remove the substring.                                            */

    memmove(&string[offset], &string[offset+numToRemove],
            length - (offset + numToRemove) + 1);

    return(strlen(string));

}

