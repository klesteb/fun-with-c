
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                                                           */
/* TITLE:       hash_util.h                                                  */
/*                                                                           */
/* FACILITY:    Global variables and structures for the HASH package.        */
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

#ifndef HASH_UTIL_H
#define HASH_UTIL_H 1

#ifdef __cplusplus
    extern  "C" {
#endif

#include  <stdio.h>                     /* Standard I/O definitions.    */

#include "gpl/ansi_setup.h"             /* ANSI or non-ANSI C?          */

/*----------------------------------------------------------------------*/
/*    Structures and Definitions.                                       */
/*----------------------------------------------------------------------*/

typedef struct _HashTable *HashTable;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

extern int hash_util_debug;      /* Global debug switch (1/0 = yes/no). */

/*----------------------------------------------------------------------*/
/*    Public Functions                                                  */
/*----------------------------------------------------------------------*/

extern  int  hash_add P_((HashTable table,
			 char *key,
			 void *data));

extern  int  hash_create P_((int maxEntries,
			    int debug,
                HashTable *table));

extern  int  hash_delete P_((HashTable table,
			    const char *key,
                void (*callback)(void *value)));

extern  int  hash_destroy P_((HashTable table,
                void (*callback)(void *value)));

extern  int  hash_dump P_((FILE *outfile,
			  const char *header,
			  HashTable table));

extern  int  hash_search P_((HashTable table,
			    const char *key,
			    void **data));

extern  int  hash_statistics P_((FILE *outfile,
				HashTable table));

extern  int hash_count P_((HashTable table));

extern const char *hash_get P_((HashTable table, int index, void **data)); 

extern int hash_set_debug P_((HashTable table, int debug)); 

#ifdef __cplusplus
    }
#endif

#endif

