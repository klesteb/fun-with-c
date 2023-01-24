
#include "logger.h"

int main(int argc, char **argv) {

    char *process = argv[0];
    char *category = "test6";
    char *facility = "system";
    logger_t *log = log_create(facility, category, process);

    if (log != NULL) {

        log_set_category(log, category);

        log_info(log, "info works");
        log_warn(log, "warn works");
        log_error(log, "error works");
        log_fatal(log, "fatal works");
        log_debug(log, "debug works");
        log_trace(log, "trace works");

    }

    log_destroy(log);

    return 0;

}

