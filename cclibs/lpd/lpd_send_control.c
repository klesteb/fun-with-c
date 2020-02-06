
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

void * xmalloc(int);

/*----------------------------------------------------------------------*/

/**/

int lpd_send_control(

#    if __STDC__
    LpdHandle handle, char *host, int job, item_list *items)
#    else
    handle, host, job, items)

    LpdHandle handle;
    char *host;
    int job;
    item_list *items;
#    endif

{
/*
 * Function: lpd_start_job.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_send_control() will send the "receive control file" 
 *    command.
 *
 *    The control file must be an ASCII stream with the ends of lines
 *    indicated by ASCII LF. The total number of bytes in the stream is
 *    sent as the first operand. The name of the control file is sent as
 *    the second. It should start with ASCII "cfA", followed by a three
 *    digit job number, followed by the host name which has constructed the
 *    control file. Acknowledgement processing must occur as usual after
 *    the command is sent.
 *
 *    The next set of octets over the same TCP connection are the
 *    intended contents of the control file.  Once all of the contents have
 *    been delivered, an octet of zero bits is sent as an indication that
 *    the file being sent is complete. A second level of acknowledgement
 *    processing must occur at this point.
 *
 *    The "H" and the "P" commands are required (print job source host, 
 *    and user identification). In addition, a print command is also 
 *    required, followed by the name of the data file to print. At 
 *    least some versions of Linux LPD daemons also require a "N" 
 *    line (i.e. "N \n"), otherwise being unable to compute job 
 *    size when later being asked for job status.
 *
 *    Invocation:
 *
 *        status = lpd_send_control(handle, host, job, items);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <host>              - I
 *            Is the name of the local host.
 *
 *        <job>               - I
 *            A job item number. RFC-1179 says this must be between 1 and 999.
 *
 *        <items>             - I
 *            A list of items describing the job to be submited.
 *
 *        <status>            - O
 *            This function will return 0.
 *
 * Modification History
 *
 * Variables Used
 */

    queue ctl_list;
    int x;
    int stat = -1;
    int ctl_size = 0;
    int h_recv = FALSE;
    int p_recv = FALSE;
    int n_recv = FALSE;
    int pc_recv = FALSE;
    char *ctl_item = NULL;
    char buffer[LPD_K_BUFSIZ];

/*
 * Main part of function.
 */

    que_init(&ctl_list);

    for (x = 0;; x++) {

        if ((items[x].code == 0) && (items[x].buf_len == 0)) break;

        ctl_item = xmalloc(items[x].buf_len + 3);

        switch (items[x].code) {
            case LPD_CLASS:
                sprintf(ctl_item, "C%s\n", (char *)items[x].buffer);
                break;

            case LPD_HOST:
                sprintf(ctl_item, "H%s\n", (char *)items[x].buffer);
                h_recv = TRUE;
                break;

            case LPD_INDENT:
                sprintf(ctl_item, "I%s\n", (char *)items[x].buffer);
                break;

            case LPD_JOBNAM:
                sprintf(ctl_item, "J%s\n", (char *)items[x].buffer);
                break;

            case LPD_BANNER:
                sprintf(ctl_item, "L%s\n", (char *)items[x].buffer);
                break;

            case LPD_MAIL:
                sprintf(ctl_item, "M%s\n", (char *)items[x].buffer);
                break;

            case LPD_FILNAM:
                memcpy(buffer, items[x].buffer, items[x].buf_len);
                if (items[x].buf_len > 131) buffer[131] = '\0';
                sprintf(ctl_item, "N%s\n", buffer); 
                n_recv = TRUE;
                break;
            
            case LPD_USERID:
                memcpy(buffer, items[x].buffer, items[x].buf_len);
                if (items[x].buf_len > 31) buffer[31] = '\0';
                sprintf(ctl_item, "P%s\n", buffer); 
                p_recv = TRUE;
                break;

            case LPD_SYMLNK:
                free(ctl_item);
                continue;

            case LPD_TITLE:
                memcpy(buffer, items[x].buffer, items[x].buf_len);
                if (items[x].buf_len > 79) buffer[79] = '\0';
                sprintf(ctl_item, "T%s\n", buffer); 
                break;

            case LPD_UNLINK:
                sprintf(ctl_item, "U%s\n", (char *)items[x].buffer);
                break;

            case LPD_WIDTH:
                sprintf(ctl_item, "W%d\n", (int)items[x].buffer);
                break;

            case LPD_PRT_TROFFR:
                sprintf(ctl_item, "1%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_TROFFI:
                sprintf(ctl_item, "2%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_TROFFB:
                sprintf(ctl_item, "3%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_TROFFS:
                sprintf(ctl_item, "4%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_CIF:
                sprintf(ctl_item, "c%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_DVI:
                sprintf(ctl_item, "d%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_TEXT:
                sprintf(ctl_item, "f%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_PLOT:
                sprintf(ctl_item, "g%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_PRTCTL:
                sprintf(ctl_item, "l%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_DTROFF:
                sprintf(ctl_item, "n%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_POST:
                sprintf(ctl_item, "o%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_PR:
                sprintf(ctl_item, "p%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_FOR:
                sprintf(ctl_item, "r%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_TROFF:
                sprintf(ctl_item, "t%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PRT_RASTER:
                sprintf(ctl_item, "v%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            case LPD_PALL:
                sprintf(ctl_item, "z%s\n", (char *)items[x].buffer);
                pc_recv = TRUE;
                break;

            default:
                free(ctl_item);
                continue;

        }

        ctl_size += strlen(ctl_item);
        que_push_tail(&ctl_list, ctl_item);

    }

    /* Check for the minimum control information. If it is not all      */
    /* there then abort processing, free memory and return a -1.        */

    if ((p_recv != TRUE) && (h_recv != TRUE) && 
        (n_recv != TRUE) && (pc_recv != TRUE)) {

        stat = -1;
        goto fini;

    }

    /* Construct the control file name.                                 */

    stat = lfn_putline(handle->stream, handle->timeout, "\002%d cfA%03d%s\n", ctl_size, job, host);
    if (stat != 0) goto fini;

    /* Wait for the control file ack.                                   */

    if ((stat = lpd_wait_ack(handle)) == 0) {

        /* Send the control information.                                */

        for (ctl_item = que_first(&ctl_list);
             ctl_item != NULL;
             ctl_item = que_next(&ctl_list)) {

            stat = lfn_putline(handle->stream, handle->timeout, ctl_item);
            if (stat != 0) goto fini;

        }

        /* Send an EOF to indicate the end of the control info.         */

        stat = lpd_eof(handle);
        if (stat != 0) goto fini;

        /* Wait for the EOF ack.                                        */

        stat = lpd_wait_ack(handle);

    }

    fini:

    /* Free used memory.                                                */

    while ((ctl_item = que_pop_tail(&ctl_list)) != NULL) {

        free(ctl_item);

    }

    return(stat);

}

