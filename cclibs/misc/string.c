
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

char *string(int number, int ch) {
/*
 * Function: string.c
 * Version : v1.0
 * Created : 09-Sep-1998
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *  The buffer that is returned by this function needs to be freed with
 *  free(). The following text is from the DEC Basic reference manual.
 * 
 *  The STRING$ function creates a string containing a specified 
 *  number of identical characters. 
 *
 *  Format 
 *
 *   str-var = STRING$(int-exp1, int-exp2) 
 *
 *  Syntax Rules 
 *
 *   1.  Int-exp1 specifies the character string's length. 
 *
 *   2.  Int-exp2 is the decimal ASCII value of the character that 
 *       makes up the string. This value is treated modulo 256. 
 *
 *  Remarks 
 *
 *   1.  BASIC signals the error ``String too long'' (ERR=227) if 
 *       int-exp1 is greater than 65535. 
 *
 *   2.  If int-exp1 is less than or equal zero. 
 *
 *   3.  BASIC treats int-exp2 as an unsigned 8-bit integer. For 
 *       example, -1 is treated as 255. 
 *
 *   4.  If either int-exp1 or int-exp2 is a floating-point expres- 
 *       sion, BASIC truncates it to a LONG integer. 
 *
 * Modifications
 *
 * Variables
 */
 
    char *buff = NULL;
    
/*
 * Main part of function.
 */

    if (number < 0) number = 255;
    
    if (number) {
      
      if ((buff = xmalloc(number + 1)) == NULL) return(NULL);
      memset(buff, ch, number);

    }

    return(buff);
                  
}

