
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

int  hash_dump (

#if __STDC__
    FILE  *outfile,
    const  char  *header,
    HashTable  table)
#else
    outfile, header, table)

    FILE  *outfile ;
    char  *header ;
    HashTable  table ;
#endif

{
/*
 * Function: hash_dump.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_dump() dumps a hash table to the specified output file.
 *
 *    Invocation:
 *
 *        status = hash_dump(outfile, header, table);
 *
 *    where
 *
 *        <outfile>           - I
 *            is the UNIX file descriptor (FILE *) for the output file.
 *
 *        <header>            - I
 *            is a text string to be output as a header.  The string is
 *            actually used as the format string in an FPRINTF statement,
 *            so you need to include any newlines ("\n"), etc. that you
 *            need.  This argument can be NULL.
 *
 *        <table>             - I
 *            is the hash table handle returned by hash_create().
 *
 *        <status>            - O
 *            returns zero.
 *
 * Modification History
 *
 * Variables Used
 */

    int  i;
    HashItem  *item;

/*
 * Main part of function.
 */

    if (header != NULL)  fprintf(outfile, header);

    if (table == NULL) {

        fprintf(outfile, "-- Null Hash Table --\n");
        return(0);

    }

    hash_statistics(outfile, table);
    fprintf(outfile, "\n");

    for (i = 0; i < table->maxChains; i++) {

        item = table->chain[i];

        if (item != NULL) {

            fprintf(outfile, "Bucket %d:\n", i);

            while (item != NULL) {

                fprintf(outfile, "    Value: %p    Key: \"%s\"\n",
                        item->value, item->key);
                item = item->next;

            }

        }

    }

    return(0);

}

