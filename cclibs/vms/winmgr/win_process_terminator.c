
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

void win_process_terminator(word term, win_r_event *event) {
/*
 * Function: win_process_terminator.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This sub program will take the terminator from an input event and 
 *    process in the proper way. This will allow for all input routines to 
 *    use the same processing of terminators.
 *
 * Modification History
 *
 * Variables Used
 */

	long stat;

/*
 * Main part of function.
 */

    if (term == SMG$K_TRM_PF1) {

        event->code = win_k_gold;
        event->term = 0;

    } else if ((term == SMG$K_TRM_HT) || 
               (term == SMG$K_TRM_BUFFER_FULL)) {

        event->code = win_k_normal;
        event->term = term;

    } else if (term == SMG$K_TRM_CR) {

        event->code = win_k_commit;
        event->term = term;

    } else if (((term >= 32) && (term <= 127)) || 
               (term == SMG$K_TRM_CTRLE) ||
               (term == SMG$K_TRM_CTRLH) ||
               (term == SMG$K_TRM_CTRLI) ||
               (term == SMG$K_TRM_CTRLJ) ||
               (term == SMG$K_TRM_CTRLX) ||
               (term == SMG$K_TRM_CTRLU) ||
               (term == SMG$K_TRM_CTRLA)) {

        if (event->code == win_k_gold) {

            if ((term == 80) || (term = 112)) {

                stat = smg$print_pasteboard(&init.pasteboard_id);
                vms_check_status(stat);
                win_message("Printing the screen...", FALSE);
                event->code = win_k_print_screen;
                event->term = 0;

            } else {

                event->code = win_k_gold_key;
                event->term = term;

            }

        } else {

            event->code = win_k_key;
            event->term = term;

        }

    } else if ((term == SMG$K_TRM_PF2) || 
               (term == SMG$K_TRM_HELP)) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_help;
            event->term = term;

        } else {

            event->code = win_k_help;
            event->term = term;

        }

    } else if (term == SMG$K_TRM_PF3) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_menu;
            event->term = 0;

        } else {

            event->code = win_k_menu;
            event->term = 0;

        }

    } else if (term == SMG$K_TRM_PF4) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_exit;
            event->term = 0;

        } else {

            event->code = win_k_exit;
            event->term = term;

        }

    } else if ((term == SMG$K_TRM_CTRLZ) || 
               (term == SMG$K_TRM_F10)) {

        event->code = win_k_gold_exit;
        event->term = 0;

    } else if ((term == SMG$K_TRM_UP) || 
               (term == SMG$K_TRM_RIGHT) ||
               (term == SMG$K_TRM_DOWN) || 
               (term == SMG$K_TRM_LEFT) ||
               (term == SMG$K_TRM_CTRLB) || 
               (term == SMG$K_TRM_CTRLR) ||
               (term == SMG$K_TRM_CTRLF) || 
               (term == SMG$K_TRM_CTRLD)) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_arrow;
            event->term = term;

            if (term == SMG$K_TRM_CTRLR) event->term = SMG$K_TRM_UP;
            if (term == SMG$K_TRM_CTRLB) event->term = SMG$K_TRM_DOWN;
            if (term == SMG$K_TRM_CTRLF) event->term = SMG$K_TRM_RIGHT;
            if (term == SMG$K_TRM_CTRLD) event->term = SMG$K_TRM_LEFT;

        } else {

            event->code = win_k_arrow;
            event->term = term;

            if (term == SMG$K_TRM_CTRLR) event->term = SMG$K_TRM_UP;
            if (term == SMG$K_TRM_CTRLB) event->term = SMG$K_TRM_DOWN;
            if (term == SMG$K_TRM_CTRLF) event->term = SMG$K_TRM_RIGHT;
            if (term == SMG$K_TRM_CTRLD) event->term = SMG$K_TRM_LEFT;

        }

    } else if ((term == SMG$K_TRM_FIND) || 
               (term == SMG$K_TRM_INSERT_HERE) ||
               (term == SMG$K_TRM_REMOVE) || 
               (term == SMG$K_TRM_SELECT) ||
               (term == SMG$K_TRM_PREV_SCREEN) || 
               (term == SMG$K_TRM_NEXT_SCREEN)) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_edit;
            event->term = term;

        } else {

            event->code = win_k_edit;
            event->term = term;

        }

    } else if ((term == SMG$K_TRM_F7) || 
               (term == SMG$K_TRM_F8) ||
               (term == SMG$K_TRM_F9) || 
               (term == SMG$K_TRM_F11) ||
               (term == SMG$K_TRM_F17) || 
               (term == SMG$K_TRM_F18) ||
               (term == SMG$K_TRM_F19) || 
               (term == SMG$K_TRM_F20)) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_function;
            event->term = term;

        } else {

            event->code = win_k_function;
            event->term = term;

        }

    } else if ((term == SMG$K_TRM_F12) || 
               (term == SMG$K_TRM_F13) ||
               (term == SMG$K_TRM_F14)) {

        event->code = win_k_key;
        event->term = term;

    } else if ((term == SMG$K_TRM_KP0) || 
               (term == SMG$K_TRM_KP1) ||
               (term == SMG$K_TRM_KP2) || 
               (term == SMG$K_TRM_KP3) ||
               (term == SMG$K_TRM_KP4) || 
               (term == SMG$K_TRM_KP5) ||
               (term == SMG$K_TRM_KP6) || 
               (term == SMG$K_TRM_KP7) ||
               (term == SMG$K_TRM_KP8) || 
               (term == SMG$K_TRM_KP9) ||
               (term == SMG$K_TRM_ENTER) || 
               (term == SMG$K_TRM_MINUS) ||
               (term == SMG$K_TRM_COMMA) || 
               (term == SMG$K_TRM_PERIOD)) {

        if (event->code == win_k_gold) {

            event->code = win_k_gold_keypad;
            event->term = term;

        } else {

            event->code = win_k_keypad;
            event->term = term;

        }

    } else {

        event->term = term;

    }

}

