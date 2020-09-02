
#include <stdio.h>
#include "trace.h"

int main(int argc, char **argv) {

    trace_t *trace = NULL;

    if ((trace = trace_create(NULL))) {

        trace_destroy(trace);

    }

    return 0;

}

