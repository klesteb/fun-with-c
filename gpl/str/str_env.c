
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "str_priv.h"

/*----------------------------------------------------------------------*/

void  str_env (

#    if __STDC__
        const  char  *string,
        int  length,
        char  *translation,
        size_t  maxLength)
#    else
        string, length, translation, maxLength)

        char  *string ;
        int  length ;
        char  *translation ;
        size_t  maxLength ;
#    endif

{
/*
 * Function: str_env.c
 * Version : 1.0
 * Created : 02-Oct-1997
 * Author  : Alex Measday
 *
 * Description
 *
 *    Function str_env() translates environment variables ("$<name>") and
 *    home directory references ("~") embedded in a string.  For example,
 *    if variable DG has been defined as "/usr/alex/dispgen", str_env()
 *    will translate
 *
 *        "tpocc:$DG/page.tdl"
 *
 *    as
 *
 *        "tpocc:/usr/alex/dispgen/page.tdl".
 *
 *    Remember that C-Shell variables (defined using "set name = value") are
 *    NOT environment variables (defined using "setenv name value") and are
 *    NOT available to programs.  Define any variables you might need as
 *    environment variables.
 *
 *    Environment variables can be nested, i.e., defined in terms of each other.
 *    Undefined environment variables are not an error and are assumed to have
 *    a value of "" (a zero-length string).
 *
 *    Invocation:
 *
 *        str_env(string, length, &translation, maxLength);
 *
 *    where
 *
 *        <string>            - I
 *            Is the string which contains environment variable references.
 *
 *        <length>            - I
 *            Is the length of the string.  If LENGTH is less than zero,
 *            str_env() determines the length by scanning STRING for a
 *            terminating null character.
 *
 *        <translation>       - O
 *            Is the address of a buffer which will receive the translated
 *            string.
 *
 *        <maxLength>         - I
 *            Is the maximum length of the translation; i.e., the size of
 *            the translation buffer.
 *
 * Modification History
 *
 * Variables Used
 */

    char  follow, *name, *s;
    int  i;

/*
 * Main part of function.
 */

    if (translation == NULL)  return;
    if (string == NULL) {

        strcpy(translation, "");
        return;

    }

    str_copy(string, length, translation, maxLength);

    /* Scan through the string, replacing "~"s by the user's home       */
    /* directory and environment variables ("$<name>") by their values. */

    for (i = 0; translation[i] != '\0';  ) {

        if (translation[i] == '~') {            /* "~"                  */

            s = getenv("HOME");                 /* Get home directory.  */
            if (s == NULL) {

                i++ ;                           /* Insert "~".          */

            } else {

                str_remove(1, i, translation);  /* Insert home directory. */
#ifdef VMS
				 {
					
    				char unixpath[1024];

                	/* VMS returns the home directory in VMS syntax. */

                	fnm_vms2unix(s, unixpath, sizeof(unixpath) -1);
                	str_insert(unixpath, -1, i, translation, maxLength);

				 }
#else
                str_insert(s, -1, i, translation, maxLength);
#endif
            }

        } else if (translation[i] == '$') {     /* "$<name>"            */

            length = 0;                         /* Extract "<name>".    */
            name = (char *)get_word(&translation[i], "$./", &length);
            follow = name[length];
            name[length] = '\0';
            s = getenv(name);                   /* Lookup "<name>".     */
            name[length] = follow;

            /* Replace "$<name>" ...                                    */

            str_remove(name - &translation[i] + length, i, translation);

            /* ... by "<value>".                                        */

            if (s != NULL) str_insert(s, -1, i, translation, maxLength);

        } else {                                /* Normal character.    */

            i++;

        }

    }

    return;

}

