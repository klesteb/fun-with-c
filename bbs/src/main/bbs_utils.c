
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

#include "bbs/src/main/bbs_common.h"
#include "bbs/src/main/bbs_protos.h"

/*---------------------------------------------------------------------------*/

int bbs_load_file(char *filename, queue *lines, error_trace_t *errors) {

    int stat = OK;
    ssize_t count;
    int retries = 10;
    int timeout = 30;
    char buffer[1025];
    files_t *temp = NULL;
    int flags = O_RDONLY;

    when_error_in {

        temp = files_create(filename, retries, timeout);
        check_creation(temp);

        stat = files_open(temp, flags, 0);
        check_return(stat, temp);

        memset(buffer, '\0', 1025);
        stat = files_gets(temp, buffer, 1024, &count);
        check_return(stat, temp);

        while ((count > 0)) {

            stat = que_push_tail(lines, strdup(buffer));
            check_status(stat, QUE_OK, E_NOQUEUE);

            memset(buffer, '\0', 1025);
            stat = files_gets(temp, buffer, 1024, &count);
            check_return(stat, temp);

        };

        stat = files_close(temp);
        check_return(stat, temp);

        stat = files_destroy(temp);
        check_return(stat, temp);

        if (que_size(lines) < 1) {

            stat = que_push_head(lines, strdup("No help available\n"));
            check_status(stat, QUE_OK, E_INVOPS);

        }

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

char *str_time_hm(time_t date) {

    static char ttime[9];
    struct tm *l_d;

    l_d = localtime(&date);
    snprintf(ttime, 9, "%02d:%02d", l_d->tm_hour, l_d->tm_min);

    return ttime;

}

char *str_time_hms(time_t date) {

    static char ttime[9];
    struct tm *l_d;

    l_d = localtime(&date);
    snprintf(ttime, 9, "%02d:%02d:%02d", l_d->tm_hour, l_d->tm_min, l_d->tm_sec);

    return ttime;

}

char *gdate(time_t tt, int Y2K) {

    static char glc[15];
    struct tm   *tm;

    tm = localtime(&tt);
    if (Y2K) {

        snprintf(glc, 15, "%02d-%02d-%04d", tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900);

    } else {

        snprintf(glc, 15, "%02d-%02d-%02d", tm->tm_mon + 1, tm->tm_mday, tm->tm_year % 100);

    }

    return glc;

}

