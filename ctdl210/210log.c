
/************************************************************************/
/*                log.c                                                 */
/*                                                                      */
/*       userlog code for  Citadel bulletin board system                */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 83Feb27 CrT    Fixed login-in-Mail> bug.                             */
/* 83Feb26 CrT    Limited # new messages for new users.                 */
/* 83Feb18 CrT    Null pw problem fixed.                                */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov03 CrT    Began local history file & general V1.2 cleanup       */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <ncurses.h>
#include <errno.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.h"

/************************************************************************/
/*                  contents                                            */
/*                                                                      */
/*    getLog()      loads requested userlog record                      */
/*    login()       is menu-level routine to log caller in              */
/*    logInit()     builds the RAM index to userlog.buf                 */
/*    newPW()       is menu-level routine to change a PW                */
/*    newUser()     menu-level routine to log a new caller              */
/*    noteLog()     enters a userlog record into RAM index              */
/*    putLog()      stores a logBuffer into citadel.log                 */
/*    PWSlot()      returns userlog.buf slot password is in             */
/*    slideLTab()   support routine for sorting logTab                  */
/*    sortLog()     sort userlog by time since last call                */
/*    terminate()   menu-level routine to exit system                   */
/*    userlog()     sysop special to display userlog                    */
/*    zapLogFile()  erases & re-initializes userlog.buf                 */
/************************************************************************/


/************************************************************************/
/*    login() is the menu-level routine to log someone in        */
/************************************************************************/
void login(char *password) {
/* password - TRUE if parameters follow    */

    int foundIt, ltentry;

    foundIt = ((ltentry = PWSlot(password)) != ERROR);

    if (foundIt && *password) {

        /* recite caller's name, etc:     */

        putString(" %s\n", logBuf.lbname);

        /* update userlog entries: */

        loggedIn = TRUE;
        setUp(TRUE);

        showMessages(newOnly, FALSE);

        listRooms(/* doDull== */ !expert);

        if ((logBuf.lbId[MAILSLOTS - 1] -
            (logBuf.lbvisit[logBuf.lbgen[MAILROOM] & CALLMASK] + 1) < 0x8000) &&
            logBuf.lbId[MAILSLOTS-1] - oldestLo < 0x8000 &&
            thisRoom != MAILROOM) {

            putString("\n  * You have private mail in Mail> *\n ");

        }

    } else {

        /* discourage password-guessing: */

        if (strlen(password) > 1) usleep(2000);

        if (!unlogLoginOk) {

            putString(" No record -- leave message to 'sysop' in Mail>\n ");

        } else if (getYesNo(" No record: Enter as new user")) {

            newUser();

        }

    }

}

/************************************************************************/
/*    logInit() indexes userlog.buf                                     */
/************************************************************************/
void logInit(void) {

    int i;
    int count;

    count = 0;

    /* clear logTab */

    for (i = 0; i < MAXLOGTAB; i++) {

        logTab[i].ltnewest = ERROR;

    }

    /* load logTab: */

    for (thisLog = 0; thisLog < MAXLOGTAB; thisLog++) {

        putString("log#%d\n", thisLog);
        getLog(&logBuf, thisLog);

        /* count valid entries:         */

        if (logBuf.lbvisit[0] != ERROR) count++;

        /* copy relevant info into index:   */

        logTab[thisLog].ltnewest = logBuf.lbvisit[0];
        logTab[thisLog].ltlogSlot= thisLog;
        logTab[thisLog].ltnmhash = hash(logBuf.lbname);
        logTab[thisLog].ltpwhash = hash(logBuf.lbpw);

    }

    putString(" logInit--%d valid log entries\n", count);
    sortLog();

}

/************************************************************************/
/*    newPW() is menu-level routine to change one's password            */
/*    since some Citadel nodes run in public locations, we avoid        */
/*    displaying passwords on the console.                              */
/************************************************************************/
void newPW(void) {
    
    char oldPw[NAMESIZE];
    char pw[NAMESIZE];
    int goodPW;

    /* save password so we can find current user again: */

    strcpy(oldPw, logBuf.lbpw);
    storeLog();

    do {

        getString(" new password", pw, NAMESIZE);
        normalizeString(pw);

        /* check that PW isn't already claimed: */
        goodPW = (PWSlot(pw) == ERROR && strlen(pw) >= 2);

        if (!goodPW) putString("\n Poor password\n ");

    } while ((strlen(pw) < 2 || !goodPW ));

    doCR();

    PWSlot(oldPw);            /* reload old log entry         */
    pw[NAMESIZE - 1] = 0x00;  /* insure against loss of carrier:  */

    if (goodPW && strlen(pw) > 1) {    /* accept new PW:            */

        strcpy(logBuf.lbpw, pw);
        logTab[0].ltpwhash = hash(pw);

    }

    putString("\n %s\n pw: ", logBuf.lbname);
    putString("%s\n ", logBuf.lbpw);

    doCR();

}

/************************************************************************/
/*    newUser() prompts for name and password                           */
/************************************************************************/
void newUser(void) {
    
    char    fullnm[NAMESIZE];
    char    pw[NAMESIZE];
    int     good, g, h, i, ourSlot;
    unsigned    low;


    configure();    /* make sure new users configure reasonably    */

    if (!expert)   tutorial("password.blb");

    do {

        /* get name and check for uniqueness... */

        do {

            getString(" Name", fullnm, NAMESIZE);
            normalizeString(fullnm);
            h = hash(fullnm);

            for (i = 0, good = TRUE; i < MAXLOGTAB && good; i++) {
                
                if (h == logTab[i].ltnmhash) good = FALSE;

            }

            if (!h || h == hash("Citadel") || h == hash("Sysop")) {

                good = FALSE;

            }
            
            /* lie sometimes -- hash collision !=> name collision */
            
            if (!good) putString("We already have a %s\n", fullnm);
            
        } while (!good);

        /* get password and check for uniqueness...    */

        do {

            getString(" password", pw, NAMESIZE);
            normalizeString(pw);
            h = hash(pw);

            for (i = 0, good = strlen(pw) > 1;  i < MAXLOGTAB && good; i++) {

                if (h == logTab[i].ltpwhash) good = FALSE;

            }

            if (!h) good = FALSE;
            if (!good) {
                putString("\n Poor password\n ");
            }

        } while( !good);

        putString("\n nm: %s", fullnm);
        putString("\n pw: ");
        putString("%s\n ", pw);

    } while (!getYesNo("OK"));

    /* kick least recent caller out of userlog and claim entry:    */

    ourSlot = logTab[MAXLOGTAB - 1].ltlogSlot;
    slideLTab(0, MAXLOGTAB -1 );
    logTab[0].ltlogSlot = ourSlot;
    getLog(&logBuf, ourSlot);

    /* copy info into record:    */

    strcpy(logBuf.lbname, fullnm);
    strcpy(logBuf.lbpw, pw);

    low = newestLo - 50;
    if (oldestLo - low < 0x8000)   low = oldestLo;

    for (i = 1; i < MAXVISIT; i++) {

        logBuf.lbvisit[i]= low;

    }
        
    logBuf.lbvisit[0]= newestLo;
    logBuf.lbvisit[(MAXVISIT - 1)]= oldestLo;

    /* initialize rest of record:    */

    for (i = 0; i < MAXROOMS; i++) {

        if (roomTab[i].rtflags & PUBLIC) {

            g = (roomTab[i].rtgen);
            logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

        } else {

            /* set to one less */

            g = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
            logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT-1);

        }

    }

    for (i = 0; i < MAILSLOTS; i++) {

        logBuf.lbslot[i] = 0;
        logBuf.lbId[i]   = oldestLo - 1;

    }

    /* fill in logTab entries    */

    logTab[0].ltpwhash  = hash(pw);
    logTab[0].ltnmhash  = hash(fullnm);
    logTab[0].ltlogSlot = thisLog;
    logTab[0].ltnewest  = logBuf.lbvisit[0];

    /* special kludge for Mail> room, to signal no new mail:   */

    roomTab[MAILROOM].rtlastMessage = logBuf.lbId[MAILSLOTS - 1];

    loggedIn = TRUE;

    storeLog();

    listRooms(/* doDull== */ !expert);

}

/************************************************************************/
/*    noteLog() notes logTab entry in RAM buffer in master index        */
/************************************************************************/
void noteLog(void) {

    int i, slot;

    /* figure out who it belongs between:    */

    for (i = 0; logTab[i].ltnewest > logBuf.lbvisit[0];  i++);

    /* note location and open it up:        */

    slot = i;
    slideLTab(slot, MAXLOGTAB - 1);

    /* insert new record */

    logTab[slot].ltnewest  = logBuf.lbvisit[0];
    logTab[slot].ltlogSlot = thisLog;
    logTab[slot].ltpwhash  = hash(logBuf.lbpw);
    logTab[slot].ltnmhash  = hash(logBuf.lbname);

}

/************************************************************************/
/*    PWSlot() returns userlog.buf slot password is in, else ERROR      */
/*    NB: we also leave the record for the user in logBuf.              */
/************************************************************************/
int PWSlot(char *pw) {

    int  h, i;
    int  foundIt, ourSlot;

    h = hash(pw);

    /* Check all passwords in memory: */

    for (i = 0, foundIt = FALSE; !foundIt && i < MAXLOGTAB; i++) {

        /* check for password match here */

        /* If password matches, check full password                   */
        /* with current newUser code, password hash collisions should */
        /* not be possible... but this is upward compatable & cheap   */

        if (logTab[i].ltpwhash == h) {

            ourSlot = logTab[i].ltlogSlot;
            getLog(&logBuf, ourSlot);

            if (strcasecmp(pw, logBuf.lbpw) == 0) {

                /* found a complete match */

                thisSlot = i;
                foundIt  = TRUE;

            }

        }

    }

    return (foundIt) ? thisSlot : ERROR;

}

/************************************************************************/
/*    slideLTab() slides bottom N lots in logTab down.  For sorting.    */
/************************************************************************/
void slideLTab(int slot, int last) {

    int i;

    /* open slot up: (movmem isn't guaranteed on overlaps) */

    for (i = last - 1; i >= slot; i--) {
        
        memmove(&logTab[i], &logTab[i + 1], sizeLTentry);

    }

}

/************************************************************************/
/* sortLog ShellSorts userlog by time of last call                      */
/************************************************************************/
void sortLog(void ) {
    
#define TSIZE 16
    char *temp[TSIZE];
    int finis, i, intCount, step;

    putString("sortLog...\n");

    if (sizeLTentry > TSIZE) {

        putString("!!!increase TSIZE in sortLog to %d\n", sizeLTentry);

    }

    intCount = 0;

    for (finis = FALSE, step = MAXLOGTAB >> 1; !finis || step>1; ) {

        if (finis) {

            step = step / 3 + 1;
            finis = FALSE;

        }

        finis = TRUE;

        putString("stepsize=%d\n", step);

        for(i = step; i < MAXLOGTAB; i++) {

            if (logTab[i - step].ltnewest < logTab[i].ltnewest) {

                intCount++;
                finis = FALSE;

                /* interchange two entries */

                memmove(&logTab[i-step], temp, sizeLTentry);
                memmove(&logTab[i], &logTab[i-step], sizeLTentry);
                memmove(temp, &logTab[i], sizeLTentry);

            }

        }

    }

    putString("sortLog: %d interchanges\n", intCount);

}

/************************************************************************/
/*    storeLog() stores the current log record.                         */
/************************************************************************/
void storeLog(void) {

    logTab[0].ltnewest = newestLo;

    logBuf.lbvisit[0] = newestLo;
    logBuf.lbwidth    = termWidth;
    logBuf.lbnulls    = termNulls;
    logBuf.lbflags    = expert | termUpper | termLF | termTab | aide;

    putLog(&logBuf, thisLog);

}

/************************************************************************/
/*    terminate() is menu-level routine to exit system        */
/************************************************************************/
void terminate(char discon) {

    /* 1.  parameter <discon> is TRUE or FALSE.          */
    /* 2.  if <discon> is TRUE, breaks modem connection  */
    /*     or switches whichIO from CONSOLE to MODEM,    */
    /*     as appropriate.                               */
    /* 3.  modifies externs: struct logBuf,              */
    /*             struct logTab                         */
    /* 4.  returns no values                             */
    /*          modified    dvm 9-82                     */

    if (discon) {

        exitToCpm = TRUE;

    }

    putString(" %s logged out\n\n", logBuf.lbname);

    logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;

    storeLog();
    loggedIn = FALSE;
    setUp(TRUE);

}

/************************************************************************/
/*    getLog() loads requested log record into RAM buffer               */
/************************************************************************/
void getLog(struct logBuffer *lBuf, int n) {

    int recsize = sizeof(struct logBuffer);
    int offset = n * recsize;

    thisLog = n;

    errno = 0;
    if ((lseek(logfl, offset, SEEK_SET)) < 0) {

        putString(" ?getLog(): seek failed, reason: %d\n", errno);

    }

    errno = 0;
    if (read(logfl, lBuf, recsize) < 0) {

        putString(" ?getLog(): read failed, reason: %d\n", errno);

    }

    crypte(lBuf, recsize, n);    /* decode buffer    */

}

/************************************************************************/
/*    putLog() stores given log record into ctdllog.sys                 */
/************************************************************************/
void putLog(struct logBuffer *buf, int n) {

    int recsize = sizeof(struct logBuffer);
    int offset = n * recsize;

    crypte(buf, recsize, n);    /* encode buffer    */

    errno = 0;
    if ((lseek(logfl, offset, SEEK_SET)) < 0) {

        putString(" ?putLog(): seek failed, reason: %d\n", errno);

    }

    errno = 0;
    if (write(logfl, buf, recsize) < 0) {

        putString(" ?putLog(): write failed, reason: %d\n", errno);

    }

    crypte(buf, recsize, n);    /* decode buffer    */

}

/************************************************************************/
/*    zapLogFile() erases & re-initializes userlog.buf                  */
/************************************************************************/
void zapLogFile(void) {

    int  i;

    if (!getYesNo("\nWipe out log file")) return;

    /* clear RAM buffer out:            */

    for (i = 0; i < MAXVISIT; i++) {

        logBuf.lbvisit[0] = 0;

    }

    for (i = 0; i < MAILSLOTS; i++) {

        logBuf.lbslot[i] = 0;
        logBuf.lbId[i]   = 0;

    }

    for (i = 0; i < NAMESIZE; i++) {

        logBuf.lbname[i] = 0;
        logBuf.lbpw[i]   = 0;

    }

    /* write empty buffer all over file;    */

    for (i = 0; i < MAXLOGTAB; i++) {

        putLog(&logBuf, i);

    }

}

