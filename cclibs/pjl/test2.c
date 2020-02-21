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

        printf("\nusage: test2 <host> <port>\n\n");
        return(0);

    }

    printf("\nUsing host %s, port %s\n", argv[1], argv[2]);

    if ((pjl_open(argv[1], argv[2], 20.0, &handle)) == 0) {

        pjl_start(handle);

        pjl_load_variables(handle);
        pjl_dump_variables(handle);

        pjl_stop(handle);
        pjl_close(handle);

    } else printf("Unable to attach to printer!\n");

    return 0;

}

