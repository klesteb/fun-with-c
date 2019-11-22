
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2019 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>

#include "que_util.h"

/*----------------------------------------------------------------*/

char **buffer_create(queue *lines) {

    int x = 0;
    int size = 0;
    char *line = NULL;
    char **buffer = NULL;

    if (lines != NULL) {

        size = que_size(lines);

        if ((buffer = (char **)calloc(1, size)) != NULL) {

            for (line = que_first(lines);
                 line != NULL;
                 line = que_next(lines)) {

                buffer[x] = sprintf("*.s\n", strlen(line), line);
                x++;

            }

        }

    }

    return buffer;

}

int buffer_destroy(char **buffer, int rows) {

    int x = 0;
    int stat = -1;

    if (buffer != NULL) {

        for (x = 0; x < rows; x++) {

            free(buffer[x]);

        }

        free(buffer);
        stat = 0;

    }

    return stat;

}

