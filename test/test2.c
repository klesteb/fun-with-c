
#include <stdio.h>
#include "when.h"

int test() {
    
    return ERR;
    
}

int main(int argc, char **argv) {

    int stat = OK;

    when_error;

       stat = test();
       check_status(stat, OK, 10);

    error_handler;

       printf("Error: %d, line: %d, file: %s, function: %s\n",
              _er_trace.errnum, _er_trace.lineno, _er_trace.filename,
              _er_trace.function);
       clear_error();

    end_when;

    return 0;

}

