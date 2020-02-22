
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

int pjl_enter(

#if __STDC__
    PjlHandle handle, char *language)
#else
    handle, language)

    PjlHandle handle;
    char *language;
#endif

{
/*
 * Function: pjl_enter.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will set the default printer control language. It will
 *    check the printer configuration to see if the desired language is
 *    supported. If not ERR is returned.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char *option = NULL;
    char buffer[PJL_K_BUFSIZ];
    char *wanted = "LANGUAGES";
    PjlResponse *response = NULL;
    char *command = "@PJL ENTER LANGUAGE = %s \r\n";

/*
 * Main part of function.
 */

    for (response = que_first(&handle->configs);
         response != NULL;
         response = que_next(&handle->configs)) {

        if (strcmp(response->name, wanted) == 0) {

            for (option = que_first(&response->options);
                 option != NULL;
                 option = que_next(&response->options)) {

                if (strcmp(language, option) == 0) {

                    memset(buffer, '\0', PJL_K_BUFSIZ);
                    sprintf(buffer, command, language);
                    stat = _pjl_put(handle, buffer);
                    goto fini;

                }

            }

        }

    }

    fini:
    return(stat);

}

