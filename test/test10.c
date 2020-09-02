
#include <stdio.h>
#include <log4c.h>

#include "when.h"
#include "tracer.h"
#include "logger.h"
#include "errors.h"
#include "message.h"

tracer_t *dump = NULL;
errors_t *errs = NULL;
logger_t *logs = NULL;
message_t *msgs = NULL;

int setup(void) {

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

        dump = tracer_create(errs);
        check_creation(dump);

        exit_when;

    } use {

        capture_trace(dump);
        clear_error();
        stat = ERR;

    } end_when;

    return stat;

}

int dump_trace(char *buffer) {

    log_error(logs, "%s", buffer);

    return OK;

}

void cleanup(void) {

    log_destroy(logs);
    errors_destroy(errs);
    message_destroy(msgs);
    tracer_destroy(dump);

}

int test(void) {

    int stat = OK;

    when_error {

        cause_error(E_INVOPS);
        exit_when;

    } use {

        capture_trace(dump);
        clear_error();
        stat = ERR;

    } end_when;

    return ERR;

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error {

        stat = setup();
        check_status(stat, OK, E_INVOPS);

        stat = test();
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        capture_trace(dump);
        tracer_dump(dump, dump_trace);
        clear_error();

    } end_when;

    cleanup();

    return 0;

}

