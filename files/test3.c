
#include <stdio.h>
#include "files.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    struct stat buf;
    char buffer[101];
    files_t *temp = NULL;
    int flags = O_RDONLY;
    char *filename = "files.pod";

    if ((temp = files_create(filename))) {

        if (files_open(temp, flags, 0) == OK) {

            files_stat(temp, &buf);
            printf("file size = %d\n", buf.st_size);

            do {

                stat = files_gets(temp, buffer, 100, &count);
                if (stat == OK) {

                    printf("%d: %s", count, buffer);

                }

            } while ((stat == OK) && (count > 0));

            files_close(temp);

        } else {

            printf("unable to open %s\n", filename);

        }

        files_destroy(temp);

    }

    return 0;

}

