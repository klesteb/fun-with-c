
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <string.h>
#include <descrip.h>
#include <ssdef.h>
#include <maildef.h>
#include <rmsdef.h>
#include <mail$routines.h>
#include <lib$routines.h>

#include "vms.h"
#include "que_util.h"
#include "rmsio.h"

/*----------------------------------------------------------------------*/

void vms_mail(char *to, char *from, char *cc, char *subject, void *qbody) {
/*
 * Function: vms_mail.c
 * Version : 1.0
 * Created : 18-Aug-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will send a mail message using callable mail. If you
 *    want to change who this message is from, the calling process must have
 *    SYSPRV enabled. Otherwise the from line will default to the process
 *    owner. This is the callable mail default.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long rlen;
    UWORD addr_type;
    char buffer[512];
    char filename[512];
    char *buff;
    int ctx = 0;
    int x = 0;
    ITEM_LIST_3 addr_item[3];
    ITEM_LIST_3 hdr_item[2];
    ITEM_LIST_3 null_item = {0,0,0,0};
    ITEM_LIST_3 body_item[2] = {{0,0,0,0},{0,0,0,0}};
    RFILE *fp = NULL;
    size_t cnt;
    queue *body;

/*
 * Main part of function.
 */

    body = qbody;

    stat = mail$send_begin(&ctx, &null_item, &null_item);
    vms_check_status(stat);

    /* Build the To: line.                                             */

    if (to[0] != '@') {             /* Check for a distribution list.  */

        buff = strtok(to, ",");
        while (buff != NULL) {

            SET_ITEM3(hdr_item[0], MAIL$_SEND_TO_LINE, buff, strlen(buff), &rlen);
            SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

            stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
            vms_check_status(stat);

            /* Create an Address list entry.                            */

            addr_type = MAIL$_TO;
            SET_ITEM3(addr_item[0], MAIL$_SEND_USERNAME, buff, strlen(buff), &rlen);
            SET_ITEM3(addr_item[1], MAIL$_SEND_USERNAME_TYPE, &addr_type, sizeof(UWORD), &rlen);
            SET_ITEM3(addr_item[2], 0, 0, 0, 0);

            stat = mail$send_add_address(&ctx, addr_item, &null_item);
            vms_check_status(stat);

            buff = strtok(NULL, ",");

        }

    } else {                        /* Process the distribution list.   */

        strncpy(filename, &to[1], strlen(to));
        if (strstr(filename, ".DIS") == NULL) strcat(filename, ".DIS");

        if ((fp = rms_open(filename, 
                           RMS_O_ACCESS, RMS_K_ACCESS_READ, 
                           RMS_O_ALLOW, RMS_K_ALLOW_UPDATE, 
                           RMS_O_END)) != NULL) {

            memset(buffer, '\0', sizeof(buffer));

            while (rms_getseq(fp, buffer, 512, &cnt) != RMS$_EOF) {

                SET_ITEM3(hdr_item[0], MAIL$_SEND_TO_LINE, buffer, strlen(buffer), &rlen);
                SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

                stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
                vms_check_status(stat);
                
                /* Create an Address list entry.                        */

                addr_type = MAIL$_TO;
                SET_ITEM3(addr_item[0], MAIL$_SEND_USERNAME, buffer, strlen(buffer), &rlen);
                SET_ITEM3(addr_item[1], MAIL$_SEND_USERNAME_TYPE, &addr_type, sizeof(UWORD), &rlen);
                SET_ITEM3(addr_item[2], 0, 0, 0, 0);

                stat = mail$send_add_address(&ctx, addr_item, &null_item);
                vms_check_status(stat);

                memset(buffer, '\0', sizeof(buffer));

            }

            rms_close(fp);

        }

    }

    /* Build the From: line. This requires SYSPRV.                      */

    if (strlen(from) != 0) {

        SET_ITEM3(hdr_item[0], MAIL$_SEND_FROM_LINE, from, strlen(from), &rlen);
        SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

        stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
        vms_check_status(stat);

    }

    /* Build the CC: line.                                             */

    if (strlen(cc) != 0) {

        if (cc[0] != '@') {         /* check for a distribution list.  */

            buff = strtok(cc, ",");
            while (buff != NULL) {

                SET_ITEM3(hdr_item[0], MAIL$_SEND_CC_LINE, buff, strlen(buff), &rlen);
                SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

                stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
                vms_check_status(stat);

                /* Create an Address list entry.                        */

                addr_type = MAIL$_CC;
                SET_ITEM3(addr_item[0], MAIL$_SEND_USERNAME, buff, strlen(buff), &rlen);
                SET_ITEM3(addr_item[1], MAIL$_SEND_USERNAME_TYPE, &addr_type, sizeof(UWORD), &rlen);
                SET_ITEM3(addr_item[2], 0, 0, 0, 0);

                stat = mail$send_add_address(&ctx, addr_item, &null_item);
                vms_check_status(stat);

                buff = strtok(NULL, ",");

            }

        } else {                    /* process the distribution list.   */

            strncpy(filename, &to[1], strlen(to));
            if (strstr(filename, ".DIS") == NULL) strcat(filename, ".DIS");

            if ((fp = rms_open(filename, 
                               RMS_O_ACCESS, RMS_K_ACCESS_READ, 
                               RMS_O_ALLOW, RMS_K_ALLOW_UPDATE,
                               RMS_O_END)) != NULL) {

                while ((stat = rms_getseq(fp, buffer, 512, &cnt)) != RMS$_EOF) {

                    SET_ITEM3(hdr_item[0], MAIL$_SEND_TO_LINE, buffer, strlen(buffer), &rlen);
                    SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

                    stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
                    vms_check_status(stat);
                
                    /* Create an Address list entry.                    */

                    addr_type = MAIL$_CC;
                    SET_ITEM3(addr_item[0], MAIL$_SEND_USERNAME, buffer, strlen(buffer), &rlen);
                    SET_ITEM3(addr_item[1], MAIL$_SEND_USERNAME_TYPE, &addr_type, sizeof(UWORD), &rlen);
                    SET_ITEM3(addr_item[2], 0, 0, 0, 0);

                    stat = mail$send_add_address(&ctx, addr_item, &null_item);
                    vms_check_status(stat);

                }

                rms_close(fp);

            }

        }

    }

    /* Build the Subject: line.                                        */

    if (strlen(subject) != 0) {

        SET_ITEM3(hdr_item[0], MAIL$_SEND_SUBJECT, subject, strlen(subject), &rlen);
        SET_ITEM3(hdr_item[1], 0, 0, 0, 0);

        stat = mail$send_add_attribute(&ctx, hdr_item, &null_item);
        vms_check_status(stat);

    }

    /* Build the body of the message.                                   */

    if (body != NULL) {

        for (buff = que_first(body);
             buff != NULL;
             buff = que_next(body)) {

            if (strlen(buff) == 0) buff = " ";

            SET_ITEM3(body_item[0], MAIL$_SEND_RECORD, buff, strlen(buff), &rlen);
            SET_ITEM3(body_item[1], 0,0,0,0);

            stat = mail$send_add_bodypart(&ctx, &body_item, &null_item);
            vms_check_status(stat);

        }

    }

    /* Send the message on its way.                                     */

    stat = mail$send_message(&ctx, &null_item, &null_item);
    vms_check_status(stat);

    /* Terminate the mail context.                                      */

    stat = mail$send_end(&ctx, &null_item, &null_item);
    vms_check_status(stat);

}

