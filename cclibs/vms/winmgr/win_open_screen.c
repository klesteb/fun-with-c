
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <ttdef.h>
#include <tt2def.h>
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"
#include "bitops.h"

/*-Prototypes-----------------------------------------------------------*/

int trap_message(long, long, long, long, long);
int trap_out_of_band(SMG$R_OUT_OF_BAND_TABLE, long, long, long, long);

/*-Global-Variables-----------------------------------------------------*/

win_r_initial init;
long win0;

/*----------------------------------------------------------------------*/

void win_open_screen(char *help_file, int application) {
/*
 * Function: win_open_screen.c
 * Version : 1.0
 * Created : 22-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is used to attach the SMG routines to the terminal.
 *    It also allowes for the trapping of broadcast messages, ctrl/c - 
 *    ctrl/y It allowes the messages to be displayed at the bottom of the 
 *    screen. Will read in any key tables that may have been defined. And 
 *    set up a link to a DCL style help library.
 *
 * Modification History
 *
 * Variables Used
 */

    long row;
    long col;
    long stat;
    long size;
    long flags = 0;
    long key_mask = 0;
    long video_mask = 0;
    long display_mask = 0;

    SMG$R_ATTRIBUTE_INFO_BLOCK info;

/*
 * Main part of function.
 */

    video_mask = SMG$M_BOLD;
    display_mask = SMG$M_TRUNC_ICON;
    memset(&init.help_file, '\0', 256);
    strncpy(init.help_file, help_file, 255);

    /* Set the bitmask for out-of-band handling.                        */

    bit_set(key_mask, SMG$K_TRM_CTRLC);
    bit_set(key_mask, SMG$K_TRM_CTRLY);
    bit_set(key_mask, SMG$K_TRM_CTRLO);
    bit_set(key_mask, SMG$K_TRM_CTRLW);
 
    /* Create the pasteboard.                                           */

    stat = smg$create_pasteboard(&init.pasteboard_id);
    vms_check_status(stat);

    /* Get the pasteboard attributes.                                   */

    size = SMG$C_PASTEBOARD_INFO_BLOCK;
    stat = smg$get_pasteboard_attributes(&init.pasteboard_id, &info, &size);
    vms_check_status(stat);

    /* Store that info for later usage.                                 */

    init.cols = info.smg$w_num_columns;
    init.rows = info.smg$b_num_rows;
    init.dev  = info.smg$b_pbd_type;

    /* Associate a virtual keyboard with the terminal.                  */

    stat = smg$create_virtual_keyboard(&init.keyboard_id);
    vms_check_status(stat);

    /* Set the keypad mode.                                             */

    if (application) flags = SMG$M_KEYPAD_APPLICATION;

    stat = smg$set_keypad_mode(&init.keyboard_id, &flags);
    vms_check_status(stat);
     
    /* Create a background display area. Think of this as a stdscr ie.  */
    /* the Unix curses library. Now we can put text on the terminal     */
    /* without creating a window...                                     */

    stat = smg$create_virtual_display(&init.rows, &init.cols, &win0, 
                                      &display_mask, &video_mask, 0);
    vms_check_status(stat);

    /* Postion it on the screen.                                        */

    col = 1;
    stat = smg$paste_virtual_display(&win0, &init.pasteboard_id,
                                     &init.rows, &col, 0);
    vms_check_status(stat);

    /* Create a display area for messages and such things.              */

    row = 1;
    col = init.cols;
    stat = smg$create_virtual_display(&row, &col, &init.msg_display_id, 
                                      &display_mask, &video_mask, 0);
    vms_check_status(stat);

    /* Position it at the bottom of the screen.                         */

    row = init.rows;
    col = 1;
    stat = smg$paste_virtual_display(&init.msg_display_id, 
                                     &init.pasteboard_id, &row, &col, 0);
    vms_check_status(stat);

    /* Create a display area for titles and such things.                */

    row = 2;
    stat = smg$create_virtual_display(&row, &init.cols, 
                                      &init.title_display_id, 
                                      &display_mask, &video_mask, 0);
    vms_check_status(stat);

    /* Position it at the top of the screen.                            */

    row = 2;
    col = 1;
    stat = smg$paste_virtual_display(&init.title_display_id, 
                                     &init.pasteboard_id, &row, &col, 0);
    vms_check_status(stat);

    /* Turn AST trapping of broadcast messages on.                      */

    stat = smg$set_broadcast_trapping(&init.pasteboard_id, &trap_message, 0);
    vms_check_status(stat);

    /* Trap ctrl/y - ctrl/c keystrokes and display appropiate message.  */
 
    stat = smg$set_out_of_band_asts(&init.pasteboard_id, &key_mask,
                                    &trap_out_of_band, init.msg_display_id);
    vms_check_status(stat);

    /* Clear the pasteboard.                                            */

    stat = smg$erase_pasteboard(&init.pasteboard_id);
    vms_check_status(stat);

}

/**/

int trap_message(long junk, long r0, long r1, long pc, long psl_4) {
/*
 * Function: trap_message.c
 * Version : 1.0
 * Created : 22-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This subprogam is activated when a broadcast message is sent to the
 *    terminal. This is the AST level of interface. The real display and
 *    handling of boradcast messages is done by win_display_broadcast_message.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

/*
 * Main part of function.
 */

    stat = smg$cancel_input(&init.keyboard_id);
    vms_check_status(stat);

    win_display_broadcast_message();

    return(0);

}

/**/

int trap_out_of_band(SMG$R_OUT_OF_BAND_TABLE tbl, long r0, long r1, long pc, long psl_4) {
/*
 * Function: trap_out_of_band.c
 * Version : 1.0
 * Created : 22-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This function is activated when a out of band ast is generated. An
 *    appropiate response will be displayed in the message area at the bottom
 *    of the screen. If that protion of the screen is covered it will be
 *    moved to the front for a peroid of time and then returned to the
 *    background.
 *
 * Modification History
 *
 * Variables Used
 */

    long row;
    long col;
    long stat;
    long size;
    long lines;

    char *ctrl_c_msg = "Typing ctrl c is not appropiate at this time...";
    char *ctrl_y_msg = "Typing ctrl y is not appropiate at this time...";
    char *ctrl_o_msg = "Typing ctrl o is not appropiate at this time...";

    SMG$R_ATTRIBUTE_INFO_BLOCK info;
    struct dsc$descriptor msg_desc;

/*
 * Main part of function.
 */

    size = SMG$C_PASTEBOARD_INFO_BLOCK;
    stat = smg$get_pasteboard_attributes(&tbl.smg$l_pbd_id, &info, &size);
    vms_check_status(stat);

    /* Do the appropiate action for the out of band ast.                */

    switch (tbl.smg$b_char) {
        case SMG$K_TRM_CTRLC:
            lines = 1;
            SET_SDESC(msg_desc, ctrl_c_msg, strlen(ctrl_c_msg));
            stat = smg$put_line(&tbl.smg$l_user_arg, &msg_desc, &lines, 0,0,0,0,0);
            vms_check_status(stat);
            break;

        case SMG$K_TRM_CTRLO:
            SET_SDESC(msg_desc, ctrl_o_msg, strlen(ctrl_o_msg));
            stat = smg$put_line(&tbl.smg$l_user_arg, &msg_desc, &lines, 0,0,0,0,0);
            vms_check_status(stat);
            break;

        case SMG$K_TRM_CTRLY:
            SET_SDESC(msg_desc, ctrl_y_msg, strlen(ctrl_y_msg));
            stat = smg$put_line(&tbl.smg$l_user_arg, &msg_desc, &lines, 0,0,0,0,0);
            vms_check_status(stat);
            break;

        case SMG$K_TRM_CTRLW:
            init.cols = info.smg$w_num_columns;
            init.rows = info.smg$b_num_rows;
            stat = smg$repaint_screen(&tbl.smg$l_pbd_id);
            vms_check_status(stat);
            break;

        case SMG$K_TRM_CTRLR:
        case SMG$K_TRM_CTRLU:
        case SMG$K_TRM_CTRLJ:
            col = 1;
            row = info.smg$w_phys_cursor_row;
            stat = smg$repaint_line(&tbl.smg$l_pbd_id, &row, &col);
            vms_check_status(stat);
            break;

        default:

    }

    return(0);

}

