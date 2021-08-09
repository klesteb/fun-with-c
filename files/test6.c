
#include <stdio.h>

#include "include/when.h"
#include "files/files.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    char buffer[101];
    int retries = 10;
    int timeout = 30;
    char filename[256];
    files_t *temp = NULL;
    int flags = O_RDONLY;

    if (argc < 2) {

        printf("Usage: test6 <filename>\n");
        return 0;

    }

    strcpy(filename, argv[1]);

    when_error_in {

        temp = files_create(filename, retries, timeout);
        check_creation(temp);

        stat = files_open(temp, flags, 0);
        check_return(stat, temp);

        do {

            stat = files_gets(temp, buffer, 100, &count);
            check_return(stat, temp);

            printf("%d: %s\n", count, buffer);

        } while (count > 0);

        stat = files_close(temp);
        check_return(stat, temp);

        stat = files_destroy(temp);
        check_return(stat, temp);

        exit_when;

    } use {

        printf("Error: %d, line: %d, file: %s, function: %s\n",
               trace_errnum, trace_lineno, trace_filename, trace_function);

        clear_error();

    } end_when;

    return 0;

}

