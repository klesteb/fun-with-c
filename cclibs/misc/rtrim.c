
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
#include <ctype.h>

/*----------------------------------------------------------------------*/

char *rtrim(char *string) {
/*
 * Function: rtrim.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *    Removes all the rightmost spaces from the string specified
 *    by "string".
 *
 * Modifications
 *
 * Variables
 */
 
	int x;
	
/*
 * Main part of function.
 */
    
	if (string != NULL) { 
		
		x = strlen(string) - 1;

		for (; x > 0; x--) {
		
			if (isspace(string[x])) {
			
				string[x] = '\0';
			
   			} else break;

   		}

	}
	
    return(string);
   
}

