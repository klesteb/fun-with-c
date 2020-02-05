
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

int  hash_create (

#    if __STDC__
        int  maxEntries,
        int  debug,
        HashTable  *table)
#    else
        maxEntries, debug, table)

        int  maxEntries ;
        int  debug ;
        HashTable  *table ;
#    endif

{
/*
 * Function: hash_create.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_create() creates an empty hash table. hash_add() can 
 *    then be called to add entries to the table. The number of buckets 
 *    in the table is equal to the first prime number larger than the 
 *    expected maximum number of elements in the table.
 *
 *    Invocation:
 *
 *        status = hash_create(maxEntries, debug, &table);
 *
 *    where
 *
 *        <maxEntries>        - I
 *            Is the maximum number of entries expected in the table.
 *
 *        <debug>             - I
 *            Enables debug output (to STDOUT) on any HASH_UTIL calls
 *            for the new hash table.
 *
 *        <table>             - O
 *            Returns a handle for the new hash table.  This handle is
 *            used for accessing the table in subsequent HASH_UTIL calls.
 *
 *        <status>            - O
 *            Returns the status of creating the hash table, zero if no 
 *            errors occurred and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    int  i, prime, size;

/*
 * Main part of function.
 */

    /* Find the first prime number larger than the expected number of   */
    /* entries in the table.                                            */

    prime = (maxEntries % 2) ? maxEntries : maxEntries + 1;

    /* Check odd numbers only.                                          */

    for (;;) {

        if (hash_prime(prime))  break;
        prime += 2;

    }

    /* Create and initialize the hash table.                            */

    size = sizeof(_HashTable) + ((prime - 1) * sizeof (HashItem *));

    if ((*table = (void *)malloc(size)) == NULL) {

        vperror("(hash_create) Error allocating table of %d elements.\nmalloc: ", prime);
        return(errno);

    }

    (*table)->debug = debug;
    (*table)->maxChains = prime;
    (*table)->numChains = 0;           /* Number of non-empty chains.   */
    (*table)->longestChain = 0;        /* Length of longest chain.      */
 
   for (i = 0; i < prime; i++)
        (*table)->chain[i] = (HashItem *)NULL;

    if ((*table)->debug)  printf("(hash_create) Created hash table %p of %d elements.\n", *table, prime);

    return(0);

}

