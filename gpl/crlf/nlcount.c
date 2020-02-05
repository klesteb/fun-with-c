
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

int  nlcount (

#    if __STDC__
        const  char  *string,
        int  length)
#    else
        string, length)

        char  *string ;
        int  length ;
#    endif

{
/*
 * Function: nlcount.c
 * Version : 1.0
 * Created : 07-Jul-1996
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function nlCount() counts the number of newline ("\n") characters
 *    in a string.
 *
 *    Invocation
 *
 *        numNL = nlcount(string, length);
 *
 *    where
 *
 *        <string>    - I
 *            is the string to be examined.
 *
 *        <length>    - I
 *            specifies the length of the string.  If LENGTH is less than
 *            zero, nlCount() determines the length by scanning the string
 *            for a null terminator.
 *
 *        <numNL>        - O
 *            returns the number of newline ("\n") characters found in the
 *            string.
 *
 * Modification History
 *
 *    10-May-2002 K.Esteb
 *        Reformated the file.
 *
 * Variables Used
 */

    int  numNL;

/*
 * Main part of function.
 */

    if (string == NULL)  return(0);
    if (length < 0)  length = INT_MAX;

    for (numNL = 0;  (length-- > 0) && (*string != '\0');  string++)
        if (*string == '\n')  numNL++;

    return(numNL);

}

