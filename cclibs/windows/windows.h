
#ifndef WINDOWS_H
#define WINDOWS_H 1

#if defined(__VMS)

#define __VMS_CURSES

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <lib$routines.h>
#include <smg$routines.h>

extern int vms_geattr(long);
extern int vms_getbright(long);
extern int vms_getnorm(long);
extern int vms_getstand(long);
extern int vms_getreverse(long);
extern int vms_getblink(long);
extern int vms_getunderline(long);
extern int vms_cursoron(void);
extern int vms_cursoroff(void);
extern int vms_wgetch(WINDOW *);
extern void vms_wprintw(WINDOW *, char *, ...);

#define getattr(win)        (vms_getattr((win)->_id))
#define getbright(win)      (vms_getbright((win)->_id))
#define getnorm(win)        (vms_getnorm((win)->_id))
#define getstand(win)       (vms_getstand((win)->_id))
#define getreverse(win)     (vms_getreverse((win)->_id))
#define getblink(win)       (vms_getblink((win)->_id))
#define getunderline(win)   (vms_getunderline((win)->_id))
#define cursoron()          (vms_cursoron())
#define cursoroff()         (vms_cursoroff())
#define wprintw             vms_wprintw
#define wgetch              vms_wgetch

#define getorg(win,y,x)     ((y)=getorgy(win),(x)=getorgx(win))
#define getorgx(win)        ((win)->_beg_x)
#define getorgy(win)        ((win)->_beg_y)
#define getmaxc(win)        ((win)->_max_x)
#define getmaxr(win)        ((win)->_max_y)
#define getmaxrc(win,r,c)   ((r)=getmaxr(win),(c)=getmaxc(win))
#define getscroll(win)      ((win)->_scroll)
#define getwrap(win)        ((win)->_wrap)
#define getbegyx(win,y,x)   (y = (win)->_beg_y, x = (win)->_beg_x)
#define getmaxyx(win,y,x)   (y = (win)->_max_y + 1, x = (win)->_max_x + 1)
#define beep()              (printf("\7"))

#define A_ATTRIBUTES    0xffffff00
#define A_NORMAL        0x00000000
#define A_STANDOUT      0x00010000
#define A_UNDERLINE     0x00020000
#define A_REVERSE       0x00040000
#define A_BLINK         0x00080000
#define A_DIM           0x00100000
#define A_BOLD          0x00200000
#define A_ALTCHARSET    0x00400000
#define A_INVIS         0x00800000
#define A_PROTECT       0x01000000
#define A_CHARTEXT      0x000000ff
#define A_COLOR         0x0000ff00
#define COLOR_PAIR(n)   (n << 8)
#define PAIR_NUMBER(a)  ((a & A_COLOR) >> 8)

#define ACS_HLINE           ('|')
#define ACS_VLINE           ('-')

/* Funny "characters" enabled for various special function keys for input */
/* Whether such a key exists depend if its definition is in the terminfo entry */

#define KEY_BREAK       0401            /* break key (unreliable) */
#define KEY_DOWN        0402            /* The four arrow keys ... */
#define KEY_UP          0403
#define KEY_LEFT        0404
#define KEY_RIGHT       0405            /* ... */
#define KEY_HOME        0406            /* Home key (upward+left arrow) */
#define KEY_BACKSPACE   0407            /* backspace (unreliable) */
#define KEY_F0          0410            /* Function keys.  Space for 64 */
#define KEY_F(n)        (KEY_F0+(n))    /* keys is reserved. */
#define KEY_DL          0510            /* Delete line */
#define KEY_IL          0511            /* Insert line */
#define KEY_DC          0512            /* Delete character */
#define KEY_IC          0513            /* Insert char or enter insert mode */
#define KEY_EIC         0514            /* Exit insert char mode */
#define KEY_CLEAR       0515            /* Clear screen */
#define KEY_EOS         0516            /* Clear to end of screen */
#define KEY_EOL         0517            /* Clear to end of line */
#define KEY_SF          0520            /* Scroll 1 line forward */
#define KEY_SR          0521            /* Scroll 1 line backwards (reverse) */
#define KEY_NPAGE       0522            /* Next page */
#define KEY_PPAGE       0523            /* Previous page */
#define KEY_STAB        0524            /* Set tab */
#define KEY_CTAB        0525            /* Clear tab */
#define KEY_CATAB       0526            /* Clear all tabs */
#define KEY_ENTER       0527            /* Enter or send (unreliable) */
#define KEY_SRESET      0530            /* soft (partial) reset (unreliable) */
#define KEY_RESET       0531            /* reset or hard reset (unreliable) */
#define KEY_PRINT       0532            /* print or copy */
#define KEY_LL          0533            /* home down or bottom (lower left) */

/* The keypad is arranged like this: */
/* a1    up    a3   */
/* left   b2  right  */
/* c1   down   c3   */

#define KEY_A1          0534    /* Upper left of keypad */
#define KEY_A3          0535    /* Upper right of keypad */
#define KEY_B2          0536    /* Center of keypad */
#define KEY_C1          0537    /* Lower left of keypad */
#define KEY_C3          0540    /* Lower right of keypad */
#define KEY_BTAB        0541    /* Back tab key */
#define KEY_BEG         0542    /* beg(inning) key */
#define KEY_CANCEL      0543    /* cancel key */
#define KEY_CLOSE       0544    /* close key */
#define KEY_COMMAND     0545    /* cmd (command) key */
#define KEY_COPY        0546    /* copy key */
#define KEY_CREATE      0547    /* create key */
#define KEY_END         0550    /* end key */
#define KEY_EXIT        0551    /* exit key */
#define KEY_FIND        0552    /* find key */
#define KEY_HELP        0553    /* help key */
#define KEY_MARK        0554    /* mark key */
#define KEY_MESSAGE     0555    /* message key */
#define KEY_MOVE        0556    /* move key */
#define KEY_NEXT        0557    /* next object key */
#define KEY_OPEN        0560    /* open key */
#define KEY_OPTIONS     0561    /* options key */
#define KEY_PREVIOUS    0562    /* previous object key */
#define KEY_REDO        0563    /* redo key */
#define KEY_REFERENCE   0564    /* ref(erence) key */
#define KEY_REFRESH     0565    /* refresh key */
#define KEY_REPLACE     0566    /* replace key */
#define KEY_RESTART     0567    /* restart key */
#define KEY_RESUME      0570    /* resume key */
#define KEY_SAVE        0571    /* save key */
#define KEY_SBEG        0572    /* shifted beginning key */
#define KEY_SCANCEL     0573    /* shifted cancel key */
#define KEY_SCOMMAND    0574    /* shifted command key */
#define KEY_SCOPY       0575    /* shifted copy key */
#define KEY_SCREATE     0576    /* shifted create key */
#define KEY_SDC         0577    /* shifted delete char key */
#define KEY_SDL         0600    /* shifted delete line key */
#define KEY_SELECT      0601    /* select key */
#define KEY_SEND        0602    /* shifted end key */
#define KEY_SEOL        0603    /* shifted clear line key */
#define KEY_SEXIT       0604    /* shifted exit key */
#define KEY_SFIND       0605    /* shifted find key */
#define KEY_SHELP       0606    /* shifted help key */
#define KEY_SHOME       0607    /* shifted home key */
#define KEY_SIC         0610    /* shifted input key */
#define KEY_SLEFT       0611    /* shifted left arrow key */
#define KEY_SMESSAGE    0612    /* shifted message key */
#define KEY_SMOVE       0613    /* shifted move key */
#define KEY_SNEXT       0614    /* shifted next key */
#define KEY_SOPTIONS    0615    /* shifted options key */
#define KEY_SPREVIOUS   0616    /* shifted prev key */
#define KEY_SPRINT      0617    /* shifted print key */
#define KEY_SREDO       0620    /* shifted redo key */
#define KEY_SREPLACE    0621    /* shifted replace key */
#define KEY_SRIGHT      0622    /* shifted right arrow */
#define KEY_SRSUME      0623    /* shifted resume key */
#define KEY_SSAVE       0624    /* shifted save key */
#define KEY_SSUSPEND    0625    /* shifted suspend key */
#define KEY_SUNDO       0626    /* shifted undo key */
#define KEY_SUSPEND     0627    /* suspend key */
#define KEY_UNDO        0630    /* undo key */
#define KEY_TAB         0775    /* tab key */
#define KEY_ILLEGAL     0776    /* unknown key */
#define KEY_MAX         0777    /* Maximum curses key */

#elif defined(__MSDOS__)

#include "curses.h"

#define getscroll(win)      ((win)->_scroll)
#define getwrap(win)        ((win)->_wrap)
#define getbegyx(win,y,x)   (y = (win)->_beg_y, x = (win)->_beg_x)
#define getmaxyx(win,y,x)   (y = (win)->_max_y + 1, x = (win)->_max_x + 1)
#define beep()              (printf("\7"))

#define A_ATTRIBUTES    0xffffff00
#define A_NORMAL        0x00000000
#define A_STANDOUT      0x00010000
#define A_UNDERLINE     0x00020000
#define A_REVERSE       0x00040000
#define A_BLINK         0x00080000
#define A_DIM           0x00100000
#define A_BOLD          0x00200000
#define A_ALTCHARSET    0x00400000
#define A_INVIS         0x00800000
#define A_PROTECT       0x01000000
#define A_CHARTEXT      0x000000ff
#define A_COLOR         0x0000ff00
#define COLOR_PAIR(n)   (n << 8)
#define PAIR_NUMBER(a)  ((a & A_COLOR) >> 8)

#define ACS_HLINE           (1)
#define ACS_VLINE           (1)

/* Funny "characters" enabled for various special function keys for input */
/* Whether such a key exists depend if its definition is in the terminfo entry */

#define KEY_BREAK       0401            /* break key (unreliable) */
#define KEY_DOWN        0402            /* The four arrow keys ... */
#define KEY_UP          0403
#define KEY_LEFT        0404
#define KEY_RIGHT       0405            /* ... */
#define KEY_HOME        0406            /* Home key (upward+left arrow) */
#define KEY_BACKSPACE   0407            /* backspace (unreliable) */
#define KEY_F0          0410            /* Function keys.  Space for 64 */
#define KEY_F(n)        (KEY_F0+(n))    /* keys is reserved. */
#define KEY_DL          0510            /* Delete line */
#define KEY_IL          0511            /* Insert line */
#define KEY_DC          0512            /* Delete character */
#define KEY_IC          0513            /* Insert char or enter insert mode */
#define KEY_EIC         0514            /* Exit insert char mode */
#define KEY_CLEAR       0515            /* Clear screen */
#define KEY_EOS         0516            /* Clear to end of screen */
#define KEY_EOL         0517            /* Clear to end of line */
#define KEY_SF          0520            /* Scroll 1 line forward */
#define KEY_SR          0521            /* Scroll 1 line backwards (reverse) */
#define KEY_NPAGE       0522            /* Next page */
#define KEY_PPAGE       0523            /* Previous page */
#define KEY_STAB        0524            /* Set tab */
#define KEY_CTAB        0525            /* Clear tab */
#define KEY_CATAB       0526            /* Clear all tabs */
#define KEY_ENTER       0527            /* Enter or send (unreliable) */
#define KEY_SRESET      0530            /* soft (partial) reset (unreliable) */
#define KEY_RESET       0531            /* reset or hard reset (unreliable) */
#define KEY_PRINT       0532            /* print or copy */
#define KEY_LL          0533            /* home down or bottom (lower left) */

/* The keypad is arranged like this: */
/* a1    up    a3   */
/* left   b2  right  */
/* c1   down   c3   */

#define KEY_A1          0534    /* Upper left of keypad */
#define KEY_A3          0535    /* Upper right of keypad */
#define KEY_B2          0536    /* Center of keypad */
#define KEY_C1          0537    /* Lower left of keypad */
#define KEY_C3          0540    /* Lower right of keypad */
#define KEY_BTAB        0541    /* Back tab key */
#define KEY_BEG         0542    /* beg(inning) key */
#define KEY_CANCEL      0543    /* cancel key */
#define KEY_CLOSE       0544    /* close key */
#define KEY_COMMAND     0545    /* cmd (command) key */
#define KEY_COPY        0546    /* copy key */
#define KEY_CREATE      0547    /* create key */
#define KEY_END         0550    /* end key */
#define KEY_EXIT        0551    /* exit key */
#define KEY_FIND        0552    /* find key */
#define KEY_HELP        0553    /* help key */
#define KEY_MARK        0554    /* mark key */
#define KEY_MESSAGE     0555    /* message key */
#define KEY_MOVE        0556    /* move key */
#define KEY_NEXT        0557    /* next object key */
#define KEY_OPEN        0560    /* open key */
#define KEY_OPTIONS     0561    /* options key */
#define KEY_PREVIOUS    0562    /* previous object key */
#define KEY_REDO        0563    /* redo key */
#define KEY_REFERENCE   0564    /* ref(erence) key */
#define KEY_REFRESH     0565    /* refresh key */
#define KEY_REPLACE     0566    /* replace key */
#define KEY_RESTART     0567    /* restart key */
#define KEY_RESUME      0570    /* resume key */
#define KEY_SAVE        0571    /* save key */
#define KEY_SBEG        0572    /* shifted beginning key */
#define KEY_SCANCEL     0573    /* shifted cancel key */
#define KEY_SCOMMAND    0574    /* shifted command key */
#define KEY_SCOPY       0575    /* shifted copy key */
#define KEY_SCREATE     0576    /* shifted create key */
#define KEY_SDC         0577    /* shifted delete char key */
#define KEY_SDL         0600    /* shifted delete line key */
#define KEY_SELECT      0601    /* select key */
#define KEY_SEND        0602    /* shifted end key */
#define KEY_SEOL        0603    /* shifted clear line key */
#define KEY_SEXIT       0604    /* shifted exit key */
#define KEY_SFIND       0605    /* shifted find key */
#define KEY_SHELP       0606    /* shifted help key */
#define KEY_SHOME       0607    /* shifted home key */
#define KEY_SIC         0610    /* shifted input key */
#define KEY_SLEFT       0611    /* shifted left arrow key */
#define KEY_SMESSAGE    0612    /* shifted message key */
#define KEY_SMOVE       0613    /* shifted move key */
#define KEY_SNEXT       0614    /* shifted next key */
#define KEY_SOPTIONS    0615    /* shifted options key */
#define KEY_SPREVIOUS   0616    /* shifted prev key */
#define KEY_SPRINT      0617    /* shifted print key */
#define KEY_SREDO       0620    /* shifted redo key */
#define KEY_SREPLACE    0621    /* shifted replace key */
#define KEY_SRIGHT      0622    /* shifted right arrow */
#define KEY_SRSUME      0623    /* shifted resume key */
#define KEY_SSAVE       0624    /* shifted save key */
#define KEY_SSUSPEND    0625    /* shifted suspend key */
#define KEY_SUNDO       0626    /* shifted undo key */
#define KEY_SUSPEND     0627    /* suspend key */
#define KEY_UNDO        0630    /* undo key */
#define KEY_TAB         0775    /* tab key */
#define KEY_ILLEGAL     0776    /* unknown key */
#define KEY_MAX         0777    /* Maximum curses key */

#elif defined(AMIGADOS)

#include "curses.h"

#else

/* Must be Linux! */

# include "ncurses.h"

#define getmaxc(win)        ((win)->_maxx)
#define getmaxr(win)        ((win)->_maxy)
#define getscroll(win)      ((win)->_scroll)
#define getwrap(win)        (1)
#define getnorm(win)        ((win)->_attrs == A_NORMAL ? (1) : (0))
#define getstand(win)       ((win)->_attrs == A_STANDOUT ? (1) : (0))
#define getunderline(win)   ((win)->_attrs == A_UNDERLINE ? (1) : (0))
#define getreverse(win)     ((win)->_attrs == A_REVERSE ? (1) : (0))
#define getblink(win)       ((win)->_attrs == A_BLINK ? (1) : (0))
#define cursoron()          (curs_set(1))
#define cursoroff()         (curs_set(0))

#define wrapok(win, flag)      
#define wclrattr(win, flag) wattroff((win),(flag))
#define wsetattr(win, flag) wattron((win),(flag))

#define _BOLD           A_BOLD
#define _BLINK          A_BLINK
#define _REVERSE        A_REVERSE
#define _UNDERLINE      A_UNDERLINE

#define KEY_TAB         0775    /* tab key */
#define KEY_ILLEGAL     0776    /* unknown key */

#endif

#define CURSOR_OFF        FALSE
#define CURSOR_ON         TRUE
#define MAX_BAR           10
#define MAIN_OPT          5
#define PD_SUB            4
#define PD_SUB_ROW        2

#define EDT_ANYTHING      0
#define EDT_ALPHA         1
#define EDT_ALPHA_NUMERIC 2
#define EDT_LOWER_CASE    3
#define EDT_UPPER_CASE    4
#define EDT_NUMERIC       5
#define EDT_BOOLEAN       6
#define EDT_TIME          7
#define EDT_DATE          8
#define EDT_NUMBER        9

typedef struct _Windows {
    WINDOW *inner;
    WINDOW *outer;
    int delayed;
} Windows;

struct pop_struct {
    char *name;                 /* the menu option                          */
    int (*fun)();               /* the pointer to function                  */
    int select_id;              /* the list-select return code              */
    char speed_key;             /* option speed select key                  */
};

struct bar_struct {
    char *name;                 /* the menu option                          */
    char *info;                 /* the info line appearing under options    */
    int (*fun)();               /* the pointer to function                  */
    int select_id;              /* the list-select return code              */
    char speed_key;             /* option speed select key                  */
};

struct pd_struct {
    char *main;                 /* option to appear in main menu            */
    char speed_key;             /* option speed select key                  */
    struct pop_struct sub[PD_SUB];
};

struct form_struct {
    int row;                    /* row for data input field                 */
    int col;                    /* col for data input field                 */
    char *label;                /* the prompt for the field                 */
    char *string;               /* pointer to the string to edit            */
    int length;                 /* the maximun length of the field          */
    int type;                   /* type of data to be inputed               */
};

Windows *win_open(char *title, int numrows, int numcols, int row, int col, int delay);
void win_init(void);
void win_quit(void);
void win_close(Windows *win);
void win_output(Windows *win, int row, int col, char *string);
void win_insert(Windows *win, int row, int col, char *string);
int  win_getch(Windows *win);
void win_erase(Windows *win);
void win_scroll_up(Windows *win, int count);
void win_scroll_dn(Windows *win, int count);
void win_clear(Windows *win, int start, int stop);
void win_set_echo(int flag);
void win_cursor(int onoff);
void win_set_scroll(Windows *win, int flag);
void win_set_wrap(Windows *win, int flag);
int  win_move(Windows *win, int row, int col);
int  win_get_wrap(Windows *win);
int  win_get_scroll(Windows *win);
int  win_alert(char *title, char *prompt, int sound, char *fmt, ...);
void win_show(Windows *win);
void win_goto(Windows *win, int row, int col);
void win_set_attr(Windows *win, int attr);
int  win_get_attr(Windows *win, int attr);
int  win_popup_menu(struct pop_struct pop_menu[], int row, int col, int normal, int inverse);
int  win_bar_menu(Windows *win, struct bar_struct bar_menu[], int normal, int inverse);
int  win_pull_down_menu(struct pd_struct pd_menu[], int normal, int inverse);
int  win_forms(Windows *win, struct form_struct form[]);
int  win_edt_str(Windows *win, int row, int col, char *buf, int len);
int  win_edt_fld(Windows *win, int row, int col, char *buf, int len, int type);
int  win_list_select(char *ptr[], int row, int col, int normal, int inverse);

#endif

