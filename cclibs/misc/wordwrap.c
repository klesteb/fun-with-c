
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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "cclibs/que_util.h"
#include "cclibs/misc/misc.h"

/*----------------------------------------------------------------*/

static int add_word(char *temp, char *word, int width, queue *results) {

    int stat = -1;

    if ((strlen(temp) + strlen(word) + 1) <= width) {

        strcat(temp, word);
        strcat(temp, " ");

        stat = 0;

    } else {

        char *line = strdup(temp);

        if ((stat = que_push_tail(results, line)) == QUE_OK) {

            memset(temp, '\0',  width + 1);
            strcat(temp, word);
            strcat(temp, " ");

            stat = 0;

        }

    }

    return stat;

}

int wordwrap(const char *buffer, int width, queue *results) {

    int stat = 0;
    char *word = NULL;
    char temp[width + 1];
    char *s = strdup(buffer);
    char *overflow = spaces(width);
    char delim[5] = {' ', '\b', '\t', '\v', '\f'};


    memset(temp, '\0', width + 1);

    for (word = xstrtok(s, delim);
         word != NULL;
         word = xstrtok(NULL, delim)) {

        if ((strchr(word, '\n') || strchr(word, '\r'))) {

            rtrim(word);

            stat = add_word(temp, word, width, results);
            if (stat != 0) goto fini;

            stat = add_word(temp, overflow, width, results);
            if (stat != 0) goto fini;

            trim(temp);

        } else {

            stat = add_word(temp, word, width, results);
            if (stat != 0) goto fini;

        }

    }

    stat = add_word(temp, overflow, width, results);
    trim(temp);

    fini:
    free(s);
    free(overflow);

    return stat;

}

