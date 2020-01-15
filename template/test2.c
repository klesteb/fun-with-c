
#include <stdio.h>
#include <errno.h>

#include "template.h"

int main(int argc, char **argv) {

    error_trace_t trace;
    template_t *temp = NULL;

    if ((temp = template_create(NULL))) {

        object_set_error(OBJECT(temp), EAGAIN);
        object_get_error(OBJECT(temp), &trace);

        printf("errno:    %d\n", trace.errnum);
        printf("lineno:   %d\n", trace.lineno);
        printf("file:     %s\n", trace.filename);
        printf("function: %s\n", trace.function);

        template_destroy(temp);

    }

    return 0;

}

