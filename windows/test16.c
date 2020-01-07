
#include <stdio.h>
#include <ncurses.h>

#include "when.h"
#include "workbench.h"

int main(int argc, char **argv) {

    int stat;
    workbench_t *wb = NULL;

    when_error {

        wb = workbench_create(NULL);
        check_creation(wb);

        printw("Press ^C to exit\n");
        refresh();

        stat = workbench_loop(wb);
        check_status(stat, OK, E_INVOPS);

        exit_when;

    } use {

        stat = ERR;
        printf("error: %d\n", trace_errnum);

    } end_when;

    return 0;

}

