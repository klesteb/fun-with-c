#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"
#include "lfn_util.h"
#include "tcp_util.h"

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/

int main (int argc, char **argv) {

    int stat;
    FILE *fd = NULL;
    PjlHandle handle;
    char buffer[512];

    /* lfn_util_debug = 1; */
    /* tcp_util_debug = 1; */

    if (argc < 4) {

        printf("\nusage: test8 <host> <port> <filename>\n\n");
        return(0);

    }

    printf("\nUsing host %s, port %s\n", argv[1], argv[2]);

    if ((stat = pjl_create(&handle)) != OK) {

        printf("Unable to allocate resources\n");
        goto fini;

    }

    if ((stat = pjl_open(handle, argv[1], argv[2], 2.0)) != OK) {

        printf("Unable to connect to %s, on %s\n", argv[1], argv[2]);
        goto fini;

    }

    if ((stat = pjl_start(handle)) != OK) goto fini;
    if ((stat = pjl_load_model(handle)) != OK) goto fini;
    if ((stat = pjl_load_config(handle)) != OK) goto fini;
    if ((stat = pjl_load_variables(handle)) != OK) goto fini;
    if ((stat = pjl_load_ustatus(handle)) != OK) goto fini;
    if ((stat = pjl_echo(handle)) != OK) goto fini;
    if ((stat = pjl_enter(handle, "PCL")) != OK) goto fini;

    printf("Sending file %s to the printer\n", argv[3]);

    if ((fd = fopen(argv[3], "r")) != NULL) {

        int count = 0;

        while ((count = fread(buffer, 512, 1, fd)) > 0) {

            if ((stat = pjl_write(handle, buffer, count)) != OK) {

                printf("Error printing file\n");
                break;

            }

        }

        fclose(fd);

    }

    fini:
    pjl_stop(handle);
    pjl_close(handle);
    pjl_destroy(handle);

    return 0;

}

