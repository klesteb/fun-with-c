
#include <string.h>
#include <ctype.h>

#include "windows.h"

static int pull_down_sub(struct pop_struct sub_menu[], int normal, 
                         int inverse, int start, int *ext, int *expert);

int win_pulldown_menu(struct pd_struct pd_menu[], int normal, int inverse) {

    int i;                      /* general index variables                  */
    int cur_x;                  /* screen coordinates                       */
    int cur_opt = 0;            /* the current option (highlighted)         */
    int nu_opt;                 /* the number of options                    */
    int found = FALSE;          /* flag to indicate it option id found      */
    int expert = TRUE;          /* flag tp indicate expert/novice mode      */
    int ch;                     /* charachter typed                         */
    int return_code;            /* return code                              */
    int ext;                    /* returned keystroke from sub menu         */
    int start;                  /* column to start the sub menu window      */
    Windows *main_window;       /* the main options window                  */


    main_window = win_open("", 3, 80, 0, 0, FALSE);
                                        
    win_cursor(CURSOR_OFF);             /* turn cursor off                  */

    /* figure out how many options there are                                */

    nu_opt = MAIN_OPT;

    for (i = 0; i < MAIN_OPT; i++) {

        if (pd_menu[i].main[0] == '\0') {

            nu_opt = i;
            break;

        }

    }

    cur_opt = 0;

    for (;;) {                            /* infinite loop                  */

        cur_x = 1;

        for (i = 0; i < nu_opt; i++) {    /* print all options              */

            if (i == cur_opt)

                win_set_attr(main_window, inverse);

            else win_set_attr(main_window, normal);

            win_output(main_window, 0, cur_x, pd_menu[i].main);

            cur_x = cur_x + strlen(pd_menu[i].main) + 3;

        }

        if (found) {

            /* Figure out where to draw the pull-down box. The column must  */
            /* begin at leat on the 3rd column. We can calculate the column */
            /* at which to place the pull-down window by adding up the      */
            /* lengths of the main menu options appearing before the        */
            /* current option.                                              */

            start = 3;

            for (i = 0; i < cur_opt; i++)
                start = start + strlen(pd_menu[i].main) + 3;

            /* call function to create the pull down                        */

            return_code = pull_down_sub(pd_menu[cur_opt].sub, normal, 
                                        inverse, start, &ext, &expert);

            if (return_code != 0) break;

            /* reactivate expert mode                                       */

            if (ext == KEY_EXIT) {

                expert = TRUE;
                found = FALSE;

            } else if (ext == KEY_RIGHT) {

                expert = FALSE;
                cur_opt++;
                if (cur_opt >= nu_opt) cur_opt = 0;

            } else if (ext == KEY_LEFT) {

                expert = FALSE;
                cur_opt--;
                if (cur_opt < 0) cur_opt = nu_opt - 1;

            }

        } else {

            /* read the keystrokes until a selection is made                */

            ch = win_getch(main_window);

            if (ch == KEY_RIGHT) {

                cur_opt++;
                if (cur_opt >= nu_opt) cur_opt = 0;

            } else if (ch == KEY_LEFT) {

                cur_opt--;
                if (cur_opt < 0) cur_opt = nu_opt - 1;

            } else if (ch == KEY_ENTER) {

                found = TRUE;
                expert = FALSE;

            } else if (ch == KEY_DOWN) {

                found = TRUE;
                expert = FALSE;

            } else if (ch == KEY_EXIT) {

                return_code = 0;
                break;

            } else {

                /* do we have a letter? scan each option for a match        */

                ch = toupper(ch);

                for (i = 0; i < nu_opt; i++) {

                    if (pd_menu[i].speed_key == ch) {

                        cur_opt = i;
                        found = TRUE;
                        break;

                    }

                }

            }

        }

    }

    win_close(main_window);

    return(return_code);

}

static int pull_down_sub(struct pop_struct sub_menu[], int normal, 
                         int inverse, int start, int *ext, int *expert) {
                            
    int i;                  /* general index variables                      */
    int col;                /* screen coordinate                            */
    int width;              /* info for pull down window                    */
    int nu_opt;             /* the number of options in pull down           */
    int cur_opt = 0;        /* the current (highlighted) option             */
    int found = FALSE;      /* flag to indicate selection made (found)      */
    int return_code = 0;    /* return code                                  */
    int scr_cols;           /* number of rows and columns for stdscr        */
    int ch;                 /* character typed                              */
    Windows *pd_win;


    /* find out how many options are in pulldown menu                       */

    nu_opt = PD_SUB;

    for (i = 0; i < PD_SUB; i++) {

        if (sub_menu[i].name[0] == '\0') {

            nu_opt = i;
            break;

        }

    }

    /* figure out the max width of the window, assume 0 to start            */

    width = 0;

    for (i = 0; i < nu_opt; i++) {

        if (strlen(sub_menu[i].name) > width) {

            width = strlen(sub_menu[i].name);

        }

    }

    /* move box to the left if it will spill off the right side             */

    scr_cols = getmaxc(stdscr);

    if ((start + width + 1) > scr_cols) start = scr_cols - width - 2;

    /* create the pull-down window based on calculated values               */

    pd_win = win_open("", nu_opt + 2, width + 2, PD_SUB_ROW, start, FALSE);

    for (;;) {                        /* infinite loop                      */

        col = 0;

        for (i = 0; i < nu_opt; i++) {

            if (i == cur_opt)

                win_set_attr(pd_win, inverse);

            else win_set_attr(pd_win, normal);

            win_output(pd_win, col++, 0, sub_menu[i].name);

        }

        if (found) {

            if (*sub_menu[cur_opt].fun == NULL) {

                /* return the specified code if NULL                        */

                return_code = sub_menu[cur_opt].select_id;

            } else {

                /* a function was specified by pointer, call the            */
                /* function and get the return_code                         */

                return_code = (*sub_menu[cur_opt].fun)();

            }

            /* exit the pull-down if expert and return_code > 0             */

            if ((return_code != 0) && (*expert == TRUE)) break;

            if (return_code > 0) break;

            /* reset for the next pass                                      */

            if (return_code < 0) return_code = 0;
            found = FALSE;

        } else {

            /* read the keystrokes until a selection is made                */

            ch = win_getch(pd_win);

            if (ch == KEY_DOWN) {        /* move down                       */

                cur_opt++;    
                if (cur_opt >= nu_opt) cur_opt = 0;

            } else if (ch == KEY_UP) {    /* move up                        */

                cur_opt--;
                if (cur_opt < 0) cur_opt = nu_opt - 1;

            } else if (ch == KEY_RIGHT) {

                *ext = ch;
                break;

            } else if (ch == KEY_LEFT) {

                *ext = ch;
                break;

            } else if (ch == KEY_ENTER) {

                found = TRUE;
                *expert = FALSE;

            } else if (ch == KEY_EXIT) {

                *ext = ch;
                return_code = 0;
                break;

            } else {

                /* do we have a letter?    scan each option for a match     */

                ch = toupper(ch);

                for (i = 0; i < nu_opt; i++) {

                    if (sub_menu[i].speed_key == ch) {

                        cur_opt = i;
                        found = TRUE;
                        break;

                    }

                }

            }

        }

    }

    win_close(pd_win);

    return(return_code);

}


