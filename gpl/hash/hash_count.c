
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

int  hash_count (

#if _STDC__
    HashTable  table)
#else
    table)

    HashTable  table;
#endif

{
/*
 * Function: hash_count.c
 * Version : 1.0
 * Created : 18-Oct-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function hash_count() returns the number of key/data pairs in a 
 *    hash table.
 * 
 *    Invocation:
 *
 *        count = hash_count(table);
 *
 *    where
 *
 *        <table>             - I
 *            Is the hash table handle returned by hashCreate().
 *
 *        <count>             - O
 *            Returns the number of key/data pairs in the hash table.
 *
 * Modification History
 * 
 *     18-Feb-2020 K.Esteb
 *         Back ported this routine from a newer release.
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    return ((table == NULL) ? 0 : table->totalItems) ;

}

