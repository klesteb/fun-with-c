
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

#include "errors.h"
#include "tracer.h"
#include "drs_util.h"

tracer_t *dump;
errors_t *errs;

char path[256];

int process(void) {

    
}

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;

    when_error_in {

        errs = errors_create();
        check_creation(errs);

        dump = tracer_create(errs);
        check_creation(dump);


        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

void cleanup(void) {

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

    memset(path, '\0', 256);
    strcpy(path, ".");

    while ((ch = getopt(argc, argv, options)) != EOF) {

        switch (ch) {
            case 'd':
                memset(path, '\0', 256);
                strncpy(path, optarg, 255);
                break;

            case 'H':
            case 'h':
            case '?':
                printf("\n");
                printf("Usage: lcname [-d <directory>]\n");
                printf("  -d - the directory to process.\n");
                printf("\n");

                return EXIT_SUCCESS;

        }

    }

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = process();
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

