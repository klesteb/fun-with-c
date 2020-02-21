#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"
#include "opt_util.h"
#include "tcp_util.h"

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/

int main (int argc, char **argv) {

    PjlHandle handle;

    /* opt_util_debug = 1; */
    /* tcp_util_debug = 1; */

    if (argc < 3) {

        printf("\nusage: test3 <host> <port>\n\n");
        return(0);

    }

    printf("\nUsing host %s, port %s\n", argv[1], argv[2]);

    if ((pjl_open(argv[1], argv[2], 2.0, &handle)) == 0) {

        pjl_start(handle);

        pjl_load_ustatus(handle);
        pjl_dump_ustatus(handle);

        pjl_stop(handle);
        pjl_close(handle);

    } else printf("Unable to attach printer!\n");

    return 0;

}

