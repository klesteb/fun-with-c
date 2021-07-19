
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

#include "jam.h"
#include "qwk.h"
#include "room.h"
#include "when.h"
#include "finds.h"
#include "errors.h"
#include "tracer.h"
#include "bitops.h"
#include "que_util.h"
#include "fnm_util.h"
#include "misc/misc.h"
#include "main/bbs_config.h"

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

int create_room(room_base_t **temp, qwk_area_t *area) {

    int stat = OK;

    when_error_in {

        errno = 0;
        *temp = calloc(1, sizeof(room_base_t));
        if (*temp == NULL) cause_error(errno);

        (**temp).aide = 1;
        (**temp).base = MSGBASE;
        (**temp).timeout = TIMEOUT;
        (**temp).retries = RETRIES;
        (**temp).conference = area->area;
        strncpy((**temp).name, area->name, 31);
        (**temp).flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES | RM_NETWORK);
        strncpy((**temp).path, fnm_build(1, FnmPath, MSGPATH, NULL), 255);

        stat = room_add(room, *temp);
        check_return(stat, room);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int process_message(qwk_header_t *header, char *text) {

    int stat = OK;

    when_error_in {
        
        
        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

    /* memset(zone, '\0', 10); */
    /* memset(output, '\0', 19); */
    /* tm = localtime(&header->date_time); */
    /* strftime(zone, 9, "%z", tm); */
    /* sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d%4s", */
    /*         1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday, */
    /*         tm->tm_hour, tm->tm_min, tm->tm_sec, zone); */

    /* printf("--------------------------------\n"); */
    /* printf("status    : %d\n", header->status); */
    /* printf("number    : %ld\n", header->number); */
    /* printf("date time : %s\n", output); */
    /* printf("to        : %s\n", header->to); */
    /* printf("from      : %s\n", header->from); */
    /* printf("subject   : %s\n", header->subject); */
    /* printf("password  : %s\n", header->password); */
    /* printf("reply     : %ld\n", header->reply); */
    /* printf("records   : %ld\n", header->records); */
    /* printf("alive     : %d\n", header->alive); */
    /* printf("conference: %d\n", header->conference); */
    /* printf("seq num   : %d\n", header->seq_number); */
    /* printf("net tag   : %s\n", header->net_tag); */
    /* printf("\n"); */
    /* printf("%s\n", text); */

}

int process_area(char *area_num) {

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

            stat = process_message(&header, text);
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

            } else {

                stat = create_room(&temp, area);
                check_status(stat, OK, E_INVOPS);

            }

            if (bit_test(temp->flags, RM_MESSAGES)) {

                stat = room_handler(room, temp, (void **)&jam);
                check_return(stat, room);

                memset(area_num, '\0', 10);
                snprintf(area_num, 5, "%03ld", area->area);

                printf("processing: %s, %s\n", area_num, area->name);

                stat = process_area(area_num);
                check_status(stat, OK, E_INVOPS);

            }

            free(area);
            stat = room_free(room, temp);
            check_return(stat, room);

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

