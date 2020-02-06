
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1992 by Kevin L. Esteb                                     */
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

void insert(char *string, char *buf, int start, int length) {
/*
 * Function: insert.c
 * Version : v1.0
 * Created : 23-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synposis
 *
 *    Insert "buf" into "string" starting at "start" for "length".
 *
 * Modifications
 *
 * Variables
 */
 
   int x;
   
/*
 * Main part of function.
 */
   
   if (start < 0) return;  /* invalid number                            */
   
   /* if an attempt is made to copy beyond end of string then adjust    */
   
   if ((start + length + 1) > strlen(string)) length = strlen(string) - start;

   for (x = 0; x < length; x++)
      string[start + x] = buf[x];
      
}

