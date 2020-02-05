
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

#include "hash_priv.h"

/*----------------------------------------------------------------------*/

/**/

int  hash_destroy (

#    if __STDC__
        HashTable  table)
#    else
        table)

        HashTable  table ;
#    endif

{
/*
 * Function: hash_destroy.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_destroy() deletes a hash table.
 *
 *    Invocation:
 *
 *        status = hash_destroy(table);
 *
 *    where
 *
 *        <table>            - I
 *            Is the hash table handle returned by hashCreate().
 *
 *        <status>            - O
 *            Returns the status of deleting the hash table, zero if no errors
 *            occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int  i;
    HashItem  *item, *next;

/*
 * Main part of function.
 */

    if (table->debug)  printf("(hash_destroy) Deleting hash table %p.\n", table);

    if (table == NULL)  return(0);

    for (i = 0; i < table->maxChains; i++) {

        for (item = table->chain[i]; item != NULL; item = next) {

            next = item->next;
            free(item->key);                  /* Free item key.         */
            free((char *)item);               /* Free the item.         */

        }

    }

    free((char *)table);                      /* Free the hash table.   */

    return(0);

}

