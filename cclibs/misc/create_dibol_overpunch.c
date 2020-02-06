
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
extern void *xmalloc(int);

/*----------------------------------------------------------------------*/

char *create_dibol_overpunch(char *in_amount) {
/*
 * Function: create_dibol_overpunch.c
 * Version : 1.0
 * Created : 29-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will create a dibol overpunch number. If the number is 
 *    negative value then the appropiate letter from 'p' thru 'y' will be
 *    used.
 *
 *    ex.
 *
 *        number = create_dibol_overpunch("-1234560");
 *
 *    Where number will equal "123456p".
 *
 *    Note: The string returned must be freed with free().
 *
 * Modification History
 *
 * Variables Used
 */

    char *sign = NULL;
    char *buffer = NULL;
    char *number = NULL;
    char *last_digit = NULL;

    int digit;
    int in_len;
    int minus;

/*
 * Main part of function.
 */

    in_len = strlen(in_amount);

    buffer = xmalloc(in_len + 1);

    /* Check for a minus sign.                                          */

    sign = left(in_amount, 1);
    minus = *(sign);
    if (minus == '-') {

        /* This number is negative, create the overpunch.               */

        number = seg(in_amount, 1, in_len - 1);
        last_digit = right(in_amount, in_len);
        digit = *(last_digit);
        digit = (digit + 64);
        sprintf(buffer, "%s%c", number, digit);

    } else strcpy(buffer, in_amount);

    free(sign);
    free(number);
    free(last_digit);

    return(buffer);

}

