
#include <stdio.h>
#include <errno.h>

#include "tracer/tracer.h"
#include "bbs/src/jam/jam.h"

tracer_t *dump;

int main(int argc, char **argv) {

    int base = 1;
    int timeout = 1;
    char *path = ".";
    char retries = 10;
    char *name = "001";
    jam_t *temp = NULL;
    error_trace_t trace;

    if ((dump = tracer_create(NULL))) {

        if ((temp = jam_create(path, name, retries, timeout, base, dump))) {

            object_set_error1(OBJECT(temp), EAGAIN);
            object_get_error(OBJECT(temp), &trace);

            printf("errno:    %d\n", trace.errnum);
            printf("lineno:   %d\n", trace.lineno);
            printf("file:     %s\n", trace.filename);
            printf("function: %s\n", trace.function);

            jam_destroy(temp);

        }

        tracer_destroy(dump);

    }

    return 0;

}

