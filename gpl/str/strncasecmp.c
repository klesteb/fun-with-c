
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

int  strncasecmp(

#    if __STDC__
        const  char  *thisString,
        const  char  *thatString,
        size_t  length)
#    else
        thisString, thatString, length)

        char  *thisString ;
        char  *thatString ;
        size_t  length ;
#    endif

{
/*
 * Function: strncasecmp.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function strncasecmp() performs a length-limited comparison of two 
 *    strings, ignoring the case of the individual characters.  
 *    strncasecmp() is supported by many C libraries, but it is not part 
 *    of the ANSI C library.
 *
 *    Invocation:
 *
 *        comparison = strncasecmp(thisString, thatString, length);
 *
 *    where:
 *
 *        <thisString>    - I
 *        <thatString>    - I
 *            are the null- or length-terminated strings being compared.
 *
 *        <length>    - I
 *            is the number of characters in each string to examine.
 *
 *        <comparison>    - O
 *            returns one of three possible values:
 *
 *                < 0 if THISSTRING is lexically less than THATSTRING,
 *                = 0 if the two strings are equal except for case, or
 *                > 0 if THISSTRING is lexically greater than THATSTRING.
 *
 *            The less-than and greater-than relations are also case-insensitive.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *that, *this;

/*
 * Main part of function.
 */

    /* Check for NULL strings.                                          */

    if ((thisString == NULL) && (thatString != NULL)) {

        return(-1);

    } else if ((thisString == NULL) && (thatString == NULL)) {

        return(0);

    } else if ((thisString != NULL) && (thatString == NULL)) {

        return(1);

    }

    /* Compare the two strings, character by character.                 */

    this = (char *)thisString;
    that = (char *)thatString;

    while ((*this != '\0') && (*that != '\0') && (length > 0)) {

        if (toupper(*this) < toupper(*that)) {

            return(-1);

        } else if (toupper(*this) > toupper(*that)) {

            return(1);

        }

        this++;  
        that++;  
        length--;

    }

    /* The strings are identical (excepting case) for the first LENGTH  */
    /* characters or as far as the shorter string goes.                 */

    if (length <= 0) {

        return(0);         /* First LENGTH characters are equal.        */

    } else if (*this != '\0') {

        return(-1);         /* THIS < THAT.                             */

    } else if (*that != '\0') {

        return(1);          /* THAT < THIS.                             */

    } else return(0);       /* strlen() < LENGTH, but strings are equal.*/

}

