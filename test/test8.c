
#include <stdio.h>
#include "when.h"
#include "logger.h"
#include "errors.h"
#include "message.h"

errors_t *errs = NULL;
logger_t *logs = NULL;
message_t *msgs = NULL;

int setup(error_trace_t *error) {

    int stat = OK;
    char *process = "test8";
    char *category = "test8";
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

        copy_error(error);
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

int test(error_trace_t *error) {

    int stat = OK;

    when_error {

        cause_error(E_INVOPS);
        exit_when;

    } use {

        copy_error(error);
        clear_error();
        stat = ERR;

    } end_when;

    return ERR;

}

int main(int argc, char **argv) {

    int stat = OK;
    error_trace_t error;

    when_error {

        stat = setup(&error);
        check_status(stat, OK, 10);

        stat = test(&error);
        check_status(stat, OK, 11);

        exit_when;

    } use {

        char text[32]; 
        char *fmt = "%s, line: %d, file: %s, function: %s";

        errors_get_text(errs, error.errnum, text, 31);
        log_error(logs, fmt, text, error.lineno, error.filename, error.function);

        errors_get_text(errs, error.errnum, text, 31);
        log_error(logs, fmt, text, trace_lineno, trace_filename, trace_function);

        clear_error();
        clear_copied(error);

    } end_when;

    cleanup();

    return 0;

}

