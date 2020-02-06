
#include <stdarg.h>
#include <ctype.h>
 
#include "windows.h"

/* args to newwin */

#define A_ROWS  12  /* # rows in window */
#define A_COLS  60  /* # cols in window */
#define A_ORGY  7   /* window has origin at ... */
#define A_ORGX  10  /* ... row A_ORGY, col A_ORGX */

/* user text area */

#define A_UROWS 8   /* # rows in message area */
#define A_UCOLS 50  /* # cols in message area */
#define A_UORGY 2   /* message are has origin at ... */
#define A_UORGX 5   /* ... row A_UORGY, col A_UORGX */
#define A_USLOP 50  /* extra room in buffer so we don't overflow */

/* size of local buffer for alerts() */

#define A_UBUFSIZE  (A_UROWS * A_UCOLS + 1 + A_USLOP)

static char *getword(char *buf, char **nxtp, int *lenp);
static int alerts(char *title, char *prompt, int sound, char *s);

/*-----------------------------------------------------------------------*/

int win_alert(char *title, char *prompt, int sound, char *fmt, ...) {

    va_list ap;
    char    buf[A_UBUFSIZE];

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    return alerts(title, prompt, sound, buf);

}

static char *getword(char *buf, char **nxtp, int *lenp) {

    char *cp;

    /* 
     * skip leading whitespace, except for newlines 
     */

    while (*buf && isspace(*buf)) {

        if (*buf == '\n') {

            *lenp = 0;
            *nxtp = buf + 1;
            return buf;

        } else buf++;

    }

    /*
     * collect the word, if there is one, and set the length
     */
    
    cp = buf;

    while (*cp && !isspace(*cp))
        cp++;

    *lenp = cp - buf;

    /* 
     * skip trailing whitespace, stopping for a newline
     */
            
    while (*cp && isspace(*cp)) {

        if (*cp == '\n')

            break;

        else cp++;

    }

    *nxtp = cp;

    return(*lenp ?  buf : 0);

}

static int alerts(char *title, char *prompt, int sound, char *s) {
    
    int uy = 0, ux = 0, wlen, key = 0;
    char ubuf[A_UROWS][A_UCOLS + 1], *word;
    Windows *awin;

    /* allocate space */
    
    if (!title)  title  = "Warning";
    if (!prompt) prompt = "Press ENTER to continue ...";

    if ((awin = win_open(title, A_ROWS, A_COLS, A_ORGY, A_ORGX, TRUE)) != NULL) {
    
/*        if (sound < 0) wblinkon(awin); */

        /*
         * set up the user text ...
         *
         * this is a word wrap routine
         *
         * it follows the same rules as the
         * word wrap routine in the Mac Toolbox
         * 
         * a "word" is any sequence of characters
         * that aren't whitespace.  a newline
         * causes an explicit wrap.
         */

        while ((word = getword(s, &s, &wlen))) {

            if (!wlen) {

                /* got a newline */

                if (uy >= (A_UROWS - 1)) break; /* truncate */

                ubuf[uy][ux] = 0;
                ux = 0;
                uy++;

            }  else {

                /* got a word */
                /* does it fit? */

                if (ux + wlen + (ux != 0) >= A_UCOLS) {

                    /* nope */

                    if (uy >= (A_UROWS - 1)) break; /* truncate */

                    ubuf[uy][ux] = 0;
                    ux = 0;
                    uy++;

                }     

                /* copy it into ubuf */            

                if (ux != 0)
                    ubuf[uy][ux++] = ' ';

                while (wlen-- > 0)
                    ubuf[uy][ux++] = *word++;

            }

        }

        ubuf[uy++][ux] = 0;

        while (uy < A_UROWS)
            ubuf[uy++][0] = 0;
        
        /* put user text on window */
    
        for (uy = 0; uy < A_UROWS; uy++)
            win_output(awin, uy + A_UORGY, A_UORGX, ubuf[uy]);

        win_output(awin, A_UORGY + A_UROWS + 2, A_UORGX, prompt);
        win_show(awin);

        /* pop up & get a key */
        
        while (sound-- > 0)
            beep();

        key = win_getch(awin);

        win_close(awin);

    }

    return(key);

}
    

