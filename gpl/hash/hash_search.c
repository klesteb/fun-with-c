
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

int  hash_search (

#    if __STDC__
        HashTable  table,
        const  char  *key,
        void  **data)
#    else
        table, key, data)

        HashTable  table ;
        char  *key ;
        void  **data ;
#    endif

{
/*
 * Function: hash_search.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_search() looks up a key in a hash table and returns the
 *    data associated with that key. The hash table must be created using
 *    hash_create() and entries must be added to the table using hash_add().
 *
 *    Invocation:
 *
 *        found = hash_search(table, key, &data);
 *
 *    where
 *
 *        <table>             - I
 *            Is the hash table handle returned by hash_create().
 *
 *        <key>               - I
 *            Is the key for the item being searched in the table.
 *
 *        <data>              - O
 *            Returns the data associated with the key. The value returned is
 *            a (VOID *) pointer; this pointer can be cast back to whatever data
 *            or pointer to data was stored by hash_add().
 *
 *        <found>             - O
 *            Returns TRUE (non-zero) if the key was found in the hash table;
 *            FALSE (zero) is returned if the key was not found.
 *
 * Modification History
 *
 * Variables Used
 */

    HashItem  *item;
    int  comparison, index;

/*
 * Main part of function.
 */

    /* Lookup the item in the hash table.                               */

    index = hash_key(key, table->maxChains);
    comparison = -1;

    for (item = table->chain[index]; item != NULL; item = item->next) {

        comparison = strcmp(item->key, key);
        if (comparison >= 0)  break;

    }

    /* If found, return the item's data value to the calling routine.   */

    if (comparison == 0) {

        if (data != NULL)  *data = (void *) item->value;
        if (table->debug)  printf("(hash_search) \"%s\":%p found in table %p.\n", key, item->value, table);
        return(-1);

    } else {

        if (data != NULL)  *data = NULL;
        if (table->debug)  printf("(hash_search) Key \"%s\" not found in table %p.\n", key, table);
        return(0);

    }

}

