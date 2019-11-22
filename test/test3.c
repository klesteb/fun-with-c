
#include <stdio.h>
#include "when.h"

int test() {

    return ERR;

}

int main(int argc, char **argv) {

    int stat = OK;

    when_error {

        stat = test();
        check_status(stat, 10);

        exit_when;

    } error_handler {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    return 0;

}

