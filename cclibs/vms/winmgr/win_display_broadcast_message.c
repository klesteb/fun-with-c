
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"
#include "deque.h"

/*-Prototypes-----------------------------------------------------------*/

static int get_broadcasts(queue *);
extern int trap_message(long, long, long, long, long);

/*----------------------------------------------------------------------*/

void win_display_broadcast_message(void) {
/*
 * Function: win_display_broadcast_message.c
 * Version : 1.0
 * Created : 23-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function will check to see if there is any pending system 
 *    broadcast messages. If there are some it will then open a window 
 *    on the screen and display all the messages within the window. 
 *    When done the window will be closed and continue on until the 
 *    next time. The user can abort out of the queued broadcast messages 
 *    by pressing PF4. This will not work for repeated typing of ^T.
 *
 * Modification History
 *
 * Variables Used
 */

    char *prompt_string = "Press <return> for next message or <PF4> to discard the rest.";

    long x;
    long row;
    long col;
    long stat;
    long done;
    long wrap;
    long flags;
    long lines;
    long aborted;
    long msg_count;
    long keyboard_id;

    char *msg_str = NULL;
    queue msg_lst;
    win_r_window win;

    struct dsc$descriptor msg_dsc;

/*
 * Main part of function.
 */

    SET_WIN(win, init.rows, (((init.cols - 78) / 2) + 1), 3, 78, "Broadcast Message");

    /* Turn AST trapping of broadcast messages off. We need to do       */
    /* this so we don't interrupt ourselves when getting the messages.  */

    stat = smg$set_broadcast_trapping(&init.pasteboard_id, 0, 0);
    vms_check_status(stat);

    /* Get the broadcast messages that are waiting.                     */

    if (get_broadcasts(&msg_lst)) {
        
        /* Create a virtual keyboard, necessary for prompts from AST's. */

        stat = smg$create_virtual_keyboard(&keyboard_id);
        vms_check_status(stat);

        /* Create the broadcast messages window.                        */

        win_open_window(&win);

        /* Display the waiting messages.                                */

        for (msg_str = que_first(&msg_lst);
             msg_str != NULL;
             msg_str = que_next(&msg_lst)) {

            SET_SDESC(msg_dsc, msg_str, sizeof(msg_str) - 1);
            
            flags = 1;
            lines = 1;
            wrap = SMG$M_WRAP_WORD;
            stat = smg$put_line(&win.display_id, &msg_dsc, &lines, 0,
                                &flags, &wrap, 0,0);
            vms_check_status(stat);

            win_prompt(prompt_string, FALSE, 5, keyboard_id, &aborted);
            if (aborted) break;

            stat = smg$erase_display(&win.display_id);
            vms_check_status(stat);

        }

        /* Deallocate the message list.                                 */

        for (msg_str = que_pop_head(&msg_lst);
             msg_str != NULL;
             msg_str = que_pop_head(&msg_lst)) {

            free(msg_str);

        }

        /* Close the broadcast message window.                          */

        win_close_window(&win);

        /* Delete the virtual keyboard.                                 */

        stat = smg$delete_virtual_keyboard(&keyboard_id);
        vms_check_status(stat);

    }

    /* Turn AST trapping of broadcast messages back on.                 */

    stat = smg$set_broadcast_trapping(&init.pasteboard_id, &trap_message, 0);
    vms_check_status(stat);

}

/**/

static int get_broadcasts(queue *msg_lst) {
/*
 * Function: get_broadcasts.c
 * Version : 1.0
 * Created : 23-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will retrieve the broadcast messages and put them
 *    into a linked list for display purposes.
 *
 * Modification History
 *
 * Variables Used
 */

    int stuff = FALSE;
    long stat;

    word msg_len;
    char msg_str[256];
    char *holder = NULL;

    struct dsc$descriptor msg_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(msg_dsc, msg_str, sizeof(msg_str) - 1);

    stat = smg$get_broadcast_message(&init.pasteboard_id, &msg_dsc, &msg_len, 0);
    vms_check_status(stat);

    while (stat != SMG$_NO_MORMSG) {

        stuff = TRUE;

        msg_str[msg_len] = '\0';
        holder = strdup(msg_str);
        que_push_tail(msg_lst, holder);

        stat = smg$get_broadcast_message(&init.pasteboard_id, &msg_dsc, &msg_len, 0);
        vms_check_status(stat);

    }

    return(stuff);

}

