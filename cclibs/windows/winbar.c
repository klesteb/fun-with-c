
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "windows.h"

int win_bar_menu(Windows *win, struct bar_struct bar_menu[], int normal, int inverse) {
								
    int x = 1;                  /* x screen location                        */
    int i;                      /* index variables                          */
    int nu_opt;                 /* number of options in menu                */
    int cur_opt;                /* current menu (highlighted option)        */
    int ch;                     /* typed character                          */
    int found = FALSE;          /* selection made (found) flag              */
    int return_code = 0;
	char blank[133];

	memset(blank, ' ', 132);
	blank[132] = '\0';

    win_cursor(CURSOR_OFF);             /* turn cursor off                  */
										
    /* figure out how many options there are                                */

    for (i = 0; i < MAX_BAR; i++) {

        if (bar_menu[i].name[0] == '\0') {

            nu_opt = i;
            break;

        }

    }

    cur_opt = 0;
										
    for (;;) {                          /* infinite loop                    */

        x = 1;

        for (i = 0; i < nu_opt; i++) {  /* print all options                */

            if (i == cur_opt) 

                win_set_attr(win, inverse);

            else win_set_attr(win, normal);

            win_output(win, 0, x, bar_menu[i].name);

            x = x + strlen(bar_menu[i].name) + 3;

        }

        if (bar_menu[cur_opt].info[0] != '\0') { 

			/* is there info?                								*/

            win_set_attr(win, A_NORMAL);
			win_output(win, 1, 1, blank);
            win_output(win, 1, 1, bar_menu[cur_opt].info);

        }

        if (found) {                    /* selection was made               */

            if (bar_menu[cur_opt].fun == NULL) {

				/* return the specified code if NULL						*/

                return_code = bar_menu[cur_opt].select_id;
				break;

            } else {

                return_code = (*bar_menu[cur_opt].fun)();

				if (return_code != 0) break;
				found = FALSE;

            }

        } else {

			/* read the keystrokes until a selection is made				*/

            ch = win_getch(win);

            if (ch == KEY_RIGHT) {

                cur_opt++;
                if (cur_opt >= nu_opt) cur_opt = 0;

            } else if (ch == KEY_LEFT) {

                cur_opt--;
                if (cur_opt < 0) cur_opt = nu_opt - 1;

            } else if (ch == KEY_ENTER) {

                found = TRUE;

            } else if (ch == KEY_EXIT) {

                return_code = 0;
                break;

            } else {

				/* do we have a letter? scan each option for a match        */

                ch = toupper(ch);

                for (i = 0; i < nu_opt; i++) {

                    if (bar_menu[i].speed_key == ch) {

                        cur_opt = i;
                        found = TRUE;
                        break;

                    }

                }

            }

        }

    }
        
    return(return_code);

}

