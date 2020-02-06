
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ssdef.h>

#include "vms.h"
#include "mbxio.h"

#define MBXBUF 512

extern int strnicmp(const char *, const char *, int);

/*--------------------------------------------------------------------------*/

int process_buffer(char *buffer) {

    int working = TRUE;

    printf("MBX buffer = %s\n", buffer);

    if (strnicmp(buffer, "EXIT", 4) == 0) {

        working = FALSE;

    }

    return(working);

}

/**/

int main(int arg, char **argv) {

    IFILE *fd = NULL;
    ulong ef;
    ulong mask = 0;
    int stat;
    int cnt = 0;
    int working = TRUE;
    char buffer[MBXBUF + 1];

    if ((fd = mbx_open("TESTMBX", TRUE, 0, mbx_a_both, MBXBUF)) == NULL) {

        printf("Unable to open mailbox!\n");
        exit(EXIT_FAILURE);

    }

    ef = mbx_ef(fd);
    vms_set_ef_mask(ef, &mask);

    vms_queio(fd);

    while (working) {

        vms_wait_ef(ef, mask, ef_a_or);

        if (vms_check_ef(ef) == SS$_WASSET) {

            vms_clear_ef(ef);

            while ((cnt = mbx_read(fd, buffer, MBXBUF)) > 0) {

                buffer[cnt] = '\0';
                working = process_buffer(buffer);

            }

            vms_queio(fd);

        }

    }

    mbx_close(fd);

    exit(EXIT_SUCCESS);

}
    
