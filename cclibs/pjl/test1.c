#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"
#include "opt_util.h"
#include "tcp_util.h"
#include "lfn_util.h"

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/

int main (int argc, char **argv) {

    PjlHandle handle;

    /* opt_util_debug = 1; */
    /* tcp_util_debug = 1; */
    /* lfn_util_debug = 1; */
    
    if (argc < 3) {

        printf("usage: test1 <host> <port>\n");
        return(0);

    }

    printf("Using host %s, port %s\n", argv[1], argv[2]);

    if ((pjl_open(argv[1], argv[2], 2.0, &handle)) == 0) {

        pjl_start(handle);

        pjl_load_config(handle);
        pjl_dump_config(handle);
        
        pjl_stop(handle);
        pjl_close(handle);

    } else printf("Unable to attach printer!\n");

    return 0;

}

