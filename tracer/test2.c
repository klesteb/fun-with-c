
#include <stdio.h>
#include <errno.h>

#include "tracer.h"

int main(int argc, char **argv) {

    error_trace_t trace;
    tracer_t *temp = NULL;

    if ((temp = tracer_create(NULL))) {

        object_set_error1(OBJECT(temp), EAGAIN);
        object_get_error(OBJECT(temp), &trace);

        printf("errno:    %d\n", trace.errnum);
        printf("lineno:   %d\n", trace.lineno);
        printf("file:     %s\n", trace.filename);
        printf("function: %s\n", trace.function);

        tracer_destroy(temp);

    }

    return 0;

}

