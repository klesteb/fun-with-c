
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       hash_priv.h                                                  */
/*                                                                           */
/* FACILITY:    Private variables and structures for the HASH package.       */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the HASH utilities.                    */
/*                                                                           */
/* ENVIRONMENT: VMS/AIX/Linux                                                */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      23-Apr-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef HASH_PRIV_H
#define HASH_PRIV_H 1

#include <errno.h>                  /* System error definitions.        */
#include <math.h>                   /* Math library definitions.        */
#include <stdio.h>                  /* Standard I/O definitions.        */
#include <stdlib.h>                 /* Standard C Library definitions.  */
#include "str_util.h"               /* String manipulation functions.   */
#include "vperror.h"                /* VPERROR() definitions.           */
#include "hash_util.h"              /* Hash table definitions.          */

/*----------------------------------------------------------------------*/
/* Hash Table Data Structures.                                          */
/*----------------------------------------------------------------------*/
									
typedef  struct  HashItem {
    char  *key;                     /* Item key.                        */
    const  void  *value;            /* Item value.                      */
    struct  HashItem  *next;        /* Pointer to next item in list.    */
}  HashItem ;

typedef  struct  _HashTable {
    int  debug;                     /* Debug switch (1/0 = yes/no).     */
    int  totalItems;                /* total number of items in table */
    int  maxChains;                 /* Maximum number of entries N in table. */
    int  numChains;                 /* Actual number of non-empty entries. */
    int  longestChain;              /* Records length of longest chain. */
    int *numItems;                  /* Array number of items in item chains. */
    HashItem  *chain[1];            /* Array of N pointers to item chains. */
}  _HashTable;

/*----------------------------------------------------------------------*/
/* Private Functions                                                    */
/*----------------------------------------------------------------------*/

int  hash_key (
#    if __STDC__
        const  char  *key,
        int  tableSize
#    endif
    ) ;

int  hash_prime (
#    if __STDC__
        int  number
#    endif
    ) ;

#endif

