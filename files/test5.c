
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "misc/misc.h"

int main(int argc, char **argv) {

    int fd;
    int size = 100;
    char buffer [101];
    char filename[256];

    if (argc < 2) {

        printf("Usage: test5 <filename>\n");
        return 0;

    }

    strcpy(filename, argv[1]);

    if ((fd = open(filename, O_RDONLY)) > 0) {

        while (xgetline(fd, buffer, size, '\n')) {

            printf("%s\n", buffer);

        }

        close(fd);

    } else {

        printf("unable to open file: %s\n", filename);

    }

    return 0;

}

