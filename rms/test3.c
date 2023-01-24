
#include <stdio.h>
#include "files.h"

int main(int argc, char **argv) {

    int stat = OK;
    ssize_t count;
    int exists = 0;
    int retries = 10;
    int timeout = 30;
    struct stat buf;
    char buffer[101];
    files_t *temp = NULL;
    int flags = O_RDONLY;
    char *filename = "files.pod";

    if ((temp = files_create(filename, retries, timeout))) {

        files_exists(temp, &exists);
        if (exists) {

            files_stat(temp, &buf);
            printf("file size = %d\n", buf.st_size);
            printf("file mode = %d\n", buf.st_mode);

            if (files_open(temp, flags, 0) == OK) {

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

        } else {

            printf("unable to find %s\n", filename);

        }

        files_destroy(temp);

    }

    return 0;

}

