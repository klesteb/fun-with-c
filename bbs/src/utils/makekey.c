
/*-----------------------------------------------------------------------*/
/*                                                                       */
/*                 Copyright (c) 1993 by Kevin L. Esteb                  */
/*                        All rights reserved                            */
/*                                                                       */
/* This file contains copyrighted material produced by Kevin L. Esteb.   */
/*                                                                       */
/* Distribution of the source code contained in this file, in any        */
/* form, modified or unmodified without PRIOR, WRITTEN APPROVAL by the   */
/* author is expressly prohibited.                                       */
/*                                                                       */
/*    The author can be reached at PO Box 5321, Everett, Wa. 98206       */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "misc/misc.h"

/*-----------------------------------------------------------------------*/

int main(int argc, char **argv) {
/*
 * Program: makekey.c
 * Version: 1.0
 * Created: 24-Jun-1994
 * Author : Kevin Esteb
 *
 * Description:
 *
 *    This program will create keys for the bbs program.
 *
 * Modifications:
 *
 * Varaiables:
 */

    int x;
    unsigned long junk;
    char buffer[BUFSIZ];

/*
 * Main part of functon.
 */

    if (argc < 2) {

        printf("Usage: makekey <name>\n");
        return 0;

    }

    strcpy(buffer, argv[1]);

    for (x = 2; x < argc; x++) {

        strcat(buffer, " ");
        strcat(buffer, argv[x]);

    }

    junk = elf_hash((const unsigned char *)buffer);

    printf("Key value = %ld\n", junk);

    return 0;

}

