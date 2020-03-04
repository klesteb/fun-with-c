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

    /* lfn_util_debug = 1; */
    /* tcp_util_debug = 1; */
    /* vperror_print = 1; */

    if (argc < 3) {

        printf("\nusage: test4 <host> <port>\n\n");
        return(0);

    }

    printf("\nUsing host %s, port %s\n", argv[1], argv[2]);

    if ((stat = pjl_create(&handle)) == OK) {

        if ((stat = pjl_open(handle, argv[1], argv[2], 20.0)) == OK) {

            pjl_start(handle);
            pjl_stop(handle);
            pjl_close(handle);

        } else printf("Unable to connect to: %s, port: %s\n", argv[1], argv[2]);

        pjl_destroy(handle);

    } else printf("Unable to allocate resources\n");

    return 0;

}

