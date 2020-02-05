
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

int  hash_delete (

#    if __STDC__
        HashTable  table,
        const  char  *key)
#    else
        table, key)

        HashTable  table ;
        char  *key ;
#    endif

{
/*
 * Function: hash_delete.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_delete() deletes a key-data entry from a hash table. The
 *    table must have already been created by hash_create() and the key-data
 *    entry added to the table by hash_add().
 *
 *    Invocation:
 *
 *        status = hash_delete(table, key);
 *
 *    where
 *
 *        <table>             - I
 *            Is the hash table handle returned by hash_create().
 *
 *        <key>               - I
 *            Is the key for the item being deleted from the table.
 *
 *        <status>            - O
 *            Returns the status of deleting the key from the hash table, zero
 *            if no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    HashItem  *item, *prev;
    int  index;

/*
 * Main part of function.
 */

    if (table == NULL) {

        errno = EINVAL;
        vperror("(hash_delete) Hash table not created yet.\n");
        return(errno);

    }

    /* Locate the key's entry in the hash table.                        */

    index = hash_key(key, table->maxChains);
    prev = (HashItem *) NULL;

    for (item = table->chain[index]; item != NULL; item = item->next) {

        if (strcmp(item->key, key) == 0)  break;
        prev = item;

    }

    /* Unlink the entry from the hash table and free it.                */

    if (item == NULL) {

        if (table->debug)  printf("(hash_delete) Key \"%s\" not found in table %p.\n", key, table);
        return(-2);

    } else {

        if (prev == NULL) {

            table->chain[index] = item->next;

        } else prev->next = item->next;

    }

    if (table->debug)  printf("(hash_delete) Deleted \"%s\":%p from table %p.\n", item->key, item->value, table) ;

    free(item->key);                  /* Free item key.                 */
    free((char *)item);               /* Free the item.                 */

    return(0);

}

