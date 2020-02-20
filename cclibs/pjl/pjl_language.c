
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2004 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "pjl_priv.h"

/*----------------------------------------------------------------------*/

int pjl_language(
#if __STDC__
    PjlHandle handle, char *language)
#else
    handle, language)

    PjlHandle handle;
    char *language;
#endif
{
/*
 * Function: pjl_language.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the default printer control language. It will
 *    check the printer configuration to see if the desired language is
 *    supported. If not a -1 is returned.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = 0;
    char *buff = NULL;
    char *item = "LANGUAGE";
    char *wanted = "LANGUAGES";

/*
 * Main part of function.
 */

    for (buff = que_first(&handle->configs);
         buff != NULL;
         buff = que_next(&handle->configs)) {

        if (strnicmp(buff, wanted, strlen(wanted)) == 0) {

            for (;
                 buff != NULL;
                 buff = que_next(&handle->configs)) {

                if (buff[0] == '\t') {

                    if (stricmp(language, buff) == 0) {

                        stat = lfn_putline(handle->stream, handle->timeout,
                                           "@PJL ENTER LANGUAGE = %s \r\n",
                                           language);
                        goto fini;

                    }

                } else {

                    stat = -1;
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

