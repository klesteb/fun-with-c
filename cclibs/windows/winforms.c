
#include <string.h>

#include "windows.h"

int win_forms(Windows *win, struct form_struct form[]) {

    int number = 0;
    int i, done = FALSE;
	int row, col;
    int ext;
    char *temp;

    /* search for terminating zero in data structure                        */

    for (;;) {

        if (form[number++].row == 0) break;
        if (number == 1000) return(1);      /* count to high, no terminator */

    }

    number--;                       /* reset to correct value               */

    /* write the field prompts and highlight input area                     */

    for (i = 0; i < number; i++) {

        row = form[i].row;
        col = form[i].col;

        win_output(win, row, col, form[i].label);

        col = col + strlen(form[i].label) + 1;

        win_output(win, row, col, form[i].string);

    }

    /* at this point we have set up the screen, we now input the data       */

    i = 0;

    /* this is the main loop in which editing occurs                        */

    while (!done) {

        row = form[i].row;
        col = form[i].col;

        temp = form[i].string;

        col = col + strlen(form[i].label) + 1;

        /* loop until a valid field is read                                 */

        for (;;) {

            ext = win_edt_fld(win, row, col, temp, form[i].length, form[i].type);

            if (ext == KEY_ENTER) {

                form[i].string = temp;
                i++;
                if (i > (number - 1)) done = TRUE;
                break;

            } else if (ext == KEY_BTAB) {

                i--;
                if (i < 0) i = number - 1;
                break;

            } else if (ext == KEY_TAB) {

                i++;
                if (i > (number - 1)) i = 0;
                break;

            } else if (ext == KEY_REFRESH) {

                temp = form[i].string;
                win_output(win, row, col, form[i].string);

            } else if (ext == KEY_EXIT) {

                done = TRUE;
                break;

            } else if (ext == KEY_NPAGE) {

                done = TRUE;
                break;

            } else if (ext == KEY_PPAGE) {

                done = TRUE;
                break;

            }

        }

    }

    return(ext);

}

