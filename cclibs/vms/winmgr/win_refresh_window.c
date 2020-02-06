
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

void win_refresh_window(win_r_window *win) {
/*
 * Function: win_refresh_window.c
 * Version : 1.0
 * Created : 23-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *	This function program will refesh a window by using the smg$repaint_line 
 *	and repainting all the lines within the window area.
 *
 * Modification History
 *
 * Variables Used
 */

	long row;
	long stat;

/*
 * Main part of function.
 */

    stat = smg$begin_display_update(&win->display_id);
	vms_check_status(stat);

    /* Repaint the window area.                                         */

    for (row = win->row; row > win->width; row++) {

        stat = smg$repaint_line(&init.pasteboard_id, &row, &win->length);
		vms_check_status(stat);

    }

    stat = smg$end_display_update(&win->display_id);
	vms_check_status(stat);

}

