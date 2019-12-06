
#include <stdio.h>
#include "when.h"
#include "logger.h"
#include "errors.h"
#include "message.h"

errors_t *errs = NULL;
logger_t *logs = NULL;
message_t *msgs = NULL;

int setup(void) {

    int stat = OK;
    char *process = "test7";
    char *category = "root";
    char *facility = "system";

    when_error {

        logs = log_create(facility, category, process);
        check_creation(logs);

        errs = errors_create();
        check_creation(errs);

        msgs = message_create(NULL, 0);
        check_creation(msgs);

        exit_when;

    } use {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();
        stat = ERR;

    } end_when;

    return stat;

}

void cleanup(void) {

    log_destroy(logs);
    errors_destroy(errs);
    message_destroy(msgs);

}

int test(void) {

    return ERR;

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error {

        stat = setup();
        check_status(stat, OK, 10);

        stat = test();
        check_status(stat, OK, 11);

        exit_when;

    } use {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

       clear_error();

    } end_when;

    cleanup();

    return 0;

}

