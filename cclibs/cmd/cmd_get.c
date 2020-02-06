
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2002 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "cmd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int cmd_get(

#    if __STDC__
    char **argument)
#    else
    argument)

    char **argument;
#    endif

{
/*
 * Function: cmd_get.c
 * Version : 1.0
 * Created : 07_May-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function cmd_get() will parse qualifiers and return any arguments from 
 *    the command tables.
 *
 *    Invocation:
 *
 *        index = cmd_get(argument);
 *
 *    where
 *
 *        <argument>          - O
 *            The argument is returned for a matching qualifier. This is a
 *            pointer to interal data. A local copy should be made as the 
 *            buffer is overwritten in subsequent calls to this function.
 *
 *        <index>             - O
 *            Returns the index (1..N) of the next qualifier; i.e., is this 
 *            qualifier the 1st, 2nd, or Nth qualifer in the set of qualifiers 
 *            specified in the command tables. For example, if "ab:{flag}c" 
 *            are the allowable qualifiers, then the appearance of "-c" line 
 *            will be returned by cmd_get() as an index of 4. CMD_END (zero) 
 *            is returned when the qualifier scan is complete; CMD_ERROR is 
 *            returned for invalid options and missing arguments.
 *
 * Modification History
 *
 * Variables Used
 */

    int index = CMD_ERROR;

/*
 * Main part of function.
 */

    index = opt_get(handle->scan, argument);

    return(index);

}

