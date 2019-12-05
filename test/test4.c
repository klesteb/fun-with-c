
#include <stdio.h>
#include "when.h"
#include "logger.h"

int test() {

    return ERR;

}

int main(int argc, char **argv) {

    int stat = OK;
    logger_t *logs = NULL;
    char *process = argv[0];
    char *category = "root";
    char *facility = "system";

    when_error {

        if ((logs = log_create(facility, category, process)) == NULL) {

            retrieve_error(logs);

        }

        stat = test();
        check_status(stat, OK, 11);

        exit_when;

    } error_handler {

        log_fatal(logs, "Error: %d, line: %d, file: %s, function: %s\n",
              trace_errnum, trace_lineno, trace_filename, trace_function);

       clear_error();

    } end_when;

    return 0;

}

