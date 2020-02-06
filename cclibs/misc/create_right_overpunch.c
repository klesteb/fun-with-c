
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
extern char *seg(const char *, int, int);
extern char *strupr(char *);
extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

char *create_right_overpunch(char *in_amount) {
/*
 * Function: create_right_overpunch.c
 * Version : 1.0
 * Created : 29-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is used to create right overpunch numbers as 
 *    used by Cobol programs.
 *
 *    ex.
 *        number = create_right_overpunch("12345")
 *
 *    Note: The string returned must be freed with free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *sign = NULL;
    char *temp = NULL;
    char *buffer = NULL;
    char *number = NULL;
    char *last_digit = NULL;

    int minus;
    int digit;
    int in_len;

/*
 * Main part of function.
 */

    in_len = strlen(in_amount);

    buffer = xmalloc(in_len + 1);

    /* Upper case the string.                                           */

    temp = xmalloc(in_len + 1);
    strcpy(temp, in_amount);
    strupr(temp);

    /* Check for a minus sign.                                          */

    sign = left(temp, 1);
    minus = *(sign);
    if (minus == '-') {

        /* This number is negative, create the overpunch.               */

        number = seg(temp, 1, in_len - 1);
        last_digit = right(temp, in_len);
        digit = *(last_digit);

        switch (digit) {
            case '0':
                sprintf(buffer, "%s%s", number, "}");
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                digit = (digit + 25);
                sprintf(buffer, "%s%c", number, digit);
                break;

            default:
                strcpy(buffer, temp);

        }

    } else {

        /* This number is non-negative, create the overpunch.           */

        number = left(temp, in_len - 1);
        last_digit = right(temp, in_len);
        digit = *(last_digit);

        switch (digit) {
            case '0':
                sprintf(buffer, "%s%s", number, "{");
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                digit = (digit + 16);
                sprintf(buffer, "%s%c", number, digit);
                break;

            default:
                strcpy(buffer, temp);

        }

    }

    free(temp);
    free(sign);
    free(number);
    free(last_digit);

    return(buffer);

}

