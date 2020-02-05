
/*******************************************************************************

    Program to test the HASH_UTIL routines.  Compile as follows:

        % cc -g -DTEST hash_util.c -I<... includes ...>

    Invocation:

        % a.out [ <num_entries> ]

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "vperror.h"
#include "hash_util.h"

extern  int  vperror_print ;            /* 0 = no print, !0 = print */


main (argc, argv)

    int  argc ;
    char  *argv[] ;

{    /* Local variables. */
    char  *text[16] ;
    int  i, maxNumEntries ;
    void  *data, *table ;




    vperror_print = 1 ;

    maxNumEntries = (argc > 1) ? atoi (argv[1]) : 100 ;

/* Create an empty hash table. */

    if (hash_create(maxNumEntries, 1, &table)) {
        vperror("Error creating table.\nhash_create: ") ;
        exit(errno) ;
    }

/* Add "SYM_n" symbols to the table. */

    for (i = 0 ;  i < maxNumEntries ;  i++) {
        sprintf (text, "SYM_%d", i) ;
        if (hash_add(table, text, (void *) i)) {
            vperror("Error adding entry %d to the table.\nhash_add: ", i) ;
            exit(errno) ;
        }
    }

/* Verify that the symbols were entered correctly and with the correct value. */

    for (i = 0 ;  i < maxNumEntries ;  i++) {
        sprintf(text, "SYM_%d", i) ;
        if (!hash_search(table, text, &data) || ((int) data != i)) {
            vperror("Error looking up entry %d in the table.\nhash_search: ", i) ;
            exit(errno) ;
        }
    }

	/* Dump the hash table. */

    hash_dump(stdout, "\n", table) ;

	/* Delete the hash table. */

    if (hash_destroy(table)) {
        vperror("Error deleting the hash table.\nhash_destroy: ") ;
        exit(errno) ;
    }

}

