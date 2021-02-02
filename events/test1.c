
#include <stdio.h>
#include "event.h"

int main(int argc, char **argv) {

    event_t *temp = NULL;

    if ((temp = event_create())) {

        printf("version = %s\n", event_version(temp));
        event_destroy(temp);

    }

    return 0;

}

