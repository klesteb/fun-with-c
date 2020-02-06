
/*----------------------------------------------------------------------*/
/* File: win_defines.h                                                   */
/* Date: 22-Jun-2000                                                    */
/* By  : Kevin Esteb                                                    */
/*                                                                      */
/* This header file is for defining the smg window manager              */
/* routines along with there associated constants and structures.       */
/*----------------------------------------------------------------------*/

#ifndef WIN_DEFINES_H
#define WIN_DEFINES_H 1

typedef struct {
    long display_id;                /* Virtual display area.            */
    long row;                       /* Starting row.                    */
    long col;                       /* Starting column.                 */
    long width;                     /* Width of display.                */
    long length;                    /* Length of display.               */
    long delay;                     /* Delay positioning display area.  */
    char *banner;                   /* Display area banner name.        */
} win_r_window;

typedef struct {
    long code;                      /* Event code (defined below).      */
    long term;                      /* Event action code.               */
} win_r_event;
                                    
typedef struct {
    long row;                       /* Row in window.                   */
    long col;                       /* Column in window.                */
    long min_length;                /* Minimum length of input.         */
    long attrib;                    /* Display attributes.              */
    long data_type;                 /* Data type (defined below).       */
    char *fmt;                      /* Data entry format.               */
    char *help_topic;               /* Topic for help request.          */
    char *help_line;                /* Line for help prompt.            */
    win_r_event event;              /* Event action.                    */
} win_r_entry;

typedef struct {
    win_r_entry lin;                /* Entry record.                    */
    long external;                  /* Perform an external routine.     */
    long display_only;              /* Display this line only.          */
    long prompt_len;                /* Length of prompt.                */
    char *prompt;                   /* Prompt for field input.          */
    char *entry;                    /* Entry to display on form.        */
} win_r_form;

/* Event codes - normal key strokes.                                    */

#define win_k_arrow     300
#define win_k_exit      301
#define win_k_edit      302
#define win_k_function  303
#define win_k_gold      304
#define win_k_help      305
#define win_k_key       306
#define win_k_keypad    307
#define win_k_menu      308
#define win_k_normal    309
#define win_k_commit    310
#define win_k_print_screen  311
#define win_k_extern    312

/* Event codes - Gold key strokes.                                      */

#define win_k_gold_arrow     320
#define win_k_gold_exit      321
#define win_k_gold_edit      322
#define win_k_gold_function  323
#define win_k_gold_gold      324
#define win_k_gold_help      325
#define win_k_gold_key       326
#define win_k_gold_keypad    327
#define win_k_gold_menu      328
#define win_k_gold_normal    329

/* Valid types for data entry.                                          */

#define win_t_single         0
#define win_t_alpha          2      /* Alpha (A-Z, a-z, " ").           */
#define win_t_alpha_numeric  4      /* Alpha numeric (A-Z," ", 0-9).    */
#define win_t_anything       16     /* Anything.                        */
#define win_t_invisible      32     /* Anything invisible.              */
#define win_t_lower_case     64     /* Lower case alpha only.           */
#define win_t_numeric        128    /* Numeric.                         */
#define win_t_snumeric       256    /* Signed numeric.                  */
#define win_t_date           512    /* Date (mmddyy - mm/dd/yy          */
#define win_t_time           1024   /* Time (hh:mm).                    */
#define win_t_boolean        2048   /* Yes/No - True/False.             */

/* Display attributes.                                                  */

#define win_a_normal      0         /* Display line as normally.        */
#define win_a_blink       2         /* Display line as blinking.        */
#define win_a_bold        4         /* Display line as bolded.          */
#define win_a_reverse     16        /* Display line in reverse video.   */
#define win_a_underline   32        /* Display line as underlined.      */
#define win_a_invisible   64        /* Display line as invisible.       */

/* Window scrolling nemonics.                                           */

#define win_s_line_up    0          /* Scroll one up.                   */
#define win_s_line_down  1          /* Scroll one line down.            */
#define win_s_page_up    2          /* Scroll one window full, up.      */
#define win_s_page_down  3          /* Scroll one window full, down.    */
#define win_s_left       4          /* Shift window contents left.      */
#define win_s_right      5          /* Shift window contents right.     */

/* Window manager macros.                                               */

#define SET_WIN(x,a,b,c,d,e) {(x).row=(a);\
                              (x).col=(b);\
                              (x).width=(c);\
                              (x).length=(d);\
                              (x).banner=(e);}

/* Window manager prototypes.                                           */

void win_close_screen(void);
void win_open_screen(char *, int);
void win_open_window(win_r_window *);
void win_close_window(win_r_window *);
void win_clear_window(win_r_window *);
void win_refresh_window(win_r_window *);

void win_message(char *, int);
void win_display_broadcast_message(void);
void win_gotoxy(long, long, win_r_window *);
void win_keyboard_monitor(win_r_event *, long);
void win_process_terminator(word, win_r_event *);
void win_prompt(char *, long, long, long, long *);

#endif

