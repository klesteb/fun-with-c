
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

int  hash_add (

#    if __STDC__
        HashTable  table,
        const  char  *key,
        const  void  *data)
#    else
        table, key, data)

        HashTable  table ;
        char  *key ;
        void  *data ;
#    endif

{
/*
 * Function: hash_add.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_add() adds a key-value pair to a hash table. If the 
 *    key is already present in the table, its old value is replaced by 
 *    the new value. The table must have already been created by 
 *    hash_create().
 *
 *    Invocation:
 *
 *        status = hash_add(table, key, data);
 *
 *    where
 *
 *        <table>             - I/O
 *            Is the hash table handle returned by hash_create().
 *
 *        <key>               - I
 *            Is the key for the item being entered in the table.
 *
 *        <data>              - I
 *            Is the data to be associated with the key.  This argument is a
 *            (VOID *) pointer; whatever data or pointer to data being passed
 *            in for this argument should be cast to (VOID *).
 *
 *        <status>            - O
 *            Returns the status of adding the key to the hash table, zero if
 *            no errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    HashItem  *item, *prev;
    int  comparison, index;

/*
 * Main part of function.
 */

    if (table == NULL) {

        errno = EINVAL;
        vperror("(hash_add) Hash table not created yet.\n");
        return(errno);

    }

    /* If the key is already in the hash table, then replace its data   */
    /* value.                                                           */

    index = hash_key(key, table->maxChains);
    comparison = -1;
    prev = (HashItem *)NULL;

    for (item = table->chain[index];  item != NULL;  item = item->next) {

        comparison = strcmp(item->key, key);
        if (comparison >= 0)  break;
        prev = item;

    }

    if (comparison == 0) {

        item->value = data;
        if (table->debug)  printf("(hashAdd) Replaced \"%s\":%p (%p) in table %p[%d].\n",
                                   key, data, item, table, index);
        return(0);

    }

    /* Add a brand new item to the hash table: allocate an ITEM node for*/
    /* the item, fill in the fields, and link the new node into the     */
    /* chain of items.                                                  */

    /* Allocate item node.                                              */

    if ((item = (HashItem *)malloc(sizeof(HashItem))) == NULL) {

        vperror("(hash_add) Error allocating item for \"%s\":%p.\nmalloc: ", key, data);
        return(errno);

    }

    /* Fill in the item node.                                           */

    if ((item->key = strdup(key)) == NULL) {

        vperror("(hash_add) Error duplicating key \"%s\".\nstrdup: ", key);
        free((char *)item);
        return(errno);

    }

    item->value = data;

    if (prev == NULL) {                     /* Link in at head of list. */

        item->next = table->chain[index];
        if (item->next == NULL)  table->numChains++;
        table->chain[index] = item;

    } else {                                /* Link in further down the list. */

        item->next = prev->next;
        prev->next = item;

    }

    if (table->debug)  printf("(hash_add) Added \"%s\":%p (%p) to table %p[%d].\n",
                               key, data, item, table, index);

    /* For statistical purposes, measure the length of the chain and,   */
    /* if necessary, update the LONGEST_CHAIN value for the hash table. */

    comparison = 0;

    for (item = table->chain[index];  item != NULL;  item = item->next) {

        comparison++;

    }

    if (table->longestChain < comparison) table->longestChain = comparison;

    return(0);

}

