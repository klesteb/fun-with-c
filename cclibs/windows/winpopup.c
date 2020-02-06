
#include <string.h>
#include <ctype.h>

#include "windows.h"

int win_popup_menu(struct pop_struct pop_menu[], int x, int y, 
                   int normal, int inverse) {

    int col;            /* screen column                                    */
    int i = 0;          /* general purpose index variables                  */
    int width;          /* width of window                                  */
    int nu_opt;         /* number of options                                */
    int cur_opt;        /* current option (highlighted)                     */
    int ch;             /* character typed                                  */
    int found = FALSE;  /* flag to indicate option found(selected)          */
    int return_code;    /* return code                                      */
    Windows *pop_window;

    /* figure how many options there are                                    */

    nu_opt = 0;

    /* loop until empty string is found                                     */

    while (pop_menu[i++].name[0] != '\0');

    nu_opt = i - 1;        /* set nu_opt to the number of options found     */


    /* figure out the max width of the window, assume 0 to start            */

    width = 0; 

    for (i = 0; i < nu_opt; i++) { 

        if (strlen(pop_menu[i].name) > width) {

            width = strlen(pop_menu[i].name);

        }

    }

    /* make a window based on x, y parameters and the calculated width      */
    /* and height from the pop_menu struct                                  */

    pop_window = win_open("", nu_opt + 2, width + 2, y, x, FALSE);

    win_cursor(CURSOR_OFF);
										
    cur_opt = 0;                        /* first option                     */

    for (;;) {                          /* infinite loop                    */

        /* print menu option, highlight current option                      */

        col = 0;                        /* start at first column            */

        for (i = 0; i < nu_opt; i++) {  /* print each option                */

            /* hightlight current option                                    */

            if (i == cur_opt) 

                win_set_attr(pop_window, inverse);

            else win_set_attr(pop_window, normal);

            win_output(pop_window, col++, 0, pop_menu[i].name);

        }

        if (found) {                    /* selection found                  */

            if (*pop_menu[cur_opt].fun == NULL) {

                /* return the specified code if NULL                        */

                return_code = pop_menu[cur_opt].select_id;
                break;

            } else {

                /* a function was specified by pointer, call the            */
                /* function and get the return_code                         */

                return_code = (*pop_menu[cur_opt].fun)();

                if (return_code != 0) break;
                found = FALSE;

            }

        } else {

            /* read the keystrokes until a selection is made                */

            ch = win_getch(pop_window);

            if (ch == KEY_DOWN) {        /* move down                       */

                cur_opt++;    
                if (cur_opt >= nu_opt) cur_opt = 0;

            } else if (ch == KEY_UP) {    /* move up                        */

                cur_opt--;
                if (cur_opt < 0) cur_opt = nu_opt - 1;

            } else if (ch == KEY_ENTER) {

                found = TRUE;

            } else if (ch == KEY_EXIT) {

                return_code = 0;
                break;

            } else {

                /* do we have a letter?    scan each option for a match     */

				ch = toupper(ch);

                for (i = 0; i < nu_opt; i++) {

					if (pop_menu[i].speed_key == ch) {

                    	cur_opt = i;
                        found = TRUE;
						break;

                    }

                }

            }

        }

    }

    win_close(pop_window);

    return(return_code);

}

