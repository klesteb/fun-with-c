
#include <stdio.h>
#include <errno.h>

#include "trace.h"

int main(int argc, char **argv) {

    error_trace_t trace;
    trace_t *temp = NULL;

    if ((temp = trace_create(NULL))) {

        object_set_error1(OBJECT(temp), EAGAIN);
        object_get_error(OBJECT(temp), &trace);

        printf("errno:    %d\n", trace.errnum);
        printf("lineno:   %d\n", trace.lineno);
        printf("file:     %s\n", trace.filename);
        printf("function: %s\n", trace.function);

        trace_destroy(temp);

    }

    return 0;

}

