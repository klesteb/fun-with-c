#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pjl_util.h"
#include "lfn_util.h"
#include "tcp_util.h"

/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/

int main (int argc, char **argv) {

    /* print a text file on a printer */
    /* with job seperation            */

    int stat;
    FILE *fd = NULL;
    PjlHandle handle;
    char buffer[1024];
    char *fmt = "%s\r\n";
    char *jobname = "kevin";

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

    if ((stat = pjl_job(handle, jobname, 0, 0, 0, NULL)) != OK) {

        printf("unable to declare job\n");
        goto fini;

    }
    
    if ((stat = pjl_enter(handle, "PCL")) != OK) {

        printf("unable to set language\n");
        goto fini;

    }

    printf("Sending file %s to the printer\n", argv[3]);

    if ((fd = fopen(argv[3], "r")) != NULL) {

        int count = 0;
        size_t length = 0;
        char *line = NULL;

        while ((count = getline(&line, &length, fd)) > 0) {

            snprintf(buffer, count + 2, fmt, line);

            if ((stat = _pjl_write(handle, buffer, strlen(buffer))) != OK) {

                printf("Error printing file\n");
                break;

            }

        }

        free(line);
        fclose(fd);

    }

    printf("File %s has been sent\n", argv[3]);

    if ((stat = pjl_eoj(handle, jobname)) != OK) {

        printf("unable to terminate job\n");

    }

    fini:
    pjl_stop(handle);
    pjl_close(handle);
    pjl_destroy(handle);

    return 0;

}

