
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "windows.h"

extern int kb_matchkey(int *arr, int val);

static int okfunc(void) {

    return 1;

}

static int alpha(int ch) {


    return(isalpha(ch));

}

static int number(int ch) {

    return(isdigit(ch));

}

static int alpha_numeric(int ch) {

    return(isalnum(ch));

}

static int upper_case(int ch) {

    return(isupper(ch));

}

static int lower_case(int ch) {

    return(islower(ch));

}

static int numeric(int ch) {

    return((isdigit(ch) || ch == '+' || ch == '-' || ch == '.'));

}

static int boolean(int ch) {

    ch = toupper(ch);

    return((ch == 'Y' || ch == 'N' || ch == 'T' || 
            ch == 'F' || ch == '1' || ch == '0'));

}

static int times_(int ch) {

    return((isdigit(ch) || ch == ':'));

}

static int dates_(int ch) {

    return((isdigit(ch) || ch == '/'));

}

/*----------------------------------------------------------------------
 *
 *  EDITTXT -- main editing function
 *
 *  edittxt(win, buf, len, term, ins, skip, fix, zap, filter)
 *  WINDOW  *win;
 *  char    *buf;
 *  int     len;
 *  int     *term;
 *  int     ins,
 *          skip,
 *          fix,
 *          zap;
 *  int     (*filter)();
 *
 *  edit the field whose initial contents are contained in buf
 *  screen output is to window win
 *  the maximum length of the field contents is len
 *  term is string of ints which, when received from the
 *      keyboard, will terminate the edit
 *  ins tells whether insert mode can be turned on
 *  skip tells whether auto-field skipping should occur:
 *      (this is for data entry forms, and only works if fix,
 *      explained next, is set > 0)
 *      if you're at the end of the buffer and press space or
 *      you're at the beginning of the buffer and cursor left
 *      the edit terminates, returning TAB or BACKTAB
 *  fix tells how to handle trailing spaces:
 *      < 0 means truncate trailing spaces
 *      0 means leave them alone
 *      > 0 add spaces and keep field length at its maximum
 *  zap tells whether to erase the field if the first character typed
 *      is a printing character (useful for data entry forms)    
 *  filter is a function that takes 1 argument and returns non-zero
 *      if that character is ok to put into the text and zero if the
 *      character is illegal. the character passed will be a printing
 *      character.
 *
 *----------------------------------------------------------------------
 */

static int
edittxt(win, buf, len, term, ins, skip, fix, zap, filter)
Windows  *win;
char    *buf;
int     len;
int     *term;
int     ins,
        skip,
        fix,
        zap;
int     (*filter)();
{
    int c, 
        l = strlen(buf), 
        insmode = 0, 
        pos = 0, 
        y, x;

    char *cp = buf + l;

    if (fix > 0 && l < len) {
        while (l < len) {
            l++;
            *cp++ = ' ';
        }
        *cp = 0;
    } else if (fix < 0) {
        while (l > 0 && *(cp-1) == ' ') {
            l--;
            *cp-- = 0;
        }
    }

    getyx(win->inner, y, x);
    waddstr(win->inner, buf);
    wmove(win->inner, y, x);
    wrefresh(win->inner);

    while (kb_matchkey(term, c = win_getch(win)) == -1) {
	   
        if (skip && fix > 0) {
			
            if ((c == ' ' || c == KEY_RIGHT) && pos == len) {
			  
                return KEY_TAB;

            } else if ((c == '\b' || c == KEY_LEFT) && pos == 0) {
					
                return KEY_BTAB;

			}
		   
		}
	   
        if (isascii(c) && isprint(c) && (*filter)(c)) {
		   
            if (zap) {

                int y, x, f;

                while (pos > 0) {

                    pos--;
                    waddch(win->inner, '\b');

                }

                f = fix > 0 ? ' ' : 0;
                getyx(win->inner, y, x);

                while (l > 0) {

                    buf[--l] = f;
                    waddch(win->inner, ' ');

                }

                if (fix > 0) l = len;
                wmove(win->inner, y, x);
                zap = 0;

            }

            if (pos >= l) {
				
                if (l < len) {

                    buf[l++] = c;
                    buf[++pos] = 0;
                    waddch(win->inner, c);

                } else beep();

            } else if (insmode) {
					
                if (l < len || (l == len && buf[l-1] == ' ')) {

                    int n, y, x;

                    for (n = l - (l >= len); n > pos; n--)
                        buf[n] = buf[n - 1];

                    waddch(win->inner, buf[pos] = c);

                    if (l < len) buf[++l] = 0;
				   
                    getyx(win->inner, y, x);
                    waddstr(win->inner, buf+(++pos));
                    wmove(win->inner, y, x);

                } else beep();

            } else waddch(win->inner, buf[pos++] = c);
		   
            wrefresh(win->inner);
            zap = 0;
            continue;

        }

        switch (c) {
        case KEY_LEFT:
            if (pos > 0) {

                pos--;
                waddch(win->inner, '\b');

            } else beep();
            break;
		   
        case KEY_RIGHT:
            if (pos < l) {

                pos++;
                waddch(win->inner, winch(win->inner));

            } else beep();
            break;
		   
        case KEY_BEG:
            while (pos > 0) {

                pos--;
                waddch(win->inner, '\b');

            }
            break;

        case KEY_END:
            while (pos < l) {

                pos++;
                waddch(win->inner, winch(win->inner));

            }
            break;

        case KEY_IC:
            if (ins) {
				
                insmode = !insmode;

			} else beep();
            break;

        case KEY_DL:
            {
                int y, x, f;

                while (pos > 0) {

                    pos--;
                    waddch(win->inner, '\b');

                }

                f = fix > 0 ? ' ' : 0;
                getyx(win->inner, y, x);

                while (l > 0) {

                    buf[--l] = f;
                    waddch(win->inner, ' ');

                }

                if (fix > 0) l = len;
                wmove(win->inner, y, x);

            }
            break;

        case KEY_DC:
		case KEY_BACKSPACE:
/*            if (c == '\b') */
                if (pos > 0) {

                    pos--;
                    waddch(win->inner, '\b');

                } else {

                    beep();
                    break;

                }

            if (pos < l && l > 0) {

                int p, y, x;

                for (p = pos; p < l - 1; p++)
                    buf[p] = buf[p + 1];

                buf[p] = ' ';
                getyx(win->inner, y, x);
                waddstr(win->inner, buf + pos);
                wmove(win->inner, y, x);

                if (fix <= 0) {

                    buf[p] = 0;
                    l--;

                }

            } else beep();
            break;

        default:
            beep();
            break;

        }

        wrefresh(win->inner);
        zap = 0;

    }

    if (c == '\n') {
		
        while (pos++ < l) 
            waddch(win->inner, winch(win->inner));

	}
   
    return c;

}

int win_edt_str(Windows *win, int row, int col, char *buf, int len) {

    static int nl_esc[] = { KEY_ENTER, KEY_EXIT, KEY_ILLEGAL };

    wmove(win->inner, col, row);
    return edittxt(win, buf, len, nl_esc, 1, 0, -1, 0, okfunc);

}

int win_edt_fld(Windows *win, int row, int col, char *buf, int len, int type) {

    static int nl_esc[] = { KEY_ENTER, KEY_EXIT, KEY_NPAGE, 
                            KEY_PPAGE, KEY_REFRESH, KEY_UP,
                            KEY_DOWN, KEY_ILLEGAL };
    int term;

    wmove(win->inner, row, col);
    
    switch (type) {
        case EDT_ALPHA:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, alpha);
            break;

        case EDT_ALPHA_NUMERIC:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, alpha_numeric);
            break;

        case EDT_LOWER_CASE:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, lower_case);
            break;

        case EDT_UPPER_CASE:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, upper_case);
            break;

        case EDT_NUMERIC:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, numeric);
            break;

        case EDT_BOOLEAN:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, boolean);
            break;

        case EDT_TIME:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, times_);
            break;

        case EDT_DATE:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, dates_);
            break;

        case EDT_NUMBER:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, number);
            break;

        default:
            term = edittxt(win, buf, len, nl_esc, 1, 1, 1, 1, okfunc);
            break;

    }

    return(term);

}

