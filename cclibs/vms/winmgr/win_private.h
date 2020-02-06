
/*----------------------------------------------------------------------*/
/* File: wm_private.h                                                   */
/* Date: 22-Jun-2000                                                    */
/* By  : Kevin Esteb                                                    */
/*                                                                      */
/* This file contains the definations for the global area in the        */
/* windowing routines. This hold state information.                     */
/*----------------------------------------------------------------------*/

#ifndef WM_PRIVATE_H
#define WM_PRIVATE_H 1

typedef struct {
    long pasteboard_id;             /* Virtual pasteboard               */
    long keyboard_id;               /* Virtual keyboard                 */
    long msg_display_id;            /* Virtual message area             */
    long title_display_id;          /* Virtual title area               */
    long keytable_id;               /* Future use                       */
    long cols;                      /* Number of columns in pasteboard  */
    long rows;                      /* Number of rows in pasteboard     */
    long dev;                       /* SMG device type                  */
    char help_file[256];            /* Help filename                    */
} win_r_initial;

extern win_r_initial init;          /* Global state of terminal screen. */
extern long win0;                   /* Display ID of background window. */

#endif

