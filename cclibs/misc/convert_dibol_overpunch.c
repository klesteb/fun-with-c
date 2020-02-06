
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

char *convert_dibol_overpunch(char *in_amount) {
/*
 * Function: convert_dibol_overpunch.c
 * Version : 1.0
 * Created : 29-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will translate a dibol overpunch character to the 
 *    appropiate number. If it is a letter from "p" thru "y" then a 
 *    minus sign is append to the front of the string.
 *
 *    ex.
 *
 *        number = convert_dibol_overpunch("123456p")
 *
 *    Note: The string returned must be freed by free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *number = NULL;
    char *buffer = NULL;
    char *last_digit = NULL;

    int digit;
    int in_len;

/*
 * Main part of function.
 */

    in_len = strlen(in_amount);

    number = left(in_amount, in_len - 1);      /* Chop off all but last digit. */
    last_digit = right(in_amount, in_len);     /* Chop off last digit.  */
    digit = *(last_digit);                     /* Retrieve the value.   */

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
            sprintf(buffer, "%s%c", number, digit);
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
            sprintf(buffer, "-%s%c", number, digit);
            break;
    } 

    free(number);
    free(last_digit);

    return(buffer);

}

