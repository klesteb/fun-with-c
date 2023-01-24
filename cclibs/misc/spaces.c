
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

char *spaces(int number) {
/*
 * Function: spaces.c
 * Version : v1.0
 * Created : 09-Sep-1998
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *  The buffer that is returned from this function needs to be freed with
 *  free(). The following text is taken from the DEC Basic reference manual.
 * 
 *  The SPACE$ function creates a string containing a specified 
 *  number of spaces. 
 *
 *  Format 
 *
 *   str-var = SPACE$(int-exp) 
 *
 *  Syntax Rules 
 *
 *   Int-exp specifies the number of spaces in the returned string. 
 *
 *  Remarks 
 *
 *   1.  BASIC treats an int-exp less than 0 as zero. 
 *
 *   2.  If you specify a floating-point expression for int-exp, 
 *       BASIC truncates it to a LONG integer. 
 *
 * Modifications
 *
 * Variables
 */
 
    char *buff = NULL;
    
/*
 * Main part of function.
 */

    if (number) {
      
      if ((buff = xmalloc(number + 1)) == NULL) return(NULL);
      memset(buff, ' ', number);

    }

    return(buff);
                  
}

