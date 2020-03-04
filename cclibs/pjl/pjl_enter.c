
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
 *    Invocation:
 *
 *        status = pjl_enter(handle, language);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle from pjl_create().
 *
 *        <language>          - I
 *            The print language to use.
 *
 *        <status>            - O
 *            This function will return either OK or ERR.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    char buffer[PJL_K_BUFSIZ];
    char *wanted = "LANGUAGES";
    PjlResponse *response = NULL;
    char *command = "@PJL ENTER LANGUAGE = %s \r\n";

/*
 * Main part of function.
 */

    if ((handle == NULL) || (language == NULL)) {

        vperror("(pjl_enter) Invalid parameters.\n");
        goto fini;

    }

    if (que_find(&handle->configs, wanted, _pjl_response_find) == QUE_OK) {

        response = que_get(&handle->configs);

        if (que_find(&response->options, language, _pjl_option_find) == QUE_OK) {

            memset(buffer, '\0', PJL_K_BUFSIZ);
            snprintf(buffer, PJL_K_BUFSIZ - 1, command, language);

            if ((stat = _pjl_put(handle, buffer)) != OK) {

                vperror("(pjl_enter) Unable to send ENTER command.\n");

            }

        }

    }

    fini:
    return stat;

}

