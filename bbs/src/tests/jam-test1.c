
#include <stdio.h>

#include "tracer/tracer.h"
#include "bbs/src/jam/jam.h"

tracer_t *dump;

int main(int argc, char **argv) {

    int base = 1;
    int timeout = 1;
    char *path = ".";
    char retries = 10;
    char *name = "001";
    jam_t *temp = NULL;

    if ((dump = tracer_create(NULL))) {

        if ((temp = jam_create(path, name, retries, timeout, base, dump))) {

            printf("version = %s\n", jam_version(temp));
            jam_destroy(temp);

        }

        tracer_destroy(dump);

    }

    return 0;

}

