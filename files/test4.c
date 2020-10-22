
#include <stdio.h>
#include "files.h"

int main(int argc, char **argv) {

    int x = 0;
    int stat = OK;
    ssize_t count;
    int retries = 10;
    int timeout = 30;
    files_t *temp = NULL;
    char *filename = "junk.txt";
    char *text = "this is a text line";
    int mode = S_IRWXU | S_IRWXG | S_IROTH;
    int flags = O_CREAT | O_WRONLY | O_TRUNC;

    if ((temp = files_create(filename, retries, timeout))) {

        if (files_open(temp, flags, mode) == OK) {

            for (x = 0; x < 10; x++) {

                stat = files_puts(temp, text, strlen(text), &count);
                if (stat != OK) break;

            }

            files_close(temp);

        } else {

            printf("unable to open %s\n", filename);

        }

        files_destroy(temp);

    }

    return 0;

}

