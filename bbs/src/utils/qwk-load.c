
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "include/when.h"
#include "gpl/fnm_util.h"
#include "gpl/str_util.h"
#include "tracer/tracer.h"
#include "include/errors.h"
#include "cclibs/que_util.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/bitops.h"
#include "bbs/src/jam/jam.h"
#include "bbs/src/qwk/qwk.h"
#include "bbs/src/room/room.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/main/bbs_config.h"

qwk_t *qwk;
jam_t *jam;
room_t *room;
tracer_t *dump;
errors_t *errs;

char qwk_path[256];


void display_control(qwk_control_t *control) {

    struct tm *tm;
    char output[19];

    memset(output, '\0', 19);
    tm = localtime(&control->date_time);
    sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);

    printf("--------------------------------\n");
    printf("bbs name : %s\n", control->bbs_name);
    printf("city     : %s\n", control->city);
    printf("state    : %s\n", control->state);
    printf("phone    : %s\n", control->phone);
    printf("sysop    : %s\n", control->sysop);
    printf("serial # : %s\n", control->serial_num);
    printf("bbs id   : %s\n", control->bbs_id);
    printf("num areas: %ld\n", control->num_areas);
    printf("date time: %s\n", output);

}

int process_message(room_base_t *temp, qwk_header_t *header, char *text) {

    int len;
    int offset;
    queue fields;
    int stat = OK;
    char buffer[256];
    jam_field_t *field = NULL;
    jam_field_t *field1 = NULL;
    jam_field_t *field2 = NULL;
    jam_field_t *field3 = NULL;
    jam_field_t *field4 = NULL;
    jam_field_t *field5 = NULL;
    jam_field_t *field6 = NULL;
    jam_field_t *field7 = NULL;
    jam_field_t *field8 = NULL;
    jam_message_t *message = NULL;

    when_error_in {

        que_init(&fields);
        check_status(stat, QUE_OK, E_INVOPS);

        stat = jam_new_message(jam, &message);
        check_return(stat, jam);

        /* set various message fields */

        message->reply_to = header->reply;
        message->date_received = time(NULL);
        message->date_written = header->date_time;

        /* translate the qwk status into something usefull */

        if ((header->status == QWK_PRIVATE) && 
            (bit_test(temp->flags, RM_PRIVATE))) {

            bit_set(message->attribute, MSG_PRIVATE);

        }

        if ((header->status == QWK_PRIVATE) || 
            (header->status == QWK_PUB_READ) ||
            (header->status == QWK_SYS_READ)) {

            bit_set(message->attribute, MSG_READ);

        }

        /* search for qwke extensions */

        /* message sender (From:) */

        memset(buffer, '\0', 256);
        if ((offset = pos(text, "From: ", 0)) > 0) {

            len = xindex(text, 10, offset + 6);
            strncpy(buffer, mid(text, offset + 6, len), 255);
            str_remove(len + 7, offset - 1, text);

        } else {

            strcpy(buffer, header->from);

        }

        stat = jam_new_field(jam, JAMSFLD_SENDERNAME, buffer, &field1);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field1);
        check_status(stat, QUE_OK, E_NOQUEUE);

        /* message receiver (To:) */

        memset(buffer, '\0', 256);
        if ((offset = pos(text, "To: ", 0)) > 0) {

            len = xindex(text, 10, offset + 4);
            strncpy(buffer, mid(text, offset + 4, len), 255);
            str_remove(len + 5, offset - 1, text);

        } else {

            strcpy(buffer, header->to);

        }

        stat = jam_new_field(jam, JAMSFLD_RECVRNAME, buffer, &field2);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field2);
        check_status(stat, QUE_OK, E_NOQUEUE);

        /* message subject (Subject:) */

        memset(buffer, '\0', 256);
        if ((offset = pos(text, "Subject: ", 0)) > 0) {

            len = xindex(text, 10, offset + 9);
            strncpy(buffer, mid(text, offset + 9, len), 255);
            str_remove(len + 10, offset - 1, text);

        } else {

            strcpy(buffer, header->subject);

        }

        stat = jam_new_field(jam, JAMSFLD_SUBJECT, buffer, &field3);
        check_return(stat, jam);

        stat = que_push_tail(&fields, field3);
        check_status(stat, QUE_OK, E_NOQUEUE);

        /* search for synchronet qwk extensions */

        if ((offset = pos(text, "@VIA: ", 0)) > 0) {

            memset(buffer, '\0', 256);
            len = xindex(text, 10, offset + 6);
            strncpy(buffer, mid(text, offset + 6, len), 255);
            str_remove(len + 7, offset - 1, text);

            stat = jam_new_field(jam, JAMSFLD_OADDRESS, buffer, &field4);
            check_return(stat, jam);

            stat = que_push_tail(&fields, field4);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        if ((offset = pos(text, "@MSGID: ", 0)) > 0) {

            memset(buffer, '\0', 256);
            len = xindex(text, 10, offset + 8);
            strncpy(buffer, mid(text, offset + 8, len), 255);
            str_remove(len + 9, offset - 1, text);

            stat = jam_new_field(jam, JAMSFLD_MSGID, buffer, &field6);
            check_return(stat, jam);

            stat = que_push_tail(&fields, field6);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        if ((offset = pos(text, "@REPLY: ", 0)) > 0) {

            memset(buffer, '\0', 256);
            len = xindex(text, 10, offset + 8);
            strncpy(buffer, mid(text, offset + 8, len), 255);
            str_remove(len + 9, offset - 1, text);

            stat = jam_new_field(jam, JAMSFLD_REPLYID, buffer, &field7);
            check_return(stat, jam);

            stat = que_push_tail(&fields, field7);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        if ((offset = pos(text, "@REPLYTO: ", 0)) > 0) {

            memset(buffer, '\0', 256);
            len = xindex(text, 10, offset + 10);
            strncpy(buffer, mid(text, offset + 10, len), 255);
            str_remove(len + 11, offset - 1, text);

            stat = jam_new_field(jam, JAMSFLD_REPLYTO, buffer, &field8);
            check_return(stat, jam);

            stat = que_push_tail(&fields, field8);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        if ((offset = pos(text, "@TZ: ", 0)) > 0) {

            memset(buffer, '\0', 256);
            len = xindex(text, 10, offset + 5);
            strncpy(buffer, mid(text, offset + 5, len), 255);
            str_remove(len + 6, offset - 1, text);

            stat = jam_new_field(jam, JAMSFLD_TZUTCINFO, buffer, &field5);
            check_return(stat, jam);

            stat = que_push_tail(&fields, field5);
            check_status(stat, QUE_OK, E_NOQUEUE);

        }

        /* set time and the local timezone */

        /* memset(zone, '\0', 10); */
        /* tm = localtime(&message->date_written); */
         /* strftime(zone, 9, "%z", tm); */

        /* stat = jam_new_field(jam, JAMSFLD_TZUTCINFO, zone, &field1); */
        /* check_return(stat, jam); */

        /* stat = que_push_tail(&fields, field2); */
        /* check_status(stat, QUE_OK, E_INVOPS); */

        /* add the message to the msgbase */

        stat = jam_add_message(jam, message, &fields, text);
        check_return(stat, jam);

        /* free resoures */

        free(message);

        while ((field = que_pop_head(&fields))) {

            stat = jam_free_field(jam, field);
            check_return(stat, jam);

        }

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int process_area(room_base_t *temp, char *area_num) {

    char *text;
    int stat = OK;
    qwk_ndx_t ndx;
    ssize_t count = 0;
    qwk_header_t header;

    when_error_in {

        stat = qwk_open_ndx(qwk, area_num);
        check_return(stat, qwk);

        stat = qwk_get_ndx(qwk, &ndx, &count);
        check_return(stat, qwk);

        while (count > 0) {

            stat = qwk_get_message(qwk, ndx.index, &header, &text);
            check_return(stat, qwk);

            stat = process_message(temp, &header, text);
            check_status(stat, OK, E_INVOPS);

            free(text);

            stat = qwk_get_ndx(qwk, &ndx, &count);
            check_return(stat, qwk);

        }

        stat = qwk_close_ndx(qwk);
        check_return(stat, qwk);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int process_packet(void) {

    int stat = OK;
    int index = 0;
    char area_num[10];
    qwk_control_t control;
    qwk_area_t *area = NULL;
    room_base_t *temp = NULL;

    when_error_in {

        stat = qwk_get_control(qwk, &control);
        check_return(stat, qwk);

        display_control(&control);

        printf("--------------------------------\n");

        while ((area = que_pop_tail(&control.areas))) {

            stat = room_find(room, &area->area, sizeof(short), find_room_by_conference, &index);
            check_return(stat, room);

            if (index > 0) {

                stat = room_get(room, index, &temp);
                check_return(stat, room);

                if (bit_test(temp->flags, RM_MESSAGES)) {

                    stat = room_handler(room, temp, (void **)&jam);
                    check_return(stat, room);

                    memset(area_num, '\0', 10);
                    snprintf(area_num, 5, "%03ld", area->area);

                    printf("processing: %s, %s\n", area_num, area->name);

                    stat = process_area(temp, area_num);
                    check_status(stat, OK, E_INVOPS);

                }

                free(area);
                stat = room_free(room, temp);
                check_return(stat, room);

            } else {

                printf("not defined: \"%s\"\n", area->name);

            }

        }

        printf("--------------------------------\n");

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    int base = MSGBASE;
    char msgs_path[256];
    char room_path[256];
    int timeout = TIMEOUT;
    char retries = RETRIES;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        qwk = qwk_create(qwk_path, retries, timeout, FALSE, dump);
        check_creation(qwk);

        strncpy(msgs_path, fnm_build(1, FnmPath, MSGPATH, NULL), 255);
        strncpy(room_path, fnm_build(1, FnmPath, DATAPATH, NULL), 255);

        room = room_create(room_path, msgs_path, retries, timeout, base, FALSE, dump);
        check_creation(room);
        
        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

    qwk_destroy(qwk);
    jam_destroy(jam);
    room_destroy(room);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int ch;
    int stat = OK;
    extern int  optind;
    extern char *optarg;
    int rc = EXIT_SUCCESS;
    char *options = "d:Hh?";

    memset(qwk_path, '\0', 256);
    strcpy(qwk_path, ".");

    while ((ch = getopt(argc, argv, options)) != EOF) {

        switch (ch) {
            case 'd':
                memset(qwk_path, '\0', 256);
                strncpy(qwk_path, optarg, 255);
                break;

            case 'H':
            case 'h':
            case '?':
                printf("\n");
                printf("Usage: qwk-load [-d <directory>]\n");
                printf("  -d - the directory where the qwk packet is located.\n");
                printf("\n");
                printf("  The default is to load all messages in the current directory.\n");
                printf("\n");

                return EXIT_SUCCESS;

        }

    }

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = qwk_open(qwk);
        check_return(stat, qwk);

        stat = room_open(room);
        check_return(stat, room);

        stat = process_packet();
        check_status(stat, OK, E_INVOPS);

        stat = room_close(room);
        check_return(stat, room);

        stat = qwk_close(qwk);
        check_return(stat, qwk);

        exit_when;

    } use {

        rc = EXIT_FAILURE;
        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return rc;

}

