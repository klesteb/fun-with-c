
#include <stdio.h>
#include "files.h"

int main(int argc, char **argv) {

    files_t *temp = NULL;

    if ((temp = files_create(NULL))) {

        files_destroy(temp);

    }

    return 0;

}

