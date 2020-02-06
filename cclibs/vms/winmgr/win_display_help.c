
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

char *str_replace(char *, char *, char *, char *, int);

/*----------------------------------------------------------------------*/

void win_display_help(char *topic, char *sub_topic) {
/*
 * Function: win_display_help.c
 * Version : 1.0
 * Created : 27-Jun-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This subprogram will display the help library on the screen within 
 *    its own window. The user can also start the help display at the 
 *    given topic.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long mode;
    char *help_sub_topic;
    char buffer[1024];

    win_r_window help;

    struct dsc$descriptor help_dsc;
    struct dsc$descriptor file_dsc;

/*
 * Main part of function.
 */

    if (strlen(init.help_file) != 0) {

        /* Open a display window.                                       */

        SET_WIN(help, 2, 2, 21, 78, "Help");
        win_open_window(&help);

        /* Turn the cursor on.                                          */

        mode = SMG$M_CURSOR_ON;
        stat = smg$set_cursor_mode(&init.pasteboard_id, &mode);
        vms_check_status(stat);

        /* Display the help library.                                    */

        help_sub_topic = str_replace(sub_topic, " ", "_", buffer, 1023);

        SET_SDESC(help_dsc, help_sub_topic, strlen(help_sub_topic));
        SET_SDESC(file_dsc, init.help_file, strlen(init.help_file));

        stat = smg$put_help_text(&help.display_id, &init.keyboard_id,
                                 &help_dsc, &file_dsc);
        vms_check_status(stat);

        /* Turn the cursor off.                                         */

        mode = SMG$M_CURSOR_OFF;
        stat = smg$set_cursor_mode(&init.pasteboard_id, &mode);
        vms_check_status(stat);

        /* Close display window.                                        */

        win_close_window(&help);

    } else {

        win_message("Sorry, Help is not available.", TRUE);

    }

}

