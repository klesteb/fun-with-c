
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

int  hash_statistics (

#    if __STDC__
        FILE  *outfile,
        HashTable  table)
#    else
        outfile, table)

        FILE  *outfile ;
        HashTable  table ;
#    endif

{
/*
 * Function: hash_statistics.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_statistics() outputs various statistical measurements for
 *    a hash table.
 *
 *    Invocation:
 *
 *        status = hash_statistics(outfile, table);
 *
 *    where
 *
 *        <outfile>           - I
 *            Is the UNIX file descriptor (FILE *) for the output file.
 *
 *        <table>             - I
 *            Is the hash table handle returned by hashCreate().
 *
 *        <status>            - O
 *            Returns zero.
 *
 * Modification History
 *
 * Variables Used
 */

    int  count, *histogram, i, longestChain, maxChains, numChains;
    long  sum, sumOfSquares;
    HashItem  *item;

/*
 * Main part of function.
 */

    if (table == NULL) {

        fprintf(outfile, "-- Null Hash Table --\n");
        return(0);

    }

    maxChains = table->maxChains;
    numChains = table->numChains;
    longestChain = table->longestChain;

    fprintf(outfile, "There are %d empty buckets, %d non-empty buckets,\nand %d items in the longest chain.\n\n",
             maxChains - numChains, numChains, longestChain);

    if ((histogram = (int *)malloc((longestChain + 1) * sizeof(int))) == NULL) {

        vperror("(hash_statistics) Error allocating memory for histogram.\nmalloc: ");
        return(errno);

    }

    for (count = 0; count <= longestChain; count++)
        histogram[count] = 0;

    for (i = 0; i < maxChains; i++) {

        item = table->chain[i];

        for (count = 0; item != NULL; count++)
            item = item->next;

        histogram[count]++;

    }

    for (count = 1, sum = sumOfSquares = 0;
         count <= longestChain; count++) {

        fprintf(outfile, "Buckets of length %d: %d\n", count, histogram[count]);
        sum = sum + histogram[count] * count;
        sumOfSquares = sumOfSquares + histogram[count] * count * count;

    }

    fprintf(outfile, "\nMean bucket length = %G\n",
             (double) sum / (double) numChains);

    fprintf(outfile, "\nStandard deviation = %G\n",
             sqrt((double) ((numChains * sumOfSquares) - (sum * sum)) /
                  (double) (numChains * (numChains - 1))));

    free((char *)histogram);

    return(0);

}

