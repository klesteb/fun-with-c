
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

/*----------------------------------------------------------------------*/

void win_prompt(char *text, long bell, long timeout, long keyboard_id, 
                long *aborted) {
/*
 * Function: win_prompt.c
 * Version : 1.0
 * Created : 28-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will print a prompt in the message area on the screen 
 *    and then wait "timeout" seconds for a keystroke. When this happens 
 *    the display is cleared. 
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    word term;
    win_r_event event;

    struct dsc$descriptor text_dsc;

/*
 * Main part of function.
 */

    /* Print out the prompt line.                                       */

    SET_SDESC(text_dsc, text, strlen(text));

    stat = smg$put_line(&init.msg_display_id, &text_dsc);
    vms_check_status(stat);

    /* Ring the terminal bell.                                          */

    if (bell) {

        stat = smg$ring_bell(&init.msg_display_id);
        vms_check_status(stat);

    }

    /* Wait for a keystroke or timeout.                                 */

    stat = smg$read_keystroke(&keyboard_id, &term, 0, &timeout);
    if ((stat != SS$_NORMAL) &&
        (stat != SS$_TIMEOUT) &&
        (stat != SMG$_EOF)) {
        vms_check_status(stat);
    }

    /* Process the keystroke.                                           */

    win_process_terminator(term, &event);
    switch (event.code) {
        case win_k_exit:
        case win_k_gold_exit:
            *aborted = TRUE;
            break;

        default:

    }

    /* Erase the message window.                                        */

    stat = smg$erase_display(&init.msg_display_id);
    vms_check_status(stat);

}

