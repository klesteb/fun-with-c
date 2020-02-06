
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <descrip.h>
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>
#include <stdio.h>
#include <string.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

extern char *trim(char *);

/*----------------------------------------------------------------------*/

void win_open_window(win_r_window *win) {
/*
 * Function: win_open_window.c
 * Version : 1.0
 * Created : 22-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will allow the user to open a display window on the
 *    pasteboard in an consistent manner. This includes borders around 
 *    the window, a banner on the top.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
	long attrib;
	long units;
	char *holder = NULL;

	struct dsc$descriptor banner_desc;

/*
 * Main part of function.
 */

    /* Create the virtual display window.                               */

	attrib = SMG$M_BORDER;
    stat = smg$create_virtual_display(&win->width, &win->length,
                                      &win->display_id, &attrib, 0,0);
    vms_check_status(stat);

    /* Position it on the screen.                                       */

    stat = smg$paste_virtual_display(&win->display_id, &init.pasteboard_id,
                                     &win->row, &win->col);
    vms_check_status(stat);

    /* Place a banner on top of the window.                             */

	units = 2;
	attrib = SMG$K_TOP;
	holder = trim(win->banner);
	SET_SDESC(banner_desc, holder, strlen(holder));
    stat = smg$label_border(&win->display_id, &banner_desc, &attrib, &units, 0,0,0);
    vms_check_status(stat);

    /* Clear the display.                                               */

    stat = smg$erase_display(&win->display_id);
    vms_check_status(stat);

    /* Home the cursor in upper left corner.                            */

	attrib = SMG$C_UPPER_LEFT;
    stat = smg$home_cursor(&win->display_id, &attrib);
    vms_check_status(stat);

    /* Turn the cursor off.                                             */

	attrib = SMG$M_CURSOR_OFF;
    stat = smg$set_cursor_mode(&init.pasteboard_id, &attrib);
    vms_check_status(stat);

}

