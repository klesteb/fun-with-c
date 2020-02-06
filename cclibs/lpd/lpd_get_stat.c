
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 2000 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "lpd_priv.h"

/*----------------------------------------------------------------------*/

/**/

int lpd_get_stat(

#    if __STDC__
    LpdHandle handle, char *que, int type, item_list *items, queue *list)
#    else
    handle, que, type, items, list)

    LpdHandle handle;
    char *que;
    int type;
    item_list *items;
    queue *list;
#    endif

{
/*
 * Function: lpd_get_stat_long.c
 * Version : 1.0
 * Created : 22-Feb-2002
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Function lpd_get_stat() will issue a "send status long" or a
 *    "send status short" command, depending on the type.
 *
 *    If the user names or job numbers or both are supplied then only 
 *    those jobs for those users or with those numbers will be sent.
 *
 *    The response is an ASCII stream which describes the printer queue.
 *    The stream continues until the connection closes. Ends of lines are
 *    indicated with ASCII LF control characters. The lines may also
 *    contain ASCII HT control characters
 *
 *    Invocation:
 *
 *        status = lpd_get_stat_long(handle, que, type, items, &list);
 *
 *    where
 *
 *        <handle>            - I
 *            The handle returned from lpd_open().
 *
 *        <queue>             - I
 *            The print queue that information is wanted from.
 *
 *        <type>              - I
 *            The type of status listing (long, short).
 *
 *        <item>              - I
 *            The list of items (username, job number) that we are interested in.
 *
 *        <list>              - O
 *            A linked list of the responses from the server.
 *
 *        <status>            - O
 *            This function will return 0 or ERRNO.
 *
 * Modification History
 *
 * Variables Used
 */

    int x;
    int stat;
    char *data = NULL;
    char *message = NULL;

/*
 * Main part of function.
 */

    if (type == LPD_K_LONG) {

        stat = lfn_putline(handle->stream, handle->timeout, "\004%s ", que);

    } else stat = lfn_putline(handle->stream, handle->timeout, "\003%s ", que);
        
    if (stat == 0) {

        for (x = 0;; x++) {

            if ((items[x].code == 0) && (items[x].buf_len == 0)) break;

            switch (items[x].code) {
                case LPD_USER:
                case LPD_JOBID:
                    stat = lfn_putline(handle->stream, handle->timeout, "%s ", items[x].buffer);
                    break;

                default:
					break;
			   
            }
                    
            if (stat != 0) goto fini;

        }

        lfn_putline(handle->stream, handle->timeout, "\n");
        if (stat != 0) goto fini;

    }
        
    /* RFC-1179 says that the response is finished when the remote      */
    /* connection is dropped. The format of the response is determined  */
    /* by the LPD server.                                               */

    for (;;) {
    
        stat = lfn_getline(handle->stream, handle->timeout, &message);
        switch (stat) {
            case EPIPE:
                stat = 0;
                break;

            case -1:
                break;

            default:
                data = strdup(message);
                que_push_tail(list, data);
                break;

        }

    }

    fini:

    return(stat);

}

