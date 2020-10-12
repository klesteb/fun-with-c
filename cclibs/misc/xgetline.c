
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <unistd.h>

int xgetline(int fd, char *buffer, int len, int delim) {

    int a = 0;
    int b = 0;
    int c = 0;

    do {

        a = read(fd, &b, 1);

        if ((b == delim) || (a < 1)) b = 0;

        buffer[c] = b;
        ++c;
        if (c >= len) b = 0;

    } while (b != 0);

    return(c - 1);

}

