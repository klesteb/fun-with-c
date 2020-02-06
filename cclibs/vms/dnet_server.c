
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

#include <iodef.h>
#include <descrip.h>
#include <dvidef.h>
#include <jpidef.h>
#include <msgdef.h>
#include <nfbdef.h>
#include <ssdef.h>
#include <starlet.h>
#include <lib$routines.h>
#include "cclibs.h"

/*-Prototypes-----------------------------------------------------------*/

int   ast_event(dnet_t *);
int   ast_read();
char *get_client_info(char *);
long  get_unit(word chan);

/*----------------------------------------------------------------------*/

dnet_t *dnet_init(char *taskname, int rbuf_size, int *accept, int *resource) {
/*
 * Function: dnet_init.c
 * Version : 1.0
 * Created : 18-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Initialize the data structure needed for a DECnet server process.
 *
 * Modification History
 *
 * Variables Used
 */

    dnet_t *srv = NULL;
	char mbx[17];

/*
 * Main part of function.
 */

	/* DECnet task names can only be 12 characters long.                */

	if (strlen(taskname) > 12) taskname[11] = '\0';
	sprintf(mbx, "%s_mbx", taskname);
	strupr(mbx);

    if ((srv = malloc(sizeof(dnet_t)) != NULL) {

        srv->taskname = strdup(taskname);
		srv->mbxname = strdup(mbx);
        srv->netdev = "_NET:";
        srv->rbuf_size = rbuf_size;
        que_init(&srv->alink);
		que_init(&srv->dlink);
        srv->accept = accept;
        srv->resource = resource;

    }

    return(srv);

}

/**/

void dnet_fini(dnet_t *srv) {
/*
 * Function: dnet_fini.c
 * Version : 1.0
 * Created : 18-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Deinialize the data structure used by the DECnet server.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    mbxbuf *buff = NULL;

/*
 * Main part of function.
 */

    if (srv != NULL) {

        free(srv->taskname);
		free(srv->mbxname);
        
        for (buff = que_pop_tail(&srv->alink);
             buff != NULL;
             buff = que_pop_tail(&srv->alink)) {

            free(buff);

        }

        for (buff = que_pop_tail(&srv->dlink);
             buff != NULL;
             buff = que_pop_tail(&srv->dlink)) {

            free(buff);

        }

    }

}
/**/

int dnet_server(dnet_t *srv) {
/*
 * Function: dnet_server.c
 * Version : 1.0
 * Created : 18-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will start a DECnet server process.
 *
 * Modification History
 *
 * Variables Used
 */

    int stat;
    iosb iosb;

    struct {
        byte obj_type;
        long obj_number;
    } nfb = {NFB$DECLNAME, 0};

    struct dsc$descriptor nfb_dsc;
    struct dsc$descriptor taskname_dsc;
    struct dsc$descriptor mbxname_dsc;
    struct dsc$descriptor netdev_dsc;

/*
 * Main part of function.
 */

	/* Create the descriptors.                                          */

    SET_SDESC(nfb_dsc, &nfb, sizeof(nfb));
    SET_SDESC(taskname_dsc, srv->taskname, strlen(srv->taskname));
    SET_SDESC(mbxname_dsc, srv->mbxname, strlen(srv->mbxname));
    SET_SDESC(netdev_dsc, srv->netdev, strlen(srv->netdev));

    /* Initalize a mailbox.                                             */

    stat = sys$crembx(0, &srv->mbx_chan, 256, srv->bufquo, 0,0, 
                      &mbxname_dsc);
    vms_check_status(stat);

    /* Assign a channel to _NET: and associate it with the mailbox.     */

    stat = sys$assign(&netdev_dsc, &srv->net_chan, 0, &mbxname_dsc);
    vms_check_status(stat);

    /* Declare ourselves as a DECnet server task.                       */

    stat = sys$qiow(0, srv->net_chan, IO$_ACPCONTROL, &iosb.quad, 0,0,
                    &nfb_dsc, &taskname_dsc, 0,0,0,0);
    vms_check_status(stat);
    vms_check_status(iosb.io.stat);  

    /* Queue a read request to the mailbox.                             */

    stat = sys$qio(0, srv->mbx_chan, IO$_READVBLK, &srv->iosb.quad,
                   &ast_event, srv,
                   &srv->mbx_buf, sizeof(srv->mbx_buf), 0,0,0,0);

    return(stat);

}    

/**/

int ast_event(dnet_t *srv) {
/*
 * Function: ast_event.c
 * Version : 1.0
 * Created : 18-May-1999
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Handle the DECnet events. This is done at AST level.
 *
 * Modification History
 *
 * Variables Used
 */

    dnet_link *link = NULL;
    struct dsc$descriptor ncb = {0,0,0,0};

/*
 * Main part of function.
 */

    ncb.dsc$w_length = srv->mbx_buf.buffer[1 + srv->buff.buffer[0]];
    ncb.dsc$a_pointer = srv->mbx_buf.buffer[2 + srv->buff.buffer[0]];

    switch (srv->mbx_buf.type) {
        case MSG$_CONFIRM:          /* Ignore these events.             */
        case MSG$_INTMSG:
        case MSG$_REJECT:
            break;

        case MSG$_NETSHUT:          /* A shutdown request was recieved. */
            srv->action = DNET_K_SHUTDOWN;
            break;

        case MSG$_ABORT:            /* These all pretty much mean the   */
        case MSG$_DISCON:           /* same thing, disconnect this link.*/
        case MSG$_EXIT:             /* Find the link that needs to get  */ 
        case MSG$_PATHLOST:         /* shut down.                       */
        case MSG$_PROTOCOL:
        case MSG$_THIRDPARTY:
        case MSG$_TIMEOUT:
             for (link = que_first(&srv->alinks);
                  link != NULL;
                  link = que_next(&srv->alinks)) {

				if (link.unit == srv->mbxbuf.unit) {

                	que_push_last(&srv->dlinks, link);
					que_del_cur(&srv->alinks);
                	srv->action = DNET_K_DISCONNECT;
                	break;

				}

            }
            break;

        case MSG$_CONNECT:
            if (srv->resource != NULL) resource = (*srv->resource)();
            if (srv->accept != NULL)   accept = (*srv->accept)(srv->buff);
            if ((resource != -1) && (accept != -1)) {


            } else {

                stat = sys$qiow(0, net_chan, IO$_ACCESS | IO$M_ABORT,
                        &iosb.quad, 0,0,0, &ncb, 0,0,0,0);
                vms_check_status(stat);
                vms_check_status(iosb.io.stat);

            }

        default:
            break;

    }

    /* Queue a read request to the mailbox.                             */

    stat = sys$qio(0, srv->mbx_chan, IO$_READVBLK, &srv->iosb.quad,
                   &ast_event, srv,
                   &srv->mbx_buf, sizeof(srv->mbx_buf), 0,0,0,0);

    return(stat);

}

