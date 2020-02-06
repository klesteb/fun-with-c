
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-Prototypes-----------------------------------------------------------*/

extern char *left(const char *, int);
extern char *right(const char *, int);
extern char *strupr(char *);
extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

char *convert_right_overpunch(char *in_amount) {
/*
 * Function: convert_right_overpunch.c
 * Version : 1.0
 * Created : 29-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is used to convert right overpunch numbers as 
 *    created by Cobol programs.
 *
 *    ex.
 *        number = convert_right_overpunch("12345b")
 *
 *    Note: The string returned must be freed by free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *temp = NULL;
    char *number = NULL;
    char *buffer = NULL;
    char *last_digit = NULL;

    int digit;
    int in_len;

/*
 * Main part of function.
 */

    in_len = strlen(in_amount);

    temp = xmalloc(in_len + 1);
    strcpy(temp, in_amount);
    strupr(temp);                              /* Upper case the string. */

    number = left(temp, in_len - 1);           /* Chop off all but last digit. */
    last_digit = right(temp, in_len);          /* Chop off last digit.  */
    digit = *(last_digit);                     /* Retrieve the value.   */

    switch (digit) {
        case '{':             /* Non-negative, zero value.              */
        case '[':
        case '?':
        case '0':
            buffer = xmalloc(in_len + 1);
            sprintf(buffer, "%s%s", number, "0");
            break;

        case 'A':            /* Non-negative, non-zero value.           */
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
            digit = (digit - 16);
            buffer = xmalloc(in_len + 1);
            sprintf(buffer, "%s%c", number, digit);
            break;

        case '}':            /* Negative, zero value.                   */
        case ']':
        case ':':
        case '!':
            buffer = xmalloc(in_len + 2);
            sprintf(buffer, "-%s%s", number, "0");
            break;

        case 'J':            /* Negative, non-zero value.               */
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
            digit = (digit - 25);
            buffer = xmalloc(in_len + 2);
            sprintf(buffer, "-%s%c", number, digit);
            break;

        default:
            buffer = xmalloc(in_len + 1);
            strcpy(buffer, temp);

    }

    free(number);
    free(last_digit);

    return(buffer);

}

