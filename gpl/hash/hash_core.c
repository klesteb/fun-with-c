
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

int hash_util_debug;             /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/

/**/

int  hash_key (

#if __STDC__
    const  char  *key,
    int  tableSize)
#else
    key, tableSize)

    char  *key ;
    int  tableSize ;
#endif

{
/*
 * Function: hash_key.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_key() converts a character string key to an integer index
 *    into a hash table.  The conversion takes place in two steps: (i) "fold"
 *    the character string key into an integer, and (ii) divide that integer
 *    by the number of buckets in the table (a prime number computed by
 *    hash_create()).
 *
 *    The folding algorithm is best illustrated by an example.  Suppose you
 *    have a key "ABCDEFGHIJK" and integers are 4-byte entities on your
 *    computer.  The key could then folded by picking up 4-byte groups of
 *    characters from the key and adding them together:
 *
 *                               "ABCD"
 *                               "EFGH"
 *                             +  "IJK"
 *                               ------
 *                             = result
 *
 *    One way of doing this is to:
 *
 *        (1) pick up "A", then
 *        (2) shift "A" left 8 bits and add "B", then
 *        (3) shift "AB" left 8 bits and add "C", and then
 *        (4) shift "ABC" left 8 bits and add "D".
 *
 *    Pick up "EFGH" in the same way and add it to "ABCD"; then pick up "IJK"
 *    and add it on.
 *
 *    Invocation:
 *
 *        index = hash_key(key, tableSize);
 *
 *    where
 *
 *        <key>
 *            is a character string key.
 *
 *        <tableSize>
 *            is the size M of the table being hashed.
 *
 *        <index>
 *            returns the index, [0..M-1], in the table of where the key
 *            can be found.
 *
 * Modification History
 *
 * Variables Used
 */

    const  char  *s;
    unsigned  int  i, value, sum;

/*
 * Main part of function.
 */

    if (tableSize == 0)  return(0);     /* Empty table?                 */

    /* Fold the character string key into an integer number.            */

#define  BITS_TO_SHIFT  8

    s = key;
    for (sum = 0;  *s != '\0'; ) {

        for (i = value = 0;  (i < sizeof (int)) && (*s != '\0');  i++, s++)
            value = (value << BITS_TO_SHIFT) + *((unsigned char *) s);

        sum = sum + value;

    }

    return(sum % tableSize);       /* Return index [0..M-1] into table. */

}

/**/

int  hash_prime (

#    if __STDC__
        int  number)
#    else
        number)

        int  number ;
#    endif

{
/*
 * Function: hash_prime.c
 * Version : 1.0
 * Created : 23-Oct-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function hash_prime() determines if a number is a prime number.
 *
 *    Invocation:
 *
 *        isPrime = hash_prime(number);
 *
 *    where
 *
 *        <number>
 *            Is the number to be checked for "prime"-ness.
 *
 *        <isPrime>
 *            Return TRUE (non-zero) if NUMBER is prime and FALSE (zero) if
 *            NUMBER is not prime.
 *
 * Modification History
 *
 * Variables Used
 */

    int  divisor;

/*
 * Main part of function.
 */

    if (number < 0)  number = -number;
    if (number < 4)  return(1);            /* 0, 1, 2, and 3 are prime. */

    /* Check for possible divisors. The "divisor > dividend" test is    */
    /* similar to checking 2 .. sqrt(N) as possible divisors, but avoids*/
    /* the need for linking to the math library.                        */

    for (divisor = 2;;  divisor++) {

        /* Not prime - divisor found.                                   */

        if ((number % divisor) == 0) return(0);

        /* Prime - no divisors found.                                   */

        if (divisor > (number / divisor)) return(1);

    }

}

