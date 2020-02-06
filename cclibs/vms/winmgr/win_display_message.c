
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

void win_display_message(char *text, int bell) {
/*
 * Function: win_display_message.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine will print a message in the message area on the screen. 
 *    It will not clear the message area on exit. It will also ring the 
 *    terminals bell if so desired.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;

    struct dsc$descriptor text_dsc;

/*
 * Main part of function.
 */

    SET_SDESC(text_dsc, text, strlen(text));

    stat = smg$put_line(&init.msg_display_id, &text_dsc);
    vms_check_status(stat);

    if (bell) {

        stat = smg$ring_bell(&init.msg_display_id);
        vms_check_status(stat);

    }

}
