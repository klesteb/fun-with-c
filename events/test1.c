
#include <stdio.h>
#include "events.h"

int main(int argc, char **argv) {

    events_t *temp = NULL;

    if ((temp = events_create(NULL))) {

        printf("version = %s\n", events_version(temp));
        events_destroy(temp);

    }

    return 0;

}

