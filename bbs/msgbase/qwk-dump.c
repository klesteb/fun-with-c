
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
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

#include "qwk.h"
#include "when.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "que_util.h"
#include "misc/misc.h"

qwk_t *qwk;
tracer_t *dump;
errors_t *errs;

char path[256];
char conference[80];

int display_message(qwk_header_t *header, char *text) {

    struct tm *tm;
    int stat = OK;
    char output[19];
    char zone[10];

    memset(zone, '\0', 10);
    memset(output, '\0', 19);
    tm = localtime(&header->date_time);
    strftime(zone, 9, "%z", tm);
    sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d%4s",
            1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec, zone);

    printf("--------------------------------\n");
    printf("status    : %d\n", header->status);
    printf("number    : %ld\n", header->number);
    printf("date time : %s\n", output);
    printf("to        : %s\n", header->to);
    printf("from      : %s\n", header->from);
    printf("subject   : %s\n", header->subject);
    printf("password  : %s\n", header->password);
    printf("reply     : %ld\n", header->reply);
    printf("records   : %ld\n", header->records);
    printf("alive     : %d\n", header->alive);
    printf("conference: %d\n", header->conference);
    printf("seq num   : %d\n", header->seq_number);
    printf("net tag   : %s\n", header->net_tag);
    printf("\n");
    printf("%s\n", text);

    return stat;

}

int process_packet(void) {

    char *text;
    int stat = OK;
    qwk_ndx_t ndx;
    char area_num[10];
    ssize_t count = 0;
    qwk_header_t header;
    qwk_control_t control;
    qwk_area_t *area = NULL;

    when_error_in {

        stat = qwk_open(qwk);
        check_return(stat, qwk);

        stat = qwk_get_control(qwk, &control);
        check_return(stat, qwk);

        while ((area = que_pop_tail(&control.areas))) {

            memset(area_num, '\0', 10);
            snprintf(area_num, 4, "%03ld", area->area);

            printf("Processing: %s\n", area->name);

            stat = qwk_open_ndx(qwk, area_num);
            check_return(stat, qwk);

            stat = qwk_get_ndx(qwk, &ndx, &count);
            check_return(stat, qwk);

            while (count > 0) {

                stat = qwk_get_message(qwk, ndx.index, &header, &text);
                check_return(stat, qwk);

                stat = display_message(&header, text);
                check_status(stat, OK, E_INVOPS);

                free(text);

                stat = qwk_get_ndx(qwk, &ndx, &count);
                check_return(stat, qwk);

            }

            stat = qwk_close_ndx(qwk);
            check_return(stat, qwk);

            free(area);

        }

        stat = qwk_close(qwk);
        check_return(stat, qwk);

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
    int timeout = 1;
    char retries = 30;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);

        qwk = qwk_create(path, retries, timeout, FALSE, dump);
        check_creation(qwk);
        
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
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int ch;
    int stat = OK;
    extern int  optind;
    extern char *optarg;
    int rc = EXIT_SUCCESS;
    char *options = "a:d:Hh?";


    memset(path, '\0', 256);
    strcpy(path, ".");

    while ((ch = getopt(argc, argv, options)) != EOF) {

        switch (ch) {
            case 'a':
                memset(conference, '\0', 80);
                strncpy(conference, optarg, 79);
                break;

            case 'd':
                memset(path, '\0', 256);
                strncpy(path, optarg, 255);
                break;

            case 'H':
            case 'h':
            case '?':
                printf("\n");
                printf("Usage: qwk-dump [-a <area>] [-d <directory>]\n");
                printf("  -a - the area to use when dumping the messages.\n");
                printf("  -d - the directory where the qwk packet is located.\n");
                printf("\n");
                printf("  The default is to dump all messages in the current directory.\n");
                printf("\n");

                return EXIT_SUCCESS;

        }            

    }

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = process_packet();
        check_status(stat, OK, E_INVOPS);

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

