
#include <stdio.h>
#include <log4c.h>

#include "include/when.h"
#include "logger/logger.h"
#include "errors/errors.h"
#include "messages/message.h"

#define process_error(error) { \
    copy_error(error);         \
    clear_error();             \
}

errors_t *errs = NULL;
logger_t *logs = NULL;
message_t *msgs = NULL;

int setup(error_trace_t *error) {

    int stat = OK;
    char *process = "test9";
    char *category = "test9";
    char *facility = "system";

    when_error_in {

        logs = log_create(facility, category, process);
        check_creation(logs);

        errs = errors_create();
        check_creation(errs);

        msgs = message_create(NULL, 0);
        check_creation(msgs);

        exit_when;

    } use {

        process_error(error);
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
    int retries = 0;

    when_error {

        AGAIN:
        cause_error(E_INVOPS);
        exit_when;

    } use {

        retries++;
        if (retries < 2) retry(AGAIN);

        process_error(error);
        stat = ERR;

    } end_when;

    return stat;

}

int main(int argc, char **argv) {

    int stat = OK;
    error_trace_t error;

    when_error {

        stat = setup(&error);
        check_status2(stat, OK, error);

        stat = test(&error);
        check_status2(stat, OK, error);

        exit_when;

    } use {

        char text[32]; 
        char reason[256];
        char *fmt = "%s, reason: %s; line: %d, file: %s, function: %s";

        errors_get_text(errs, trace_errnum, text, 31);
        errors_get_message(errs, trace_errnum, reason, 255);
        log_error(logs, fmt, text, reason, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    cleanup();

    return 0;

}

