
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

/*----------------------------------------------------------------------*/

extern char *rtrim(char *);
extern char *ltrim(char *);

/*----------------------------------------------------------------------*/

char *trim(char *string) {
/*
 * Function: trim.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *    Removes all the leftmost and rightmost spaces from the string 
 *    specified by "string".
 *
 * Modifications
 *
 * Variables
 */
 
/*
 * Main part of function.
 */

   ltrim(string);
   rtrim(string);
   
   return(string);
      
}

