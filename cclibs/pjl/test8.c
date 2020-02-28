#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"
#include "lfn_util.h"
#include "tcp_util.h"
#include "misc/misc.h"

/*----------------------------------------------------------------------*/

extern int vperror_print;

/*----------------------------------------------------------------------*/

int file_read(FILE *fp, void *line, int size) {

    int stat = ERR;

    if (fgets(line, size, fp) != NULL) {

        if (strlen(line) < (size - 2)) {

            rtrim(line);
            strcat(line, "\r\n");

        }

        stat = OK;

    }

    return stat;

}

int main (int argc, char **argv) {

    /* print a text file on a printer */

    int stat = ERR;
    PjlHandle handle;

    /* lfn_util_debug = 1; */
    /* tcp_util_debug = 1; */
    /* vperror_print = 1; */

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

    if ((stat = pjl_start(handle)) != OK) {

        printf("unable to \"start\"\n");
        goto fini;

    }

    if ((stat = pjl_load_model(handle)) != OK) {

        printf("unable to load model\n");
        goto fini;

    }

    if ((stat = pjl_load_config(handle)) != OK) {

        printf("unable to load config\n");
        goto fini;

    }

    if ((stat = pjl_load_variables(handle)) != OK) {

        printf("unable to load variables\n");
        goto fini;

    }

    if ((stat = pjl_load_ustatus(handle)) != OK) {

        printf("unable to load ustatus\n");
        goto fini;

    }

    if ((stat = pjl_echo(handle)) != OK) {

        printf("unable to sync with printer\n");
        goto fini;

    }

    if ((stat = pjl_enter(handle, "PCL")) != OK) {

        printf("unable to set language\n");
        goto fini;

    }

    printf("Sending file %s to the printer\n", argv[3]);

    if ((stat = pjl_print(handle, argv[3], &file_read)) != OK) {

        printf("Unable to send file %s\n", argv[3]);
        goto fini;

    }

    printf("File %s has been sent\n", argv[3]);

    fini:
    pjl_stop(handle);
    pjl_close(handle);
    pjl_destroy(handle);

    return 0;

}

