
#include <string.h>
#include <ctype.h>

#include "windows.h"

extern char *strupr(char *);

int win_list_select(char *ptr[], int row, int col, int normal, int inverse) {

    int current_opt = 0;        /* highlighted current option               */
    int top_opt = 0;            /* first option in window                   */
    int offset = 0;             /* distance between top_opt and current_opt */
    int last_opt;               /* the last option that was hightlighted    */
    int height;                 /* height of the window                     */
    int width;                  /* width of the window                      */
    int last = 0;               /* last element in pointer array            */
    int i = 0, y;               /* general purpose                          */
    int ch;                     /* character typed                          */
    char search[BUFSIZ];        /* holds the speed search characters        */
    int redraw = TRUE;          /* flag which forces a redraw of window     */
    int done = FALSE;
    int rows;
    char temp[BUFSIZ];
    Windows *win;

    search[0] = '\0';

    /* find out how many options are in the NULL terminated array           */

    while(ptr[last++] != '\0');
    last -= 2;

    /* figure out the max width of the window, assume 0 to start            */

    width = 0; 

    for (i = 0; i <= last; i++) { 

        if (strlen(ptr[i]) > width) {

            width = strlen(ptr[i]);

        }

    }

    /* open the list select window                                          */

    rows = getmaxr(stdscr);

    if (last < (rows - row - 4)) 

        height = last + 1;

    else height = (rows - row - 4);

    win = win_open("", height + 3, width + 2, row, col, TRUE);

    win_set_scroll(win, TRUE);
    win_cursor(CURSOR_OFF);     /* turn cursor off                          */

    y = 1;

    /* begin the loop to process use selection                              */

    while (!done) {

        if (redraw) {           /* redraw the window contents               */

            win_set_attr(win, normal);
            win_erase(win);

            y = 0;

            for (i = top_opt; i < top_opt + height; i++) {

                if (ptr[i] == NULL) break;
                if (i == current_opt) 

                    win_set_attr(win, inverse);

                else win_set_attr(win, normal);

                win_output(win, y++, 0, ptr[i]);

            }

            redraw = FALSE;

        }

		win_show(win);
        ch = win_getch(win);            /* read a character from keyboard   */

        if ((ch >= 32) && (ch <= 126)){ /* make sure its valid              */

            /* here we search to find a match for speed search letters      */

            last_opt = current_opt;     /* save the hightlight position     */
            ch = toupper(ch);
            strcat(search, (char *)&ch);

            for (i = 0; i <= last; i++) {

                strcpy(temp, ptr[i]);
                strupr(temp);

                if (strstr(temp, search)) {

                    current_opt = i;
                    offset = current_opt - top_opt;
                    break;

                }

            }

            /* the following code moves the highlighter or requests a       */
            /* redraw as needed                                             */

            if ((current_opt > top_opt + height - 1) ||
                (current_opt < top_opt)) {

                top_opt = current_opt - offset;
                
                if (top_opt < 0) top_opt = 0;

                redraw = TRUE;

            } else {

                /* new option is within the displayed page. un-highlight    */
                /* the old position and redraw the highlight at the new     */
                /* position.                                                */

                win_set_attr(win, normal);
                win_output(win, last_opt - top_opt, 0, ptr[last_opt]);

                win_set_attr(win, inverse);
                win_output(win, current_opt - top_opt, 0, ptr[current_opt]);

            }

        } else {            /* not a printable character                    */

            search[0] = '\0';
            y = current_opt - top_opt;

            if (ch == KEY_DOWN) {

                if ((y < height) && (current_opt < last)) {

                    /* down arrow and not at bottom of window               */

                    win_set_attr(win, normal);
                    win_output(win, y, 0, ptr[current_opt]);

                    current_opt++;

                    win_set_attr(win, inverse);
                    win_output(win, y + 1, 0, ptr[current_opt]);

                } else if ((y == height) && (current_opt < last)) {

                    /* down arrow and at the bottom of the window           */

                    win_set_attr(win, normal);
                    win_output(win, y, 0, ptr[current_opt]);
                    win_scroll_up(win, 1);

                    current_opt++;
                    top_opt++;

                    win_set_attr(win, inverse);
                    win_output(win, y, 0, ptr[current_opt]);

                }

            } else if (ch == KEY_UP) {

                if (y > 0) {

                    /* up and not at top of window                          */

                    win_set_attr(win, normal);
                    win_output(win, y, 0, ptr[current_opt]);

                    current_opt--;

                    win_set_attr(win, inverse);
                    win_output(win, y - 1, 0, ptr[current_opt]);

                } else if ((y == 0) && (current_opt > 0)) {

                    /* up arrow and at the top of window                    */

                    win_set_attr(win, normal);
                    win_output(win, y, 0, ptr[current_opt]);

                    current_opt--;
                    top_opt--;

					win_scroll_dn(win, 1);
                    win_set_attr(win, inverse);
                    win_output(win, y - 1, 0, ptr[current_opt]);

                }

            } else if (ch == KEY_NPAGE) {

                if (top_opt != last) {

                    /* page down and not at the end of selection list       */

                    offset = current_opt - top_opt;
                    top_opt += height;
                    current_opt = top_opt + offset;

                    /* check for boundary errors                            */

                    if (top_opt > last) top_opt = last;
                    if (current_opt > last) current_opt = last;

                    redraw = TRUE;

                } 

            } else if (ch == KEY_PPAGE) {

                if (top_opt != 0) {

                    /* page up pressed and not at top of window             */

                    offset = current_opt - top_opt;
                    top_opt -= height;
                    current_opt = top_opt + offset;

                    /* check for boundary errors                            */

                    if (top_opt < 0) {

                        top_opt = 0;
                        current_opt = offset;

                    }

                    redraw = TRUE;

                }

            } else if (ch == KEY_ENTER) {

                done = TRUE;

            } else if (ch == KEY_EXIT) {

                current_opt = -1;
                done = TRUE;

            }

        }

    }

    win_close(win);
    return(current_opt);

}

