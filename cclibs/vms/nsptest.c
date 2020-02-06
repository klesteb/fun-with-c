
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ssdef.h>

#include "vms.h"
#include "nspio.h"

#define NSPBUF 512

extern int strnicmp(const char *, const char *, int);

/*--------------------------------------------------------------------------*/

int process_buffer(char *buffer) {

    int working = TRUE;

    printf("NSP buffer = %s\n", buffer);

    if (strnicmp(buffer, "ENDIT", 5) == 0) {

        working = FALSE;

    }

    return(working);

}

/**/

int main(int arg, char **argv) {

    IFILE *fd = NULL;
    ULONG ef;
    ULONG mask = 0;
    int stat;
    int cnt = 0;
    int working = TRUE;
    char buffer[NSPBUF + 1];
    char *procmd = "SET PROCESS/NAME=TESTING";
    char *command = "FQ";
    char *quit = "STOP_RUN";

    if ((fd = nsp_open("TREX", "", "", "TASK=QMS_SLAVE", NSPBUF)) == NULL) {

        printf("Unable to open network object!\n");
        exit(EXIT_FAILURE);

    }

    ef = nsp_ef(fd);
    vms_set_ef_mask(ef, &mask);

    vms_queio(fd);
    nsp_write(fd, procmd, strlen(procmd), 5);
    nsp_write(fd, command, strlen(command), 5);

    while (working) {

        vms_wait_ef(ef, mask, ef_a_or);

        if (vms_check_ef(ef) == SS$_WASSET) {

            vms_clear_ef(ef);

            while ((cnt = nsp_read(fd, buffer, NSPBUF)) > 0) {

                buffer[cnt] = '\0';
                working = process_buffer(buffer);

            }

            vms_queio(fd);

        }

    }

    nsp_write(fd, quit, strlen(quit), 5);
    nsp_close(fd);

    exit(EXIT_SUCCESS);

}
    
