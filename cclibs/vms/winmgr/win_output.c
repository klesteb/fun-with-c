
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

void win_output(long row, long col, char *text, long display, long attrib, 
                win_r_window *win) {
/*
 * Function: win_output.c
 * Version : 1.0
 * Created : 28-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This subprogram is used to display text on the screen. The following
 *    switches are used for clearing the display area before printing the text.
 *
 *        0 - no erasing.
 *        1 - erase from cursor position to end of line.
 *        2 - erase display area from top to cursor position.
 *        3 - erase diplay area from cursor position to bottom.
 *        4 - erase complete display area.
 *        5 - erase the text area.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long length;
    long rend = 0;
    long hrow = 1;
    long hcol = 1;
    long flags = 0;

    struct dsc$descriptor text_dsc;

/*
 * Main part of function.
 */

    if (attrib & win_a_blink)     rend |= SMG$M_BLINK;
    if (attrib & win_a_bold)      rend |= SMG$M_BOLD;
    if (attrib & win_a_reverse)   rend |= SMG$M_REVERSE;
    if (attrib & win_a_invisible) rend |= SMG$M_INVISIBLE;

    switch (display) {
        case 1:
            stat = smg$erase_display(win->display_id, &row, &col, &row, win->length);
            vms_check_status(stat);
            break;

        case 2:
            stat = smg$erase_display(win->display_id, &hrow, &hcol, &row, &col);
            vms_check_status(stat);
            break;

        case 3:
            stat = smg$erase_display(win->display_id, &row, &col, 0,0);
            vms_check_status(stat);
            break;

        case 4:
            stat = smg$erase_display(win->display_id, 0,0,0,0);
            vms_check_status(stat);
            break;

        case 5:
            length = col + strlen(text);
            stat = smg$erase_display(win->display_id, &row, &col, &row, &length);
            vms_check_status(stat);
            break;

        default:

    }

    SET_SDESC(text_dsc, text, strlen(text));

    stat = smg$put_chars(win->display_id, &text_dsc, &row, &col, &flags, &rend, 0,0);
    vms_check_status(stat);

}

