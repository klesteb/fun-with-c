
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

int  str_match (

#    if __STDC__
        const  char  *target,
        const  char  *model)
#    else
        target, model)

        char  *target ;
        char  *model ;
#    endif

{
/*
 * Function: str_match.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_match() matches a possibly-abbreviated target string 
 *    against a model string.  For example, "C", "CO", "COO", and "COOK" 
 *    are partial matches of "COOK"; "COOKS" is NOT a partial match of 
 *    "COOK".
 *
 *    Invocation:
 *
 *        found = str_match(target, model);
 *
 *    where
 *
 *        <target>            - I
 *            Is the string to be checked for matching against the model 
 *            string. In the example above, "C", "CO", "COO", etc. are 
 *            target strings.
 *
 *        <model>             - I
 *            Is the model string against which the match is tested. In 
 *            the example above, "COOK" is the model string.
 *
 *        <found>             - O
 *            Returns true (a non-zero value) if the target string is a 
 *            partial or full match of the model string; false (zero) is 
 *            returned if the target string bears no relation to the model 
 *            string.
 *
 * Modification History
 *
 * Variables Used
 */

    int  length;

/*
 * Main part of function.
 */

    length = strlen(target);

    if (length > (int) strlen(model)) {

        return(0);                          /* Target string is too long. */

    } else if (strncmp(target, model, length)) {

        return(0);                          /* No match.                */

    } else return(1);                       /* Matched!                 */

}

