
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
extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

char *convert_zone_overpunch(char *in_amount) {
/*
 * Function: convert_zone_overpunch.c
 * Version : 1.0
 * Created : 29-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will translate a zone overpunch character to the 
 *    appropiate number. If it is a letter from "p" thru "y" then a 
 *    minus sign is append to the front of the string.
 *
 *    ex.
 *
 *        number = convert_zone_overpunch("p123456")
 *
 *    Note: The string returned must be freed by free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *number = NULL;
    char *first_digit = NULL;
    char *buffer = NULL;

    int digit;
    int in_len;

/*
 * Main part of function.
 */

    in_len = strlen(in_amount);

    first_digit = left(in_amount, 1);          /* Chop off the first digit. */
    number = right(in_amount, 2);              /* Chop off the rest.    */
    digit = *(first_digit);                    /* Retrieve the value.   */

    switch (digit) {
        case '0':            /* Non-negative value, digit is unchanged. */
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            buffer = xmalloc(in_len + 1);
            sprintf(buffer, "%c%s", digit, number);
            break;
    
        case 'p':            /* Negative value, substitute number for letter. */
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
            digit = (digit - 64);
            buffer = xmalloc(in_len + 2);
            sprintf(buffer, "-%c%s", digit, number);
            break;
    }

    free(number);
    free(first_digit);

    return(buffer);

}

