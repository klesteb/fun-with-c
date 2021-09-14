
#include <stdio.h>
#include <errno.h>

#include "files.h"

int main(int argc, char **argv) {

    error_trace_t trace;
    files_t *temp = NULL;
    int retries = 10;
    int timeout = 30;
    char *filename = "files.pod";

    if ((temp = files_create(filename, retries, timeout))) {

        object_set_error1(OBJECT(temp), EAGAIN);
        object_get_error(OBJECT(temp), &trace);

        printf("errno:    %d\n", trace.errnum);
        printf("lineno:   %d\n", trace.lineno);
        printf("file:     %s\n", trace.filename);
        printf("function: %s\n", trace.function);

        files_destroy(temp);

    }

    return 0;

}

