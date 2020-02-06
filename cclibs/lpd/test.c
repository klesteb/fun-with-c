
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <rmsdef.h>
#include <starlet.h>
#include <lib$routines.h>

#include "vms.h"
#include "rmsio.h"
#include "lpd_util.h"

#define BUFSIZE 512

extern int preprocess(char *, char *);

int send_file(LpdHandle handle, char *tempfile, int file_size) {

    int x;
    int stat;
    int blocks;
    int written = 0;
    size_t count;
    char ibuf[BUFSIZE];
    RFILE *fd = NULL;


    if ((fd = rms_open_blk(tempfile, RMS_O_ACCESS, RMS_K_ACCESS_READ, 
                           RMS_O_ALLOW, RMS_K_ALLOW_NONE, RMS_O_END)) != NULL) {

        /* The temporary file is padded with NULs on the end. So read   */
        /* and send the first part of the file in block mode. When we   */
        /* are down to the last block, only send that buffer up to the  */
        /* trailing NUls.                                               */

        blocks = file_size / BUFSIZE;

        for (x = 0; x < blocks; x++) {

            memset(ibuf, '\0', BUFSIZE);

            stat = rms_read(fd, ibuf, BUFSIZE, &count);
            vms_check_status(stat);

            stat = lpd_put(handle, count, ibuf);
            if (stat != 0) goto fini;

            written += count;

        }    

        memset(ibuf, '\0', BUFSIZE);

        stat = rms_read(fd, ibuf, BUFSIZE, &count);
        vms_check_status(stat);

        count = file_size - written;

        stat = lpd_put(handle, count, ibuf);

    }

    fini:

    rms_close(fd);
    unlink(tempfile);

    return(stat);

}

int main(int argc, char **argv) {

    int stat;
    int job = 1;
    int file_size;
    RFILE *fd = NULL;
    LpdHandle handle;
    item_list items[6];
    char *host = "trex.wsipc.org";
    char *port = "lpd";
    char *server = "docusp.nwrdc.wednet.edu";
    char *user = "coopmgr";
    char *filename = "lpd_util.c";
    char *que = "remote_vms";
    char tempfile[256];
    double timeout = -1.0;
    char *xeroxopts = "(account=coopmgr,recipent=Kevin Esteb,duplex)";

    /* Create a temporary filename.                                     */

    vms_tempfile("[]", tempfile, 255);

    /* Preprocess the file, converting the carriage control into        */
    /* something usefull along with including all of the job setup      */
    /* information.                                                     */

    file_size = preprocess(filename, tempfile);
    if (file_size == -1) goto fini;

    /* Open a connection to the LPD server. On VMS this requires the    */
    /* SYSPRV privilege, on Unix it requires "ROOT" access. The LPD     */
    /* servers are very picky about file sizes and they don't always    */
    /* follow what RFC-1179 indicates. We are going to send the data    */
    /* information before the control information.                      */

    if (lpd_open(server, port, timeout, &handle) == 0) {

        if (lpd_start_job(handle, que) == 0) {

            if (lpd_start_data(handle, host, job, file_size) == 0) {

                stat = send_file(handle, tempfile, file_size);
                if (stat != 0) goto fini;

                lpd_end_data(handle);

                SET_ITEM(items[0], LPD_HOST, host, strlen(host), 0);
                SET_ITEM(items[1], LPD_USERID, user, strlen(user), 0);
                SET_ITEM(items[2], LPD_FILNAM, filename, strlen(filename), 0);
                SET_ITEM(items[3], LPD_CLASS, xeroxopts, strlen(xeroxopts), 0);
                SET_ITEM(items[4], LPD_PRT_TEXT, 0, 0, 0);
                SET_ITEM(items[5], 0, 0, 0, 0);

                lpd_send_control(handle, host, job, items);

            }

            lpd_end_job(handle);

        }

    }

    fini:

    lpd_close(handle);

    exit(EXIT_SUCCESS);

}

