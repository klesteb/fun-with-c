#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"

/*----------------------------------------------------------------------*/

extern int vperror_print;
extern int lfn_util_debug;
extern int tcp_util_debug;

/*----------------------------------------------------------------------*/

int main (int argc, char **argv) {

    int stat;
    PjlHandle handle;
    char options[PJL_K_BUFSIZ];

    /* lfn_util_debug = 1; */
    /* tcp_util_debug = 1; */
    /* vperror_print = 1; */
    
    if (argc < 3) {

        printf("\nusage: test1 <host> <port>\n\n");
        return(0);

    }

    printf("\nUsing host %s, port %s\n", argv[1], argv[2]);

    if ((stat = pjl_create(&handle)) == OK) {

        if ((stat = pjl_open(handle, argv[1], argv[2], 2.0)) == OK) {

            pjl_start(handle);

            pjl_load_model(handle);
            pjl_load_config(handle);

            pjl_dump_model(handle);
            pjl_dump_config(handle);

            printf("\n");
            pjl_get_config(handle, "MEMORY", options, PJL_K_BUFSIZ - 1);
            printf("MEMORY = %s\n", options);

            pjl_get_config(handle, "IN TRAYS", options, PJL_K_BUFSIZ - 1);
            printf("IN TRAYS = %s\n", options);
            printf("\n");

            pjl_stop(handle);
            pjl_close(handle);

        } else printf("Unable to connect to: %s, port: %s\n", argv[1], argv[2]);

        pjl_destroy(handle);

    } else printf("Unable to allocate resources\n");

    return 0;

}

