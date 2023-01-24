
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <string.h>

/*----------------------------------------------------------------------*/

extern char *xmalloc(int);

/*----------------------------------------------------------------------*/

char *right(const char *string, int number) {
/*
 * Function: right.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *  The buffer that is returned by this function needs to be freed with 
 *  free(). The following text is taken from the DEC Basic reference manual.
 *
 *  The RIGHT$ function extracts a substring from a string's 
 *  right side, leaving the string unchanged. 
 *
 *  Format 
 *
 *   str-var = RIGHT[$](str-exp, int-exp) 
 *
 *  Syntax Rules 
 *
 *   None 
 *
 *  Remarks 
 *
 *   1.  The RIGHT$ function extracts a substring from str-exp 
 *       and stores the substring in str-var. The substring begins 
 *       with the character in the position specified by int-exp   
 *       and ends with the rightmost character in the string. 
 *
 *   2.  If int-exp is less than or equal to zero, RIGHT$ returns 
 *       the entire string. 
 *
 *   3.  If int-exp is greater than the length of str-exp, RIGHT$ 
 *       returns a null string. 
 *
 *   4.  If you specify a floating-point expression for int-exp, 
 *       BASIC truncates it to a LONG integer. 
 *
 * Modifications
 *
 * Variables
 */
 
    char *buff = NULL;
    int length;
    
/*
 * Main part of function.
 */

    length = strlen(string);
    
    if (number < 1 || number > length) return(NULL);
   
    if ((buff = xmalloc(length + 1)) == NULL) return(NULL);

    strcpy(buff, &string[number - 1]);
    
    return(buff);
    
}

