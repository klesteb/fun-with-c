
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

/*----------------------------------------------------------------------*/

void win_keyboard_monitor(win_r_event *event, long timeout) {
/*
 * Function: win_keyboard_monitor.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This subprogram will process any keystrokes that are typed on the 
 *    keyboard. This will allow for the defaulting of certain tasks that 
 *    will be performed whenever that key is pressed. Everything else will 
 *    be passed back to the calling routine within the event structure.
 *
 * Modification History
 *
 * Variables Used
 */

    word term;
    long stat;
    long done = TRUE;

/*
 * Main part of function.
 */

    event->code = 0;
    event->term = 0;

    do {

        if (timeout == 0) timeout = 60;

        stat = smg$read_keystroke(&init.keyboard_id, &term, 0, &timeout);
        if ((stat != SMG$K_TRM_TIMEOUT) && 
            (stat != SMG$_EOF) &&
            (stat != SS$_CANCEL) && 
            (stat != SS$_ABORT) && 
            (stat != SS$_TIMEOUT)) {
            vms_check_status(stat);
        }

        win_process_terminator(term, event);
        if (event->code == win_k_gold) done = FALSE;

        stat = smg$cancel_input(&init.keyboard_id);
        vms_check_status(stat);

    } while (done == TRUE);

    win_display_broadcast_message();

}

