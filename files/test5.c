
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "cclibs/misc/misc.h"

int main(int argc, char **argv) {

    int fd;
    int count = 0;
    int size = 100;
    char buffer [101];
    char filename[256];
    int working = TRUE;

    if (argc < 2) {

        printf("Usage: test5 <filename>\n");
        return 0;

    }

    strcpy(filename, argv[1]);

    if ((fd = open(filename, O_RDONLY))) {

        while (working) {

            count = xgetline(fd, buffer, size, '\n');
            if (count < 1) working = FALSE;

            printf("%s\n", buffer);

        }

        close(fd);

    } else {

        printf("unable to open file: %s\n", filename);

    }

    return 0;

}

