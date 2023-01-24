
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

const char *hash_get (

#if _STDC_
    HashTable  table,
    int  index,
    void  **data)
#else
    table, index, data)

    HashTable table ;
    int  index ;
    void  **data ;
#endif

{    
/*
 * Function: hash_get.c
 * Version : 1.0
 * Created : 18-Oct-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function hash_get() returns the I-th key in a hash table and is 
 *    useful for iterating through the key/value mappings in the hash 
 *    table.  The ordering of keys is dependent on their location in 
 *    the hash table and this ordering should not be relied upon 
 *    by the application.
 * 
 *    Invocation:
 *
 *       key = hash_get(table, index, &data);
 *
 *    where
 * 
 *        <table>             - I
 *            is the hash table handle returned by hash_create().
 * 
 *       <index>              -  I
 *            is the index (0..N-1) of the desired key.
 * 
 *       <data>	              - O
 *            optionally returns the data associated with the key.  If this
 *            argument is NULL, the data value is not returned.  Otherwise,
 *            the value returned is a (VOID *) pointer; this pointer can be
 *            cast back to whatever data or pointer to data was stored by
 *            hash_add().
 * 
 *        <key>	               - O
 *            returns the indexed key in the table; NULL is returned if the
 *            index is out of bounds.
 * 
 * Modification History
 * 
 *     18-Feb-2020 K.Esteb
 *         Back ported this routine from a newer release.
 *
 * Variables Used
 */

    HashItem  *item ;
    int  i, totalItems ;

/*
 * Main part of function.
 */

    /* Locate the hash chain containing the indexed key. */

    totalItems = 0 ;
    for (i = 0 ;  i < table->maxChains ;  i++) {

        totalItems += table->numItems[i] ;
        if (totalItems > index)  break ;

    }

    if ((index < 0) || (i >= table->maxChains))
        return (NULL) ;				/* Out-of-bounds index. */

    /* Traverse the chain until the indexed key is reached. */

    item = table->chain[i] ;
    totalItems -= table->numItems[i] ;
    
    while (totalItems++ < index) {

        item = item->next ;

    }

    /* Return the key and data value to the caller. */

    if (data != NULL)  *data = (void *)item->value ;

    return (item->key) ;

}

