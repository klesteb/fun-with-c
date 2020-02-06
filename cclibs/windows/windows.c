
#include <stdlib.h>
#include <string.h>

#include "windows.h"

/*-------------------------------------------------------------------------*/

void win_init(void) {

    initscr();
    noecho();
    crmode();

}

void win_quit(void) {

    endwin();

}

Windows *win_open(char *title, int numlines, int numcols, 
                  int begin_y, int begin_x, int delay) {

    Windows *win;

    if ((win = (Windows *)calloc(1, sizeof(Windows))) != NULL) {

        win->outer = newwin(numlines, numcols, begin_y, begin_x);
        win->inner = subwin(win->outer, numlines - 2, numcols - 2, begin_y + 1, begin_x + 1);
        win->delayed = delay;
        box(win->outer, ACS_VLINE, ACS_HLINE);

        if (title != NULL) {

            wmove(win->outer, 0, 2);
            wprintw(win->outer, title);

        }

        if (win->delayed == FALSE)
            wrefresh(win->outer);

        return(win);

    }

    return(NULL);

}

void win_close(Windows *win) {

    win_set_attr(win, A_NORMAL);
    
    werase(win->outer);
    werase(win->inner);
    delwin(win->inner);
    delwin(win->outer);

    free(win);

}

void win_show(Windows *win) {

    wrefresh(win->outer);
    wrefresh(win->inner);

}

void win_output(Windows *win, int row, int col, char *string) {

    int cols;
    int length;
    char *dummy;

    length = strlen(string);
    cols = getmaxc(win->inner);

    if ((length + col) > cols) length = cols - 1;

    dummy = strndup(string, length);

    wmove(win->inner, row, col);
    wprintw(win->inner, "%s", dummy);

    if (win->delayed == FALSE)
        wrefresh(win->inner);

    free(dummy);

}

void win_erase(Windows *win) {

    wclear(win->inner);

    if (win->delayed == FALSE)
        wrefresh(win->inner);

}

void win_clear(Windows *win, int start, int stop) {

    int x;

    for (x = start; x <= stop; x++) {

        wmove(win->inner, x, 0);
        wclrtoeol(win->inner);

    }

    if (win->delayed == FALSE)
        wrefresh(win->inner);

}

int win_getch(Windows *win) {

    int ch;
   
#if defined(__linux)
   
    keypad(win->inner, TRUE);
	ch = wgetch(win->inner);
	keypad(win->inner, FALSE);

	switch (ch) {
	 case 9:
	   ch = KEY_TAB;
	   break;
	   
	 case 10:
	   ch = KEY_ENTER;
	   break;
	   
	 default:
	   break;
	   
	}
   
#else

   ch = wgetch(win->inner);

#endif
   
    return(ch);

}

void win_set_echo(int flag) {

    if (flag) 

        echo();

    else noecho();

}

void win_set_scroll(Windows *win, int flag) {

    scrollok(win->inner, flag);

}

int win_get_scroll(Windows *win) {

    return(getscroll(win->inner));

}

void win_set_wrap(Windows *win, int flag) {

    wrapok(win->inner, flag);

}

int win_get_wrap(Windows *win) {

    return(getwrap(win->inner));

}

int win_move(Windows *win, int row, int col) {

    if (mvwin(win->outer, row, col) != ERR) {

        if (mvwin(win->inner, row + 1, col + 1) != ERR) {

            touchwin(win->inner);
            refresh();

            return(OK);

        }

    }

    return(ERR);

}

void win_set_attr(Windows *win, int attr) {

    switch (attr) {
        case A_NORMAL:
            wclrattr(win->inner, (_BOLD | _BLINK | _REVERSE | _UNDERLINE));
            break;

        case A_BOLD:
        case A_STANDOUT:
            wsetattr(win->inner, _BOLD);
            break;

        case A_UNDERLINE:
            wsetattr(win->inner, _UNDERLINE);
            break;

        case A_REVERSE:
            wsetattr(win->inner, _REVERSE);
            break;

        case A_BLINK:
            wsetattr(win->inner, _BLINK);
            break;

        default:
            break;

    }

}

int win_get_attr(Windows *win, int attr) {

    int result;

    switch (attr) {
        case A_NORMAL:
            result = getnorm(win->inner);
            break;

        case A_BOLD:
        case A_STANDOUT:
            result = getstand(win->inner);
            break;

        case A_UNDERLINE:
            result = getunderline(win->inner);
            break;

        case A_REVERSE:
            result = getreverse(win->inner);
            break;

        case A_BLINK:
            result = getblink(win->inner);
            break;

        default:
            result = FALSE;
            break;

    }

    return(result);

}

void win_goto(Windows *win, int row, int col) {

    wmove(win->inner, row, col);
    wrefresh(win->inner);

}

void win_cursor(int onoff) {

    if (onoff)

        cursoron();

    else cursoroff();

}

void win_scroll_up(Windows *win, int count) {

    int x;

    for (x = 0; x < count; x++) {

        scroll(win->inner);
        wclrtoeol(win->inner);

    }

    wrefresh(win->inner);

}

void win_scroll_dn(Windows *win, int count) {

    int x, row, col;

    getyx(win->inner, row, col);
    
    wmove(win->inner, 0, 0);

    for (x = 0; x < count; x++) {

        winsertln(win->inner);
        wclrtoeol(win->inner);

    }

    wrefresh(win->inner);

}

void win_insert(Windows *win, int row, int col, char *string) {

    int cols;
    int length;

    length = strlen(string);
    cols = getmaxc(win->inner);

    if (length > cols) length = cols;

    wmove(win->inner, row, col);
    winsertln(win->inner);
    wmove(win->inner, row, col);
    wprintw(win->inner, "%- *.s", length, string);

    if (win->delayed == FALSE)
        wrefresh(win->inner);

}

#ifdef __VMS

int vms_bright(long display_id) {

    long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_BOLD);

}

int vms_getattr(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb);

}

int vms_getnorm(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) != 0) lib$signal(stat);

    return(video_attrb == 0 ? TRUE : FALSE);

}

int vms_getstand(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_BOLD);

}

int vms_getreverse(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_REVERSE);

}

int vms_getblink(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_BLINK);

}

int vms_getunderline(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_UNDERLINE);

}

int vms_getinvisible(long display_id) {

    unsigned long stat, height, width, disp_attrb, video_attrb;
    
    stat = smg$get_display_attr(&display_id, &height, &width, 
                                &disp_attrb, &video_attrb, 0,0);
    if ((stat & 1) == 0) lib$signal(stat);

    return(video_attrb & SMG$M_INVISIBLE);

}

int vms_cursoron(void) {

    unsigned long stat, mode = SMG$M_CURSOR_ON;

    stat = smg$set_cursor_mode(&stdpb->_id, &mode);
    if ((stat & 1) == 0) lib$signal(stat);

    return(0);

}

int vms_cursoroff(void) {
    
    unsigned long stat, mode = SMG$M_CURSOR_OFF;

    stat = smg$set_cursor_mode(&stdpb->_id, &mode);
    if ((stat & 1) == 0) lib$signal(stat);

    return(0);

}

void vms_wprintw(WINDOW *win, char *fmt, ...) {

    va_list ap;
    char buff[100];

    va_start(ap, fmt);
    vsprintf(buff, fmt, ap);
    va_end(ap);

    waddstr(win, buff);

}
    
int vms_wgetch(WINDOW *win) {

/* The Vax C curses package dosen't return the arrow, edit or function     */
/* keys. This is the interface routines for the keyboard. It is bypassing  */
/* Vax Curses and accessing the smg$ functions directly. Why DEC didn't do */
/* this already is beyond me...                                            */

    unsigned long stat;
    unsigned short term;
    int key;
    long timeout = 60;

    stat = smg$read_keystroke(&stdkb->_id, &term, 0,0,0,0,0);
    if ((stat != SS$_NORMAL) && (stat != SMG$_EOF) && (stat != SS$_OPINCOMPL))
        if ((stat & 1) == 0) lib$signal(stat);

    switch (term) {
        case SMG$K_TRM_CR:
        case SMG$K_TRM_ENTER:
            key = KEY_ENTER;
            break;

        case SMG$K_TRM_DELETE:
            key = KEY_DC;
            break;

        case SMG$K_TRM_CTRLA:
            key = KEY_IC;
            break;

        case SMG$K_TRM_CTRLE:
            key = KEY_END;
            break;

        case SMG$K_TRM_CTRLH:
            key = KEY_BEG;
            break;

        case SMG$K_TRM_CTRLP:
            key = KEY_PRINT;
            break;

        case SMG$K_TRM_CTRLX:
        case SMG$K_TRM_CTRLU:
            key = KEY_DL;
            break;

        case SMG$K_TRM_CTRLW:
            key = KEY_REFRESH;
            break;

        case SMG$K_TRM_CTRLZ:
            key = KEY_EXIT;
            break;

        case SMG$K_TRM_UP:
            key = KEY_UP;
            break;

        case SMG$K_TRM_DOWN:
            key = KEY_DOWN;
            break;

        case SMG$K_TRM_LEFT:
            key = KEY_LEFT;
            break;

        case SMG$K_TRM_RIGHT:
            key = KEY_RIGHT;
            break;

        case SMG$K_TRM_FIND:
            key = KEY_BEG;
            break;

        case SMG$K_TRM_INSERT_HERE:
            key = KEY_IC;
            break;

        case SMG$K_TRM_REMOVE:
            key = KEY_SDC;
            break;

        case SMG$K_TRM_SELECT:
            key = KEY_END;
            break;

        case SMG$K_TRM_PREV_SCREEN:
            key = KEY_NPAGE;
            break;

        case SMG$K_TRM_NEXT_SCREEN:
            key = KEY_PPAGE;
            break;

        case SMG$K_TRM_F6:
            key = KEY_F0;
            break;

        case SMG$K_TRM_F7:
            key = KEY_F(1);
            break;

        case SMG$K_TRM_F8:
            key = KEY_F(2);
            break;

        case SMG$K_TRM_F9:
            key = KEY_F(3);
            break;

        case SMG$K_TRM_F10:
            key = KEY_F(4);
            break;

        case SMG$K_TRM_F11:
            key = KEY_F(5);
            break;

        case SMG$K_TRM_F12:
            key = KEY_F(6);
            break;

        case SMG$K_TRM_F13:
            key = KEY_F(7);
            break;

        case SMG$K_TRM_F14:
            key = KEY_F(8);
            break;

        case SMG$K_TRM_HELP:
            key = KEY_HELP;
            break;

        case SMG$K_TRM_DO:
            key = KEY_COMMAND;
            break;

        case SMG$K_TRM_F17:
            key = KEY_F(9);
            break;

        case SMG$K_TRM_F18:
            key = KEY_F(10);
            break;

        case SMG$K_TRM_F19:
            key = KEY_F(11);
            break;

        case SMG$K_TRM_F20:
            key = KEY_F(12);
            break;

        default:
            key = term;
    }        

    return(key);

}

#endif


