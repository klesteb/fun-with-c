
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

#include "files/files.h"
#include "include/when.h"
#include "gpl/fnm_util.h"
#include "tracer/tracer.h"
#include "errors/errors.h"
#include "cclibs/que_util.h"
#include "cclibs/misc/misc.h"

#include "bbs/src/bitops.h"
#include "bbs/src/room/room.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/main/bbs_errors.h"
#include "bbs/src/main/bbs_config.h"
#include "bbs/src/main/bbs_error_codes.h"

room_t *room;
files_t *files;
tracer_t *dump;
errors_t *errs;

int create_room(char *name, char *description, int area, int flags, 
                int networked, char *path) {

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
        temp.flags = (RM_PERMROOM | RM_PUBLIC | RM_INUSE | flags);
        strncpy(temp.path, fnm_build(1, FnmPath, path, NULL), 255);

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
    char resource[256];
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

            /* resource path */

            stat = files_gets(files, resource, 255, &count);
            check_return(stat, files);
            if (count < 1) break;

            /* room type */

            stat = files_gets(files, line, 255, &count);
            check_return(stat, files);
            if (count < 1) break;

            if (stricmp(line, "subsys") == 0) {

                flags = RM_SUBSYS;

            } else if (stricmp(line, "directory") == 0) {

                flags = RM_DIRECTORY;

            } else if (strcmp(line, "bulletin") == 0) {

                flags = RM_BULLETIN;

            } else {

                flags = RM_MESSAGES;

            }

            /* check to see if room doesn't already exist */

            stat = room_find(room, &area, sizeof(short), find_room_by_conference, &index);
            check_return(stat, room);

            if (index < 1) {

                stat = create_room(name, description, area, flags, networked, 
                                   resource);
                check_status(stat, OK, E_INVOPS);

                printf("creating room: %s\n", name);
                printf("  description: %s\n", description);
                printf("   conference: %d\n", area);
                printf("    networked: "); 

                if (networked) {

                    printf("true\n");

                } else {

                    printf("false\n");

                }

                printf("     resource: %s\n", resource);
                printf("         type: ");

                if (flags == RM_MESSAGES)  printf("message\n");
                if (flags == RM_DIRECTORY) printf("directory\n");
                if (flags == RM_BULLETIN)  printf("bulletin\n");
                if (flags == RM_SUBSYS)    printf("subsys\n");

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
    int timeout = TIMEOUT;
    char retries = RETRIES;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        stat = errors_load(errs, bbs_codes, sizeof(bbs_codes));
        check_return(stat, errs);

        dump = tracer_create(errs);
        check_creation(dump);

        room = room_create(DATAPATH, ROOMNUM, retries, timeout, base, dump);
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

