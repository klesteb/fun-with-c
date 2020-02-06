
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

extern int trap_message(long, long, long, long, long);
extern char *space(int);
extern char *string(int, int);
extern char *create_zone_overpunch(char *, char *, int);

static int validate_entry(char *, win_r_entry *);
static char *adjust(char *, int);
static void startup(char *, win_r_entry *);
static void setup_term(win_r_entry *);
static void reset_term(void);

/*-Global-Variables-----------------------------------------------------*/

static int kol;
static int rend;
static int zone;
static int zone_sign;
static int dsp_len;
static int fil_len;
static int overstrike;
static int position;

/*----------------------------------------------------------------------*/

static void startup(char *values, win_r_entry *entry) {
/*
 * Function: startup.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Do basic startup stuff.
 *
 * Modification History
 *
 * Variables Used
 */

/*
 * Main part of function.
 */

    fmt = edit$(entry->fmt, 8+128)

    if ((entry->data_type == win_t_numeric) ||
        (entry->data_type == win_t_snumeric)) zone = TRUE;

    if (entry->data_type == win_t_snumeric) zone_sign = TRUE;

    dsp_len = strlen(fmt);
    dsp_str = string(dsp_len, '_');
    fil_len = dsp_len - strlen(values);
    kol = entry->col - 1;
    overstrike = FALSE;

    if (strlen(values) == 0) {

        position = 1;

    } else {

        if (entry->data_type != win_t_boolean) {

            position = strlen(values) + 1;

        } position = strlen(values);

        inp_dat = values;

    }

}

/**/

static void setup_term(win_r_entry *entry) {
/*
 * Function: setup_term.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Setup the terminal for data entry.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long mode;

/*
 * Main part of function.
 */

    /* Turn the cursor on.                                              */

    mode = SMG$M_CURSOR_ON;
    stat = smg$set_cursor_mode(&init.pasteboard_id, &mode);
    vms_check_status(stat);

    /* Turn AST level trapping of broadcast messages off.               */

    stat = smg$set_broadcast_trapping(&init.pasteboard_id, 0, 0);
    vms_check_status(stat);

    /* Determine the rendition type.                                    */

    if (entry->attrib & win_a_blink)        rend |= SMG$M_BLINK;
    if (entry->attrib & win_a_bold)         rend |= SMG$M_BOLD;
    if (entry->attrib & win_a_reverse)      rend |= SMG$M_REVERSE;
    if (entry->attrib & win_a_invisible)    rend |= SMG$M_INVISIBLE;
    if (entry->data_type & win_t_invisible) rend |= SMG$M_INVISIBLE;

}

/**/

static void reset_term(void) {
/*
 * Function: reset_term.c
 * Version : 1.0
 * Created : 21-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Reset the terminal to the state before entry was begun.
 *
 * Modification History
 *
 * Variables Used
 */

    long stat;
    long mode;

/*
 * Main part of function.
 */

    /* Turn cursor back off.                                            */

    mode = SMG$M_CURSOR_OFF;
    stat = smg$set_cursor_mode(&init.pasteboard_id, &mode);
    vms_check_status(stat);

    /* Check for any broadcast messages.                                */

    win_display_broadcast_message();

    /* Turn AST trapping of broadcast messages on.                      */

    stat = smg$set_broadcast_trapping(&init.pasteboard_id, &trap_message, 0);
    vms_check_status(stat);

}

/**/

static void display_entry(win_r_entry *entry, win_r_window *win) {
/*
 * Function: display_entry.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    Redisplay the formated entry.
 *
 * Modification History
 *
 * Variables Used
 */

    char *dsp;
    char buffer[256];

/*
 * Main part of function.
 */

    if ((inp_len != 0) ||
        ((event->code == win_k_exit) ||
         (event->code == win_k_gold_exit))) {

        if (zone == TRUE) {

            values = create_zone_overpunch(inp_dat, buffer, 255);

        } else {

            /* If data entered or delete field, copy in the old value.  */

            strcpy(values, inp_dat);

        }

    }

    /* Format the data entered.                                         */

    dsp = space(dsp_len);

    if (zone == TRUE) {

        dsp = format_zone(values, fmt)

    } else {

        if (strlen(fmt) == 0) {

            strcpy(dsp, values);

        } else {

            lset dsp = edit$(format_ascii(values, fmt), 8+128)

        }

    }

    /* Display it unless invisible.                                     */

    lset dsp = "" if (entry->data_type = win_t_invisible)
    win_output(entry->row, entry->col, dsp, 0, rend, win);

    free(dsp);

}

/**/

void win_input(char *values, win_r_entry *entry, win_r_window *win) {
/*
 * Function: win_input.c
 * Version : 1.0
 * Created : 11-Jul-2000
 * Author  : Kevin Esteb
 *
 * Description
 *
 *    This routine was developed from inpfld.b2s by Bruce Barker.
 *
 *    This routine gets a string from the terminal. The routine parses 
 *    the data from the delimiter typed. The data is stored back in the 
 *    passed area "VALUES" either right or left justified, depending on 
 *    the fill justifaction. The data is validated as correct. If incorrect 
 *    then an error message is printed and the field is asked for again. The 
 *    delimiter the field ended with is passed back. the default "VALUES" 
 *    should contain the defualt value. The delimiter the field ended with 
 *    is passed back. 
 *
 *    Inputs:
 *        Values              :default input
 *        entry->row          :row to start at
 *        entry->col          :column
 *        entry->fmt          :format of how characters to be
 *                             displayed using format_zone or format_ascii
 *        entry->data_type    :type of input
 *                            win_t_alpha         = alpha (A-Z," ")
 *                            win_t_alpha_numeric = alpha numeric (A-Z," ",0-9)
 *                            win_t_anything      = anything
 *                            win_t_lower_case    = upper + lower case
 *                            win_t_invisible     = invisible
 *                            win_t_numeric       = numeric
 *                            win_t_snumeric      = signed numeric
 *                            win_t_date          = date (mmddyy)
 *                            win_t_time          = time (hh:mm)
 *                            win_t_boolean       = y/n
 *
 *        entry->min_len      :miniumn number of characters required 
 *        entry->attrib       :how the entry is to be displayed on the screen
 *        entry->help_topic   :help topic within a help library
 *        entry->help_line    :one line help prompt
 *        win                 :window entry is to take place in
 *
 *    Outputs:
 *        values              :value returned
 *        entry->event        :code of the function key
 *
 * Modification History
 *
 * Variables Used
 */

    word term_code;

    long stat;

    int x;
    int no_good;
    int gold_key;
    int no_more = FALSE;
    int not_done = TRUE;

    declare string a
    declare string b
    declare string fmt
    declare string inp_dat
    declare string dsp_str

    win_r_event event;

/*
 * Main part of function.
 */

    startup(values, entry);
    setup_term(entry);

    /* Loop until data entry is correct.                                */

    do {

        gosub display_field
        gosub input_field                           ! Get the data.

    } while (not_done);

    display_entry(entry, win);
    reset_term();

}



    Input_Field:
    !
    ! This routine will accept keystrokes from the virtual keyboard and then
    ! determine what to do with that keystroke.
    !
        until no_more

            !
            ! Position the cursor for entry.
            !

            stat = smg$set_cursor_abs(win->display_id, entry->row, kol + position)
            call lib$stop(stat by value) if (stat and 1) = 0

            !
            ! Loop until data entry is correct.
            !

            stat = smg$read_keystroke(&init.keyboard_id, term_code,,,       &
                                      win->display_id, rend,)
            if ((stat <> smg$k_trm_timeout) and                             &
                (stat <> smg$_eof) and                                      &
                (stat <> ss$_cancel) and                                    &
                (stat <> ss$_abort) and                                     &
                (stat <> ss$_timeout)) then
                call lib$stop(stat by value) if (stat and 1) = 0
            end if

            call win_process_terminator(term_code, event)
            select event->code
                case win_k_gold
                    gold_key = true

                case win_k_normal, win_k_commit
                    gosub validate_data             ! Validate the entry.
                    entry->event = event
                    no_more = true

                case win_k_gold_key
                    entry->event = event
                    done = true
                    no_more = true

                case win_k_gold_help
                    call win_display_help(entry->help_topic, "")

                case win_k_help
                    call win_message(entry->help_line, false)

                case win_k_gold_exit, win_k_exit
                    stat = smg$repaint_line(&init.pasteboard_id, entry->row)
                    call lib$stop(stat by value) if (stat and 1) = 0
                    entry->event = event
                    done = true
                    no_more = true
                
                case win_k_arrow
                    select event->term
                        case smg$k_trm_left
                            if (position > 1) then
                                position = position - 1
                            else
                                position = 1
                                call win_message("Retreated to far, repositioned to start.", false)
                            end if
                        
                        case smg$k_trm_right
                            if (position <= dsp_len) then
                                position = position + 1
                            else
                                position = dsp_len
                                call win_message("Advanced to far, repositioned to end.", false)
                            end if

                        case smg$k_trm_up, smg$k_trm_down
                            entry->event = event
                            done = true
                            no_more = true

                    end select

                case win_k_gold_arrow
                    select event->term
                        case smg$k_trm_left
                            position = 1

                        case smg$k_trm_right
                            position = dsp_len

                        case smg$k_trm_up, smg$k_trm_down
                            entry->event = event
                            done = true
                            no_more = true

                    end select

                case win_k_menu, win_k_gold_menu
                    entry->event = event
                    done = true
                    no_more = true

                case win_k_key
                    gosub process_win_k_key

                case else
                    gold_key = false if gold_key
                
            end select

        next

        if (entry->data_type = win_t_lower_case) then

            inp_dat = edit$(inp_dat ,4+8+16+128)

        else

            inp_dat = edit$(inp_dat ,4+8+16+32+128)

        end if

        inp_len = len(inp_dat)

    return



    Process_Win_K_Key:
    !
    ! Process the win_k_key event.
    !
        select event->term
            case smg$k_trm_delete
                if (position > 1) then
                    position = position - 1
                    inp_dat = left$(inp_dat, position - 1) +                &
                              right$(inp_dat, position + 1)
                    stat = smg$begin_display_update(win->display_id)
                    call lib$stop(stat) if (stat and 1) = 0
                    stat = smg$delete_chars(win->display_id, 1, entry->row, &
                                            kol + position)
                    call lib$stop(stat) if (stat and 1) = 0
                    stat = smg$insert_chars(win->display_id, "_", entry->row,&
                                            kol + dsp_len, rend,,)
                    call lib$stop(stat) if (stat and 1) = 0
                    stat = smg$end_display_update(win->display_id)
                    call lib$stop(stat) if (stat and 1) = 0
                    inp_len = len(inp_dat)
                else
                    position = 1
                    call win_message("No, can't delete any more characters", false)
                end if

            case smg$k_trm_ctrla
                if (overstrike = true) then
                    overstrike = false
                else
                    overstrike = true
                end if

            case smg$k_trm_ctrle
                position = dsp_len

            case smg$k_trm_ctrlh
                position = 1

            case smg$k_trm_ctrlj
!                inp_dat = adjust(inp_dat, position)
!                gosub display_field
!                inp_len = len(inp_dat)

            case smg$k_trm_ctrlu
                inp_dat = right$(inp_dat, position)
                stat = smg$begin_display_update(win->display_id)
                gosub display_field
                inp_len = len(inp_dat)
                position = 1

            case smg$k_trm_ctrlx
                inp_dat = ""
                gosub display_field
                inp_len = len(inp_dat)
                position = 1

            case else
                gosub do_char_input

        end select

    return



    Do_Char_Input:
    !
    ! Handle the single character input.
    !
        if ((position <= dsp_len) and (inp_len <= dsp_len)) then
            stat = smg$begin_display_update(win->display_id)
            call lib$stop(stat) if (stat and 1) = 0
            if ((entry->data_type = win_t_boolean) or                        &
                (entry->data_type = win_t_single)) then
                inp_dat = chr$(term_code)
                if (entry->data_type = win_t_invisible) then
                    call win_output(entry->row, kol + position,              &
                                   "*", 0, rend, win)
                else
                    call win_output(entry->row, kol + position,              &
                                   chr$(term_code), 0, rend, win)
                end if
            else
                if (position <= inp_len) then
                    if (overstrike = true) then
                        mid$(inp_dat, position, 1) = chr$(term_code)
                        if (entry->data_type = win_t_invisible) then
                            call win_output(entry->row, kol + position,      &
                                           string$(inp_len, ascii("*")),    &
                                           0, rend, win)
                        else
                            call win_output(entry->row, kol + position,      &
                                           chr$(term_code), 0, rend, win)
                        end if
                    else
                        if (inp_len < dsp_len) then
                            a = left$(inp_dat, position - 1)
                            b = right$(inp_dat, position )
                            inp_dat = a + chr$(term_code) + b

                            call win_output(entry->row, entry->col, dsp_str, &
                                           0, rend, win)

                            if (entry->data_type = win_t_invisible) then
                                call win_output(entry->row, entry->col,      &
                                               string$(inp_len, ascii("*")),&
                                               0, rend, win)
                            else
                                call win_output(entry->row, entry->col,      &
                                               inp_dat, 0, rend, win)
                            end if
                        else
                            call win_message("No, entry is to long.", true)
                        end if
                    end if
                else
                    inp_dat = inp_dat + chr$(term_code)
                    if (entry->data_type = win_t_invisible) then
                        call win_output(entry->row, kol + position,          &
                                       "*", 0, rend, win)
                    else
                        call win_output(entry->row, kol + position,          &
                                       chr$(term_code), 0, rend, win)
                    end if
                end if
                position = position + 1
            end if
            inp_len = len(inp_dat)
            stat = smg$end_display_update(win->display_id)
            call lib$stop(stat) if (stat and 1) = 0
        else
            call win_message("No, entry is to long.", true)
        end if

    return



    Validate_Data:
    !
    ! Validate the entry to see if its ok.
    !
        done = true

        if ((event->code <> win_k_exit) or (event->code <> win_k_gold_exit)) then

            if (validate_entry(inp_dat, entry)) then    ! Validate the entry.

                if ((inp_len < entry->min_length) and (inp_len <> 0)) then

                    !
                    ! Be sure we got enough chars.
                    !

                    done = false
                    call win_message("No, you must enter more characters.", true)
                    return

                end if

            end if

        end if

    return



    Display_Entry:
    return


    Startup:
    !
    ! Sub program startup stuff.
    !
    return



    Setup_Term:
    return



    Reset_Term:
    return


    Display_Field:
    !
    ! Display the entry field.
    !
        stat = smg$begin_display_update(win->display_id)
        call lib$stop(stat) if (stat and 1) = 0

        call win_output(entry->row, entry->col, dsp_str, 0, rend, win)
        call win_output(entry->row, entry->col, inp_dat, 0, rend, win)

        call lib$stop(stat) if (stat and 1) = 0
        stat = smg$end_display_update(win->display_id)

    return

    end when

    handler error_handler

        exit handler

    end handler

end sub



FUNCTION integer validate_entry(string text, win_entry entry)
!
!    FUNCTION: validate_entry.bas
!    VERSION : V1.0
!    CREATED : 17-dec-1990
!    AUTHOR  : Kevin Esteb
!
!    SYNOPSIS
!
!    This function will validate the data that was entered to see if it is the
!    right stuff. If not then the user will be notified and entry will resume
!    again.
!
!    MODIFICATION HISTORY
!
!
!    PROGRAM OPTIONS
!

    %include "app$library:std.inc"

!
!    VARIBLES USED
!

    %include "app$library:win_defines.inc"

    declare integer x
    declare integer bad
    declare integer stat
    declare integer hours
    declare integer minutes
    declare integer txt_len

    declare string msg

!
!    SUBPROGRAMS - FUNCTIONS
!

    %include "app$library:dates.inc"



!
! Main part of function.
!
    when error use error_handler
    !
    ! Assume that everything is ok.
    !

    txt_len = len(text)
    stat = true

    !
    ! Check the entry by data type.
    !

    select entry->data_type
        case win_t_alpha
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "A" to "Z", " "
                    case else
                        stat = false
                        msg = "Must enter upper case characters only..."
                end select
            next x
    
        case win_t_alpha_numeric
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "A" to "Z", " ", "0" to "9"
                    case else
                        stat = false
                        msg = "Must enter upper case and numbers only..."
                end select
            next x

        case win_t_lower_case
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "A" to "Z", " ", "a" to "z", "0" to "9"
                    case else
                        stat = false
                        msg = "Must enter upper/lower case characters and numbers only..."
                end select
            next x

        case win_t_numeric, win_t_snumeric
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "0" to "9", "-", "+", "."
                    case else
                        stat = false
                        msg = 'Must enter numbers or "-" or "+" only...'
                end select
            next x

        case win_t_time
            msg = "Must enter time format hh:mm only..."
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "0" to "9", ":"

                    case else
                        stat = false

                end select
            next x
            msg = "Invalid time..."
            when error in
                stat = false if txt_len <> 5
                hours = val(seg$(text, 1, 2))
                stat = false if hours <= 0 and hours => 24 
                minutes = val(seg$(text, 4, 5))
                stat = false if minutes <= 0 and minutes => 60 
            use
                stat = false
            end when

        case win_t_date
            for x = 1 to txt_len
                select mid$(text, x, 1)
                    case "0" to "9", "/", "-"

                    case else
                        msg = 'Must enter numbers or "/" or "-" only...'
                        stat = false

                end select
            next x
            select weekday(text)
                case 1 to 7

                case else
                    msg = "Invalid date..."
                    stat = false

            end select

        case win_t_boolean
            for x = 1 to txt_len
                text = edit$(text, 32)
                select mid$(text, x, 1)
                    case "Y", "N", "T", "F" 

                    case else
                        msg = "Must enter (Y/N) or (T/F) only..."
                        stat = false

                end select
            next x

        case win_t_anything, win_t_single
        case else

    end select

    call win_message(msg, true) if (stat = false)

    end when

    handler error_handler
    !
    ! Return errors to the calling routine.
    !
        exit handler

    end handler

end function (stat)



SUB win_process_delimiter(integer position, integer start, integer finish, integer done, win_entry entry)
!
!    SUBPROG: win_process_delimiter.bas
!    VERSION: V1.0
!    CREATED: 18-dec-1990
!    AUTHOR : Kevin Esteb
!
!    SYNOPSIS
!
!    This sub program will process the delimiter that is return by inpfld. 
!
!    MODIFICATION HISTORY
!
!
!    PROGRAM OPTIONS
!

    %include "app$library:std.inc"

!
!    VARIBLES USED
!

    %include "$smgdef" %from %library "sys$library:basic$starlet.tlb"

    %include "app$library:win_defines.inc"

!
!    SUBPROGRAMS - FUNCTIONS
!



!
! Main part of sub program.
!

    when error use error_handler

        select entry->event->code
            case win_k_arrow
                select entry->event->term
                    case smg$k_trm_up
                        if position > start then
                            position = position - 1 
                        else 
                            position = finish
                        end if

                    case smg$k_trm_down
                        if position < finish then
                            position = position + 1
                        else
                            position = start
                        end if

                    case else

                end select

            case win_k_gold_arrow
                select entry->event->term
                    case smg$k_trm_up
                        position = start 

                    case smg$k_trm_down
                        position = finish

                    case else

                end select

            case win_k_normal
                if position < finish then
                    position = position + 1
                else
                    position = start
                end if

            case win_k_exit, win_k_gold_exit, win_k_commit,                    &
                 win_k_menu, win_k_gold_menu
                done = true

            case else

        end select

    end when

    handler error_handler
    !
    ! Return errors to calling routine.
    !
        exit handler

    end handler

end sub


