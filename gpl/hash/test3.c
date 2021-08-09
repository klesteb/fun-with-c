
/*******************************************************************************

    Program to test the HASH_UTIL routines.  Compile as follows:

        % cc -g -DTEST hash_util.c -I<... includes ...>

    Invocation:

        % a.out [ <num_entries> ]

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "gpl/vperror.h"
#include "gpl/hash_util.h"

extern  int  vperror_print ;            /* 0 = no print, !0 = print */


void freeit(void *data) {

    printf("freeit\n");
    
}

int main (argc, argv)

    int  argc ;
    char  *argv[] ;

{    /* Local variables. */

    char text[16];
    void *data = NULL;
    char *key = NULL;
    int i, count, maxNumEntries = 0;
    HashTable table;


    vperror_print = 0;;

    maxNumEntries = (argc > 1) ? atoi (argv[1]) : 100 ;

    /* Create an empty hash table. */

    if (hash_create(maxNumEntries, 0, &table)) {
        vperror("Error creating table.\nhash_create: ") ;
        exit(errno) ;
    }

    /* Add "SYM_n" symbols to the table. */

    for (i = 0 ;  i < maxNumEntries ;  i++) {
        sprintf (text, "SYM_%d", i) ;
        if (hash_add(table, text, (void *)i)) {
            vperror("Error adding entry %d to the table.\nhash_add: ", i) ;
            exit(errno) ;
        }
    }

    count = hash_count(table);

    for (i = 0; i < count; i++) {

        if ((key = hash_get(table, i, &data)) == NULL) {

            vperror("Error unable to find index: %d\nhash_get: ", i) ;
            goto fini;

        }

        fprintf(stdout, "Value: %p    Key: \"%s\"\n", data, key);

    }

	/* Delete the hash table. */

    fini:
    if (hash_destroy(table, freeit)) {
        vperror("Error deleting the hash table.\nhash_destroy: ") ;
        exit(errno) ;
    }

    return 0;

}

