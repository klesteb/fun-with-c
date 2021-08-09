
#include <stdio.h>

#include "files/files.h"

int main(int argc, char **argv) {

    files_t *temp = NULL;
    int retries = 10;
    int timeout = 30;
    char *filename = "files.pod";
    
    if ((temp = files_create(filename, retries, timeout))) {

        printf("version: %s\n", files_version(temp));
        files_destroy(temp);

    }

    return 0;

}

