
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

#include <pcre.h>
#include "pjl_priv.h"
#include "meo_util.h"

/*----------------------------------------------------------------------*/

int _pjl_get_response(
#if __STDC__
    PjlHandle handle, queue *list)
#else
    handle, list)

    PjlHandle handle;
    queue *list;
#endif
{
/*
 * Function: _pjl_get_response.c
 * Version : 1.0
 * Created : 09-Nov-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will read the response back from the lazer printer.
 * 
 *    The format of a response is as follows:
 * 
 *    \t<item>\r\n
 *    \t<item>\r\n
 *    \f\r\n
 *
 *    The \f indicates the end of the response. The LFN layer strips off
 *    the \r.
 * 
 * Modification History
 *
 * Variables Used
 */

    int a = 0;
    int len = 0;
    int stat = ERR;
    char *y = NULL;
    char *c = NULL;

/*
 * Main part of function.
 */

    for (;;) {

        stat = pjl_getline(handle, &y);
        switch (stat) {
            case 0:                         /* Normal read.         */
                len = strlen(y);
                if ((c = strchr(y, '\f')) != NULL) {
                    a = c - y;
                }
                if (a > 0) {
                    if (len > 1) {
                        char *buff = strndup(y, len);
                        que_push_tail(list, buff);
                    }
                    goto fini;
                } else {
                    char *buff = strndup(y, len);
                    que_push_tail(list, buff);
                }
                break;

            case EWOULDBLOCK:               /* Timeout condition.   */
                stat = 0;
                goto fini;
                break;

            default:                        /* Some error occured.  */
                goto fini;
                break;

        }

    }

    fini:
    return stat;

}

int _pjl_send_command(
#if __STDC__
    PjlHandle handle, char *command)
#else
    handle, command)

    PjlHandle handle;
    char *command;
#endif
{
/*
 * Function: _pjl_send_command.c
 * Version : 1.0
 * Created : 15-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Run a PJL command and wait for the echo back of the command.
 *
 * Modification History
 *
 * Variables Used
 */

    char *y = NULL;
    int stat = ERR;
    int len = strlen(command) - 2;

/*
 * Main part of function.
 */

    if ((stat = _pjl_put(handle, command)) != OK) {

        vperror("(pjl_send_command) Error sending the command.\n");
        goto fini;

    }

    while ((stat = pjl_getline(handle, &y))) {

        if (stat == EWOULDBLOCK) {

            stat = OK;
            break;

        }

        if (strnicmp(command, y, len) == 0) {

            stat = OK;
            break;

        }

    }

    fini:
    return stat;

}

int _pjl_put(

#if __STDC__
    PjlHandle handle, char *command)
#else
    handle, command)

    PjlHandle handle;
    char *command;
#endif

{
/*
 * Function: _pjl_put.c
 * Version : 1.0
 * Created : 22-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Send a command.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;

/*
 * Main part of function.
 */

    if ((stat = lfn_putline(handle->stream, handle->timeout, command)) != 0) {

        vperror("(pjl_put) Communications error.\n");

    }

    return stat;

}

int _pjl_write(

#if __STDC__
    PjlHandle handle, void *buffer, int size)
#else
    handle, buffer, size)

    PjlHandle handle;
    void *buffer;
    int size;
#endif

{
/*
 * Function: _pjl_write.c
 * Version : 1.0
 * Created : 24-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    int written = 0;

/*
 * Main part of function.
 */

    if ((stat = lfn_write(handle->stream, handle->timeout, size, buffer, &written)) == 0) {

        if (size == written) {

            stat = OK;

        }

    }

    return stat;

}

int _pjl_do_command(
#if __STDC__
    PjlHandle handle, char *command, queue *list)
#else
    handle, command, list)

    PjlHandle handle;
    char *command;
    queue *list;
#endif
{
/*
 * Function: _pjl_do_command.c
 * Version : 1.0
 * Created : 15-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Run a PJL command and collect the response.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;

/*
 * Main part of function.
 */

    if ((stat = _pjl_send_command(handle, command)) == OK) {

        stat = _pjl_get_response(handle, list);

    }

    return stat;

}

int _pjl_clear_list(
#if __STDC__
    queue *list)
#else
    list)

    queue *list;
#endif
{
/*
 * Function: _pjl_clear_list.c
 * Version : 1.0
 * Created : 15-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This clear a list.
  *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    void *item = NULL;

/*
 * Main part of function.
 */

    if (!que_empty(list)) {

        while ((item = que_pop_tail(list)) != NULL) {

            free(item);

        }

    }

    if (que_empty(list)) {

        stat = que_init(list);

    }

    return stat;

}

int _pjl_response_find(

#if __STDC__
    void *data, void *name)
#else
    data, name)

    void *data;
    void *name;
#endif

{
/*
 * Function: _pjl_response_find.c
 * Version : 1.0
 * Created : 25-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *
 *    Invocation:
 *
 *        status = _pjl_response_find(data, name);
 *
 *    where
 *
 *        <data>              - I
 *            The response to be compared.
 *
 *        <name>           - I
 *            The name to be compared too.
 *
 *        <status>            - O
 *            This function will return FALSE if there is a match.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = TRUE;
    PjlResponse *response = (PjlResponse *)data;

/*
 * Main part of function.
 */

    if (data != NULL) {

        if (strncmp(response->name, name, strlen(name)) == 0) {

            stat = FALSE;

        }

    }

    return stat;

}

int _pjl_option_find(

#if __STDC__
    void *option, void *name)
#else
    option, name)

    void *option;
    void *name;
#endif

{
/*
 * Function: _pjl_option_find.c
 * Version : 1.0
 * Created : 25-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *
 *    Invocation:
 *
 *        status = _pjl_option_find(option, name);
 *
 *    where
 *
 *        <option>             - I
 *            The option to be compared.
 *
 *        <name>              - I
 *            The name to be compared too.
 *
 *        <status>            - O
 *            This function will return FALSE if there is a match.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat = TRUE;

/*
 * Main part of function.
 */

    if (name != NULL) {

        if (strncmp(option, name, strlen(name)) == 0) {

            stat = FALSE;

        }

    }

    return stat;

}

int _pjl_clear_response(
#if __STDC__
    queue *responses)
#else
    list)

    queue *responses;
#endif
{
/*
 * Function: _pjl_clear_response.c
 * Version : 1.0
 * Created : 21-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This clear a list.
  *
 * Modification History
 *
 * Variables Used
 */

    int stat = ERR;
    PjlResponse *response = NULL;

/*
 * Main part of function.
 */

    while ((response = que_pop_head(responses))) {

        if (response->name != NULL) free(response->name);
        if (response->value != NULL) free(response->value);
        if (response->items != NULL) free(response->items);
        if (response->type != NULL) free(response->type);

        _pjl_clear_list(&response->options);

    }

    if (que_empty(responses)) {

        stat = que_init(responses);

    }

    return stat;

}

int _pjl_parse_ustatus(

#if __STDC__
    PjlHandle handle, PjlResponse *response, char *line)
#else
    handle, response, line)

    PjlHandle handle;
    PjlResponse *response;
    char *line;
#endif

{
/*
 * Function: _pjl_parse_ustatus.c
 * Version : 1.0
 * Created : 20-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Parse the ustatus detail line.
 *
 * Modification History
 *
 * Variables Used
 */

    int rc = 0;
    int stat = ERR;
    char name[32];
    char value[32];
    char items[32];
    char type[32];
    int count = 0;
    int ovector[30];

/*
 * Main part of function.
 */

    if ((count = pcre_exec(handle->rustatus, NULL, line, strlen(line), 0, 0, ovector, 30)) < 0) {

        vperror("(pjl_parse_ustatus) Unable to parse buffer, error: %d\n", count);
        goto fini;

    }

    if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) { 

        vperror("(pjl_parse_ustatus) Unable to retrieve name, error: %d\n", rc);
        goto fini;

    }

    if ((rc = pcre_copy_substring(line, ovector, count, 2, value, 31)) < 0) { 

        vperror("(pjl_parse_ustatus) Unable to retrieve value, error: %d\n", rc);
        goto fini;

    }

    if ((rc = pcre_copy_substring(line, ovector, count, 3, items, 31)) < 0) { 

        vperror("(pjl_parse_ustatus) Unable to retrieve items, error: %d\n", rc);
        goto fini;

    }

    if ((rc = pcre_copy_substring(line, ovector, count, 4, type, 31)) < 0){ 

        vperror("(pjl_parse_ustatus) Unable to retrieve type, error: %d\n", rc);
        goto fini;

    }

    stat = OK;
    response->name = strdup(name);
    response->value = strdup(value);
    response->items = strdup(items);
    response->type = strdup(type);

    fini:
    return stat;

}

int _pjl_parse_variables(

#if __STDC__
    PjlHandle handle, PjlResponse *response, char *line)
#else
    handle, response, line)

    PjlHandle handle;
    PjlResponse *response;
    char *line;
#endif

{
/*
 * Function: _pjl_parse_variables.c
 * Version : 1.0
 * Created : 20-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Parse the variable detail line.
 *
 * Modification History
 *
 * Variables Used
 */

    int rc = 0;
    int stat = ERR;
    char name[32];
    char value[32];
    char items[32];
    char type[32];
    int count = 0;
    int ovector[30];

/*
 * Main part of function.
 */

    if ((count = pcre_exec(handle->rvariable1, NULL, line, strlen(line), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) { 

            vperror("(pjl_parse_variable) Unable to retrieve name, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 2, value, 31)) < 0) { 

            vperror("(pjl_parse_variable) Unable to retrieve value, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 3, items, 31)) < 0) { 

            vperror("(pjl_parse_variable) Unable to retrieve items, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 4, type, 31)) < 0){ 

            vperror("(pjl_parse_variable) Unable to retrieve type, error: %d\n", rc);
            goto fini;

        }

        stat = OK;
        response->name = strdup(name);
        response->value = strdup(value);
        response->items = strdup(items);
        response->type = strdup(type);
        goto fini;

    }

    if ((count = pcre_exec(handle->rvariable2, NULL, line, strlen(line), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) { 

            vperror("(pjl_parse_variable) Unable to retrieve name, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 2, items, 31)) < 0) { 

            vperror("(pjl_parse_variable) Unable to retrieve items, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 3, type, 31)) < 0){ 

            vperror("(pjl_parse_variable) Unable to retrieve type, error: %d\n", rc);
            goto fini;

        }

        stat = OK;
        response->name = strdup(name);
        response->value = strdup("NULL");
        response->items = strdup(items);
        response->type = strdup(type);

    }

    fini:
    return stat;

}

int _pjl_parse_config(

#if __STDC__
    PjlHandle handle, PjlResponse *response, char *line)
#else
    handle, *response, line)

    PjlHandle handle;
    PjlResponse *response;
    char *line;
#endif

{
/*
 * Function: _pjl_parse_config.c
 * Version : 1.0
 * Created : 20-Feb-2020
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Parse the config detail line.
 *
 * Modification History
 *
 * Variables Used
 */

    int rc = 0;
    int stat = ERR;
    char name[32];
    char value[32];
    char items[32];
    char type[32];
    int count = 0;
    int ovector[30];

/*
 * Main part of function.
 */

    if ((count = pcre_exec(handle->rconfig1, NULL, line, strlen(line), 0, 0, ovector, 30)) > 0) {
        
        if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve name, error: %d\n", rc);
            goto fini;

        }

        stat = OK;
        response->name = strdup(name);
        response->value = NULL;
        response->items = NULL;
        response->type = NULL;
        goto fini;

    }

    if ((count = pcre_exec(handle->rconfig2, NULL, line, strlen(line), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve name, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 2, value, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve value, error: %d\n", rc);
            goto fini;

        }

        stat = OK;
        response->name = strdup(name);
        response->value = strdup(value);
        response->type = NULL;
        response->items = NULL;
        goto fini;

    }

    if ((count = pcre_exec(handle->rconfig3, NULL, line, strlen(line), 0, 0, ovector, 30)) > 0) {

        if ((rc = pcre_copy_substring(line, ovector, count, 1, name, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve name, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 2, items, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve items, error: %d\n", rc);
            goto fini;

        }

        if ((rc = pcre_copy_substring(line, ovector, count, 3, type, 31)) < 0) {

            vperror("(pjl_parse_config) Unable to retrieve type, error: %d\n", rc);
            goto fini;

        }

        stat = OK;
        response->name = strdup(name);
        response->items = strdup(items);
        response->type = strdup(type);
        response->value = NULL;

    }

    fini:
    return stat;

}

