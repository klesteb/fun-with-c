
#include <stdio.h>
#include <errno.h>

#include "include/when.h"
#include "errors/errors.h"
#include "tracer/tracer.h"
#include "cclibs/misc/misc.h"
#include "bbs/src/qwk/qwk.h"

qwk_t *qwk;
tracer_t *dump;
errors_t *errs;

int dump_trace(char *buffer) {

    fprintf(stderr, "%s\n", buffer);

    return OK;

}

int setup(void) {

    int stat = OK;
    int timeout = 1;
    char *path = ".";
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

int dump_message(qwk_header_t *header, char *text) {

    queue lines;
    struct tm *tm;
    int stat = OK;
    char *line = NULL;
    char output[19];
    char zone[10];

    when_error_in {

        memset(zone, '\0', 10);
        memset(output, '\0', 19);
        tm = localtime(&header->date_time);
        strftime(zone, 9, "%z", tm);
        sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d%4s",
                1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
                tm->tm_hour, tm->tm_min, tm->tm_sec, zone);

        stat = que_init(&lines);
        check_status(stat, QUE_OK, E_INVOPS);

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

        wordwrap(text, 60, &lines);

        while ((line = que_pop_head(&lines))) {

            printf("%s\n", line);
            free(line);

        }

        printf("\n");

        exit_when;

    } use {

        stat = ERR;
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int dump_control(qwk_control_t *control) {

    int stat = OK;
    struct tm *tm;
    char output[19];
    qwk_area_t *area = NULL;

    memset(output, '\0', 19);
    tm = localtime(&control->date_time);
    sprintf(output, "%04d-%02d-%02d %02d:%02d:%02d",
            1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);

    printf("\ncontrol.dat\n");
    printf("--------------------------------\n");
    printf("bbs name    : %s\n", control->bbs_name);
    printf("city        : %s\n", control->city);
    printf("state       : %s\n", control->state);
    printf("phone       : %s\n", control->phone);
    printf("sysop       : %s\n", control->sysop);
    printf("serial num  : %s\n", control->serial_num);
    printf("bbs id      : %s\n", control->bbs_id);
    printf("username    : %s\n", control->username);
    printf("hello file  : %s\n", control->hello_file);
    printf("news file   : %s\n", control->news_file);
    printf("goodbye file: %s\n", control->goodbye_file);
    printf("num areas   : %ld\n", control->num_areas);
    printf("date time   : %s\n", output);
    printf("areas\n");

    while ((area = que_pop_tail(&control->areas))) {

        printf("  number: %ld\n", area->area);
        printf("  name  : %s\n", area->name);

        free(area);

    }

    printf("\n");

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;
    qwk_ndx_t ndx;
    ssize_t count = 0;
    char *text = NULL;
    char *notice = NULL;
    qwk_header_t header;
    qwk_control_t control;

    when_error_in {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = qwk_open(qwk);
        check_return(stat, qwk);

        stat = qwk_get_control(qwk, &control);
        check_return(stat, qwk);

        stat = dump_control(&control);
        check_status(stat, OK, E_INVOPS);
        
        stat = qwk_get_notice(qwk, &notice);
        check_return(stat, qwk);

        printf("\nmessage.dat\n");
        printf("--------------------------------\n");
        printf("notice: %s\n", notice);
        free(notice);

        stat = qwk_open_ndx(qwk, "001");
        check_return(stat, qwk);

        stat = qwk_get_ndx(qwk, &ndx, &count);
        check_return(stat, qwk);

        while (count > 0) {

            stat = qwk_get_message(qwk, ndx.index, &header, &text);
            check_return(stat, qwk);
            
            stat = dump_message(&header, text);
            check_status(stat, OK, E_INVOPS);
            
            free(text);

            stat = qwk_get_ndx(qwk, &ndx, &count);
            check_return(stat, qwk);

        }

        stat = qwk_close(qwk);
        check_return(stat, qwk);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

