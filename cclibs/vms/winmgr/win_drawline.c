
#include <ssdef.h>
#define __NEW_STARLET 1
#include <smgdef.h>
#undef __NEW_STARLET
#include <lib$routines.h>
#include <smg$routines.h>
#include <starlet.h>

#include "vms.h"
#include "win_defines.h"
#include "win_private.h"

/*-Prototypes-----------------------------------------------------------*/

/*----------------------------------------------------------------------*/

void win_drawline(long srow, long scol, long erow, long ecol, long attrib, 
                  win_r_window *win) {
/*
 * Function: win_drawline.c
 * Version : 1.0
 * Created : 28-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *	This function will draw a line at the specified locations.
 *
 * Modification History
 *
 * Variables Used
 */

	long stat;
	long rend = 0;

/*
 * Main part of function.
 */

    if (attrib == win_a_bold)      rend = SMG$M_BOLD;
    if (attrib == win_a_reverse)   rend = SMG$M_REVERSE;
    if (attrib == win_a_underline) rend = SMG$M_UNDERLINE;
    if (attrib == win_a_invisible) rend = SMG$M_INVISIBLE;

    stat = smg$draw_rectangle(win->display_id, &srow, &scol, &erow, &ecol, &rend, 0);
    vms_check_status(stat);

}

