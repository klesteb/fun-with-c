
/*---------------------------------------------------------------------------*/
/*             Copyright (c) 2020 by Kevin L. Esteb                          */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "hash_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  hash_set_debug (

#if _STDC__
    HashTable  table, int debug)
#else
    table, debug)

    HashTable  table;
    int debug;
#endif

{
/*
 * Function: hash_set_debug.c
 * Version : 1.0
 * Created : 18-Oct-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function hash_set_debug() toggles the debug flag. 
 * 
 *    Invocation:
 *
 *        status = hash_set_debug(table, debug);
 *
 *    where
 *
 *        <table>             - I
 *            Is the hash table handle returned by hash_create().
 * 
 *        <debug>             - I
 *            The debug toggle, it should be 0 or 1.
 *
 *        <status>             - O
 *            Will return 0 for success and -1 for failure.
 *
 * Modification History
 * 
 *
 * Variables Used
 */

    int stat = ERR;

/*
 * Main part of function.
 */

    if ((table != NULL) && ((debug == 0) || (debug == 1))) {

        stat = OK;
        table->debug = debug;

    }

    return stat;

}

