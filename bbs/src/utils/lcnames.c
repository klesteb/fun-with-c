
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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "errors.h"
#include "tracer.h"
#include "fnm_util.h"

tracer_t *dump;
errors_t *errs;

int process(char *path) {

    int xstat = OK;
    DIR *dp = NULL;
    struct stat sb;
    char newname[256];
    char oldname[256];
    struct dirent *ep = NULL;

    when_error_in {

        memset(newname, '\0', 256);
        memset(oldname, '\0', 256);

        errno = 0;
        dp = opendir(path);
        if (dp == NULL) cause_error(errno);

        while ((ep = readdir(dp))) {

            if (strcmp(ep->d_name, ".") == 0) continue;
            if (strcmp(ep->d_name, "..") == 0) continue;

            strncpy(oldname, fnm_build(1, FnmPath, path, ep->d_name, NULL), 255);

            errno = 0;
            xstat = stat(oldname, &sb);
            check_status(xstat, OK, errno);

            if (S_ISREG(sb.st_mode)) {

                strncpy(newname, fnm_build(1, FnmPath, path, strlwr(ep->d_name), NULL), 255);

                errno = 0;
                xstat = rename(oldname, newname);
                check_status(xstat, OK, errno);

            }

        }

        closedir(dp);

        exit_when;

    } use {

        xstat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return xstat;

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
    char path[256];
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
                printf("Usage: lcnames [-d <directory>]\n");
                printf("  -d - the directory to process.\n");
                printf("\n");
                printf("The default action is to lower case names in the current directory.\n");
                printf("\n");

                return EXIT_SUCCESS;

        }

    }

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = process(path);
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

