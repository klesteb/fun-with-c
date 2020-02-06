
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

extern int trap_out_of_band(SMG$R_OUT_OF_BAND_TABLE, long, long, long, long);

/*----------------------------------------------------------------------*/

void win_close_screen(void) {
/*
 * Function: win_close_screen.c
 * Version : 1.0
 * Created : 22-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This sub program dettaches the SMG routines from the terminal.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long mask = 0;

/*
 * Main part of function.
 */

    /* Remove the message display area.                                 */

    stat = smg$delete_virtual_display(&init.msg_display_id);
    vms_check_status(stat);

    /* Remove the title display area.                                   */

    stat = smg$delete_virtual_display(&init.title_display_id);
    vms_check_status(stat);

    /* Disable trapping of broadcast messages.                          */

    stat = smg$disable_broadcast_trapping(&init.pasteboard_id);
    vms_check_status(stat);

    /* Disbale trapping of ctrl/y - ctrl/c keystrokes.                  */

    stat = smg$set_out_of_band_asts(&init.pasteboard_id, &mask,
                                    &trap_out_of_band, init.msg_display_id);
    vms_check_status(stat);

    /* Clear the pasteboard.                                            */

    stat = smg$erase_pasteboard(&init.pasteboard_id);
    vms_check_status(stat);

    /* Delete the pasteboard.                                           */

    stat = smg$delete_pasteboard(&init.pasteboard_id);
    vms_check_status(stat);

}

