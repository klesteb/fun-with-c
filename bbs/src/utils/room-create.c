
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

#include "room.h"
#include "when.h"
#include "finds.h"
#include "files.h"
#include "errors.h"
#include "tracer.h"
#include "bitops.h"
#include "que_util.h"
#include "fnm_util.h"
#include "misc/misc.h"
#include "main/bbs_config.h"

room_t *room;
files_t *files;
tracer_t *dump;
errors_t *errs;

int create_room(char *name, char *description, int area, int networked) {

    int stat = OK;
    room_base_t temp;

    when_error_in {

        temp.aide = 1;
        temp.base = MSGBASE;
        temp.timeout = TIMEOUT;
        temp.retries = RETRIES;
        temp.conference = area;
        strncpy(temp.name, name, 31);
        strncpy(temp.description, description, 63);
        temp.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | RM_MESSAGES);
        strncpy(temp.path, fnm_build(1, FnmPath, MSGPATH, NULL), 255);

        if (networked) {
            
            bit_set(temp.flags, RM_NETWORK);
            
        }

        int x = 0;
        for(; x < USERNUM; x++) { 

            temp.status[x] = 0;

        }

        stat = room_add(room, &temp);
        check_return(stat, room);

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int process_file(char *path) {

    int area = 0;
    int stat = OK;
    int index = 0;
    char name[255];
    char line[256];
    ssize_t count = 0;
    int flags = O_RDONLY;
    char description[256];
    int networked = FALSE;

    when_error_in {

        stat = files_open(files, flags, 0);
        check_return(stat, files);

        printf("--------------------------------\n");

        do {

            networked = FALSE;

            /* sync */

            stat = files_gets(files, line, 255, &count);
            check_return(stat, files);
            if (count < 1) break;

            /* conference */

            stat = files_gets(files, line, 255, &count);
            check_return(stat, files);
            if (count < 1) break;

            area = atoi(trim(line));

            /* name */

            stat = files_gets(files, name, 255, &count);
            check_return(stat, files);
            if (count < 1) break;
            trim(name);

            /* description */

            stat = files_gets(files, description, 255, &count);
            check_return(stat, files);
            if (count < 1) break;
            trim(description);

            /* networked */

            stat = files_gets(files, line, 255, &count);
            check_return(stat, files);
            if (count < 1) break;

            if (stricmp(line, "true") == 0) {

                networked = TRUE;

            }

            /* check to see if room doesn't already exist */

            stat = room_find(room, &area, sizeof(short), find_room_by_conference, &index);
            check_return(stat, room);

            if (index < 1) {

                stat = create_room(name, description, area, networked);
                check_status(stat, OK, E_INVOPS);

                printf("creating room: %s\n", name);
                printf("  description: %s\n", description);
                printf("   conference: %d\n", area);
                printf("    networked: %d\n", networked);
                printf("--------------------------------\n");

            } else {

                printf("room \"%s\" already exists\n", name);

            }

        } while (count > 0);

        stat = files_close(files);
        check_return(stat, files);

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

    room_destroy(room);
    files_destroy(files);
    tracer_destroy(dump);
    errors_destroy(errs);

}

int main(int argc, char **argv) {

    int ch;
    int stat = OK;
    char path[256];
    extern int  optind;
    extern char *optarg;
    int rc = EXIT_SUCCESS;
    char *options = "f:Hh?";

    memset(path, '\0', 256);
    strcpy(path, ".");

    while ((ch = getopt(argc, argv, options)) != EOF) {

        switch (ch) {
            case 'f':
                memset(path, '\0', 256);
                strncpy(path, optarg, 255);
                break;

            case 'H':
            case 'h':
            case '?':
                printf("\n");
                printf("Usage: room-create [-f <file name>]\n");
                printf("  -f - the config file to use.\n");
                printf("\n");

                return EXIT_SUCCESS;

        }

    }

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = room_open(room);
        check_return(stat, room);

        files = files_create(path, RETRIES, TIMEOUT);
        check_creation(files);

        stat = process_file(path);
        check_status(stat, OK, E_INVOPS);

        stat = room_close(room);
        check_return(stat, room);

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

