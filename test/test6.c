
#include <stdio.h>
#include "when.h"
#include "logger.h"
#include "errors.h"

errors_t *errs = NULL;
logger_t *logs = NULL;

int setup(void) {

    int stat = OK;
    char *process = "test6";
    char *category = "root";
    char *facility = "system";

    when_error {

        logs = log_create(facility, category, process);
        check_creation(logs);

        errs = errors_create();
        check_creation(errs);

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

