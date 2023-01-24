
#include <stdio.h>
#include "tracer.h"

int main(int argc, char **argv) {

    tracer_t *tracer = NULL;

    if ((tracer = tracer_create(NULL))) {

        tracer_destroy(tracer);

    }

    return 0;

}

