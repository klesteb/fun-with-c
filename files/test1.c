
#include <stdio.h>
#include "files.h"

int main(int argc, char **argv) {

    files_t *temp = NULL;
    char *filename = "files.pod";
    
    if ((temp = files_create(filename))) {

        printf("version: %s\n", files_version(temp));
        files_destroy(temp);

    }

    return 0;

}

