#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "pjl_util.h"
#include "meo_util.h"
#include "misc/misc.h"

/*----------------------------------------------------------------------*/

extern int vperror_print;
extern int lfn_util_debug;
extern int tcp_util_debug;

/*----------------------------------------------------------------------*/

int status_read(PjlHandle handle) {

    int stat = OK;
    char *y = NULL;

    while ((pjl_getline(handle, &y) != EWOULDBLOCK)) {

        if (pos(y, "END", 0) > 0) {

            stat = ERR;

        }

        meo_dumpd(stdout, "    ", 0, y, strlen(y));
        printf("\n");

    }

    return stat;

}

int file_read(FILE *fp, void *line, int size) {

    int count = 0;
    int stat = ERR;
    size_t length = 0;
    char *buffer = NULL;

    if ((count = getline(&buffer, &length, fp)) > 0) {

        if ((count + 2 ) < size) {

            snprintf(line, count + 2, "%s\r\n", buffer);

        } else {

            snprintf(line, size, "%s", buffer);

        }

        stat = OK;
        free(buffer);

    }

    return stat;

}

int main (int argc, char **argv) {

    /* print a text file on a printer */

    int stat = ERR;
    PjlHandle handle;
    char *jobname = "kevin";

    /* lfn_util_debug = 1; */
    /* tcp_util_debug = 1; */
    /* vperror_print = 1; */

    if (argc < 4) {

        printf("\nusage: test10 <host> <port> <filename>\n\n");
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

    if ((stat = pjl_ustatus(handle, "JOB", "ON")) != OK) {

        printf("unable to set ustatus JOB to ON\n");
        goto fini;

    }

    if ((stat = pjl_job(handle, jobname, 0, 0, 0, NULL)) != OK) {

        printf("unable to set language\n");
        goto fini;

    }

    if ((stat = pjl_enter(handle, "PCL")) != OK) {

        printf("unable to set language\n");
        goto fini;

    }

    printf("Sending file %s to the printer\n", argv[3]);

    if ((stat = pjl_print(handle, argv[3], &file_read, &status_read)) != OK) {

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

