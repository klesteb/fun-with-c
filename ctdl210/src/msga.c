
/************************************************************************/
/*                msga.c                                                */
/*                                                                      */
/*    Message handling for Citadel bulletin board system                */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 20Jun18 KLE  Split into 2 files.                                     */
/* 83Mar03 CrT & SB   Various bug fixes...                              */
/* 83Feb27 CrT    Save private mail for sender as well as recipient.    */
/* 83Feb23        Various.  transmitFile() won't drop first char on WC... */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov05 CrT    Stream retrieval.  Handles messages longer than MAXTEXT.*/
/* 82Nov04 CrT    Revised disk format implemented.                      */
/* 82Nov03 CrT    Individual history begun.  General cleanup.           */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include "ctdl.h"
#include "protos.h"
#include "common.h"

/************************************************************************/
/*                contents                                              */
/*                                                                      */
/*    aideMessage()    saves auto message in Aide>                      */
/*    dGetWord()       reads a word off disk                            */
/*    dPrintf()        printf() that writes to disk                     */
/*    fakeFullCase()   converts uppercase message to mixed case         */
/*    findPerson()     load log record for named person                 */
/*    getWord()        gets one word from message buffer                */
/*    mAbort()         checks for user abort of typeout                 */
/*    makeMessage()    menu-level message-entry routine                 */
/*    mFormat()        formats a string to modem and console            */
/*    mPeek()          sysop debugging tool--shows ctdlmsg.sys          */
/*    mWCprintf()      special mprintf for WC transfers                 */
/*    noteLogMessage() enter message into log record                    */
/*    noteMessage()    enter message into current room                  */
/*    note2Message()   noteMessage() local                              */
/*    printMessage()   prints a message on modem & console              */
/*    pullIt()         sysop special message-removal routine            */
/*    putWord()        writes one word to modem & console               */
/*    showMessages()   menu-level show-roomful-of-messages fn           */
/*    unGetMsgChar()   return a char to getMsgChar()                    */
/************************************************************************/

/************************************************************************/
/*    aideMessage() saves auto message in Aide>                         */
/************************************************************************/
void aideMessage(char noteDeletedMessage) {

    long ourRoom = thisRoom;

    /* message is already set up in msgBuf.mbtext */

    putRoom(ourRoom, &roomBuf);
    getRoom(AIDEROOM, &roomBuf);

    strcpy(msgBuf.mbauth, "Citadel");
    msgBuf.mbto[0] = '\0';

    if (putMessage( /* uploading== */ FALSE)) {

        noteMessage(0, ERROR);

    }

    if (noteDeletedMessage) {

        note2Message(pulledMId, pulledMLoc);

    }

    putRoom(AIDEROOM, &roomBuf);
    noteRoom();
    getRoom(ourRoom, &roomBuf);

}

/************************************************************************/
/*    dGetWord() fetches one word from current message, off disk        */
/*    returns TRUE if more words follow, else FALSE                     */
/************************************************************************/
char dGetWord(char *dest, int lim) {

    unsigned char c;

    --lim;    /* play it safe */

    /* pick up any leading blanks: */

    for (c = getMsgChar(); c == ' ' && c && lim; c = getMsgChar()) {

        if (lim) { 

            *dest++ = c;   
            lim--; 

        }

    }

    /* step through word: */

    for (; c != ' ' && c && lim; c = getMsgChar()) {

        if (lim) { 

            *dest++ = c;   
            lim--; 

        }

    }

    /* trailing blanks: */

    for (; c == ' ' && c && lim;  c = getMsgChar()) {

        if (lim) { 

            *dest++ = c;   
            lim--; 

        }

    }

    if (c) unGetMsgChar(c);    /* took one too many    */

    *dest = '\0';        /* tie off string    */

    return c;

}

/************************************************************************/
/*    dPrintf() write from format+args to disk                          */
/************************************************************************/
void dPrintf(char *format, ...) {
#define MAXWORD 256    /* maximum length of a word */

    int n;
    va_list ap;
    char *s, string[MAXWORD];

    va_start(ap, format);
    n = vsnprintf(string, MAXWORD, format, ap);
    va_end(ap);

    s = string;
    while (*s) putMsgChar(*s++);

}

/************************************************************************/
/*    fakeFullCase() converts a message in uppercase-only to a          */
/*    reasonable mix.  It can't possibly make matters worse...          */
/*    Algorithm: First alphabetic after a period is uppercase, all      */
/*    others are lowercase, excepting pronoun "I" is a special case.    */
/*    We assume an imaginary period preceding the text.                 */
/************************************************************************/
void fakeFullCase(char *text) {

    char *c;
    char lastWasPeriod;
    char state;

    for (lastWasPeriod = TRUE, c = text; *c; c++) {

        if (*c != '.' && *c != '?' && *c != '!') {

            if (isalpha(*c)) {

                if (lastWasPeriod) {

                    *c = toupper(*c);

                } else {

                    *c = tolower(*c);

                }

                lastWasPeriod = FALSE;

            }

        } else {

            lastWasPeriod = TRUE;

        }

    }

    /* little state machine to search for ' i ': */
#define NUTHIN     0
#define FIRSTBLANK 1
#define BLANKI     2

    for (state = NUTHIN, c = text; *c;  c++) {

        switch (state) {
            case NUTHIN:
                if (isspace(*c)) state = FIRSTBLANK;
                else             state = NUTHIN;
                break;
            case FIRSTBLANK:
                if (*c == 'i') state = BLANKI;
                else           state = NUTHIN;
                break;
            case BLANKI:
                if (isspace(*c)) state = FIRSTBLANK;
                else             state = NUTHIN;

                if (!isalpha(*c)) *(c - 1) = 'I';
                break;
        }

    }

}

/************************************************************************/
/*    findPerson() loads log record for named person.                   */
/*    RETURNS: ERROR if not found, else log record #                    */
/************************************************************************/
int findPerson(char *name, struct logBuffer *lBuf) {

    int  h, i, foundIt, logNo;

    h = hash(name);

    for (foundIt = i = 0;  i < MAXLOGTAB && !foundIt; i++) {

        if (logTab[i].ltnmhash == h) {

            logNo = logTab[i].ltlogSlot;
            getLog(lBuf, logNo);

            if (strcasecmp(name, lBuf->lbname) == 0) {

                foundIt = TRUE;

            }

        }

    }

    return (foundIt) ? logNo : ERROR;

}

/************************************************************************/
/*    getWord() fetches one word from current message                   */
/************************************************************************/
int getWord(char *dest, char *source, int offset, int lim) {

    int i, j;

    /* skip leading blanks if any */

    for (i = 0; source[offset+i] == ' ' && i < lim; i++);

    /* step over word */

    for (; source[offset + i] != ' ' && i < lim && source[offset + i] != 0; i++);

    /* pick up any trailing blanks */

    for (; source[offset + i] == ' ' && i < lim; i++);

    /* copy word over */

    for (j = 0; j < i; j++) {

        dest[j] = source[offset + j];
        
    }

    dest[j] = 0;    /* null to tie off string */

    return(offset + i);

}

/************************************************************************/
/*    mAbort() returns TRUE if the user has aborted typeout             */
/*    Globals modified:    outFlag                                      */
/************************************************************************/
char mAbort(void) {

    char c, toReturn;

    /* Check for abort/pause from user */

    if (!BBSCharReady()) {

        toReturn = FALSE;

    } else {

        c = toupper(iChar());

        switch (c) {
            case XOFF:
            case 'P':                    /*    pause:          */
                c = iChar();                /* wait to resume */
                if (tolower(c) == 'd' && aide)  {
                    pullMessage = TRUE;
                }
                toReturn = FALSE;
                break;
            case 'J':                    /* jump paragraph:*/
                toReturn = FALSE;
                break;
            case 'N':                    /* next:          */
                toReturn = TRUE;
                break;
            case 'S':                    /* skip:          */
                toReturn = TRUE;
                break;
            default:
                toReturn = FALSE;
                break;
        }
        
    }

    return toReturn;

}

/************************************************************************/
/*    makeMessage is menu-level routine to enter a message              */
/*    Return: TRUE if message saved else FALSE                          */
/************************************************************************/
int makeMessage(char uploading) {
/* uploading - TRUE if message is coming via WC protocol    */

    char   allUpper, *pc, toReturn;
    struct logBuffer lBuf;
    int    logNo;

    toReturn = FALSE;
    logNo    = ERROR;/* not needed, but it's nice to initialize...    */

    if (thisRoom != MAILROOM) {

        msgBuf.mbto[0] = FALSE;

    } else {

        if (!loggedIn) {

            strcpy(msgBuf.mbto, "Sysop");
            putString(" (private mail to 'sysop')\n ");

        } else {

            getString("recipient", msgBuf.mbto, NAMESIZE);
            normalizeString(msgBuf.mbto);

        }

        logNo = findPerson(msgBuf.mbto, &lBuf);

        if ((logNo == ERROR) && (hash(msgBuf.mbto) != hash("Sysop"))) {

            putString("No '%s' known", msgBuf.mbto);
            return FALSE;

        }

    }

    strcpy(msgBuf.mbauth, logBuf.lbname);        /* record author*/

    if (getText("message", msgBuf.mbtext, MAXTEXT)) {

        for (pc = msgBuf.mbtext, allUpper = TRUE; *pc && allUpper;  pc++) {

            if (toupper(*pc) != *pc)  {

                allUpper = FALSE;

            }

        }

        if (allUpper) {

            fakeFullCase(msgBuf.mbtext);

        }

        if ((toReturn = putMessage(uploading)) == 0) {

            noteMessage(&lBuf, logNo);

        }

    }

    return toReturn;

}

/************************************************************************/
/*    mFormat() formats a string to modem and console                   */
/************************************************************************/
void mFormat(char *string) {
#define MAXWORD 256    /* maximum length of a word */

    char wordBuf[MAXWORD];
    int  i;

    for (i = 0; string[i];) {

        i = getWord(wordBuf, string, i, MAXWORD);
        putWord(wordBuf);
        if (mAbort()) return;

    }

}

/************************************************************************/
/*    mPeek() dumps a sector in message.buf.    sysop debugging tool    */
/************************************************************************/
void mPeek(void) {

    char peekBuf[SECTSIZE];
    int  col, row, s, offset;

    s = getNumber(" sector to dump", 0, maxMSector - 1);
    offset = s * SECTSIZE;

    lseek(msgfl, offset, SEEK_SET);
    read(msgfl, peekBuf, SECTSIZE);

    for (row = 0; row < 2; row++) {

        putString("\n ");

        for (col = 0; col < 64; col++) {
        
            oChar(visible(peekBuf[row * 64 + col]));

        }

    }

}


/************************************************************************/
/*    noteLogMessage() slots message into log record                    */
/************************************************************************/
void noteLogMessage(struct logBuffer *lBuf, long logNo) {

    int i;

    /* store into recipient's log record:                     */
    /* slide message pointers down to make room for this one: */

    for (i = 0; i < MAILSLOTS - 1; i++) {

        (*lBuf).lbslot[i] = (*lBuf).lbslot[i + 1];
        (*lBuf).lbId[i]   = (*lBuf).lbId[i + 1];

    }

    /* slot this message in:    */

    (*lBuf).lbId[MAILSLOTS - 1]   = newestLo;
    (*lBuf).lbslot[MAILSLOTS - 1] = catSector;

    putLog(lBuf, logNo);

}

/************************************************************************/
/*    noteMessage() slots message into current room                     */
/************************************************************************/
void noteMessage(struct logBuffer *lBuf, long logNo) {

    if (!++newestLo) ++newestHi;    /* 32-bit '++' by hand    */

    logBuf.lbvisit[0] = newestLo;

    if (thisRoom != MAILROOM) {

        note2Message(newestLo, catSector);

        /* write it to disk:        */

        putRoom(thisRoom, &roomBuf);
        noteRoom();

    } else {

        if (hash(msgBuf.mbto) != hash("Sysop"))  {

            if (logNo != thisLog)  {

                noteLogMessage(lBuf, logNo);        /* note in recipient    */

            }

            noteLogMessage(&logBuf, thisLog);        /* note in ourself        */
            fillMailRoom();                /* update room also     */

        } else {

            getRoom(AIDEROOM, &roomBuf);

            /* enter in Aide> room -- 'sysop' is special */

            note2Message(newestLo, catSector);

            /* write it to disk:        */

            putRoom(AIDEROOM, &roomBuf);
            noteRoom();
            getRoom(MAILROOM, &roomBuf);

            /* note in ourself if logged in: */

            if (loggedIn) {

                noteLogMessage(&logBuf, thisLog);

            }

            fillMailRoom();

        }

    }

    /* make message official:    */

    catSector = thisSector;
    catChar   = thisChar;

    setUp(FALSE);

}

/************************************************************************/
/*    note2Message() makes slot in current room... called by noteMess   */
/************************************************************************/
void note2Message(long id, long loc) {

    int i;

    /* store into current room:                               */
    /* slide message pointers down to make room for this one: */

    for (i = 0; i < MSGSPERRM - 1; i++) {

        roomBuf.vp.msg[i].rbmsgLoc = roomBuf.vp.msg[i + 1].rbmsgLoc;
        roomBuf.vp.msg[i].rbmsgNo  = roomBuf.vp.msg[i + 1].rbmsgNo;

    }

    /* slot this message in:        */

    roomBuf.vp.msg[MSGSPERRM - 1].rbmsgNo  = id;
    roomBuf.vp.msg[MSGSPERRM - 1].rbmsgLoc = loc;

}

/************************************************************************/
/*    printMessage() prints indicated message on modem & console        */
/************************************************************************/
void printMessage(long loc, long id) {
/* loc - sector in message.buf        */
/* id  - unique-for-some-time ID#     */

    char moreFollows;
    long hereHi, hereLo;

    startAt(loc, 0);

    do {

        getMessage(); 
        sscanf(msgBuf.mbId, "%ld %ld", &hereHi, &hereLo);
fprintf(stderr, "hereHi: %ld, hereLo: %ld, ld: %ld\n", hereHi, hereLo, id);

    } while ((hereLo != id) && (thisSector == loc));

    if (hereLo != id) {

        putString("?can't find message");
#ifdef XYZZY
        putString(" loc=%d, id=%u, mbIds=%s, here=%ld %ld\n",
               loc, id, msgBuf.mbId, &hereHi, &hereLo);
#endif

        return;

    }

    doCR();

    if (msgBuf.mbdate[0])  putString("   %s ",  msgBuf.mbdate);
    if (msgBuf.mbauth[0])  putString("from %s", msgBuf.mbauth);
    if (msgBuf.mboname[0]) putString(" @%s",    msgBuf.mboname);

    if (msgBuf.mbroom[0] &&
        strcmp(msgBuf.mbroom, roomBuf.rbname) != 0) {

        putString(" in %s>", msgBuf.mbroom);

    }

    if (msgBuf.mbto[0]) {

        putString(" to %s", msgBuf.mbto);

    }

    doCR();

    do {

        moreFollows = dGetWord(msgBuf.mbtext, 150);
        putWord(msgBuf.mbtext);

    } while (moreFollows && !mAbort());

    doCR();

}

/************************************************************************/
/*    pullIt() is a sysop special to remove a message from a room       */
/************************************************************************/
void pullIt(int m) {

    int i;

    /* confirm that we're removing the right one:    */

    printMessage(roomBuf.vp.msg[m].rbmsgLoc, roomBuf.vp.msg[m].rbmsgNo);

    if (!getYesNo("pull")) return;

    /* record vital statistics for possible insertion elsewhere: */

    pulledMLoc = roomBuf.vp.msg[m].rbmsgLoc;
    pulledMId  = roomBuf.vp.msg[m].rbmsgNo;

    if (thisRoom == AIDEROOM) return;

    /* return emptied slot: */

    for (i = m; i > 0; i--) {
        
        roomBuf.vp.msg[i].rbmsgLoc = roomBuf.vp.msg[i - 1].rbmsgLoc;
        roomBuf.vp.msg[i].rbmsgNo  = roomBuf.vp.msg[i - 1].rbmsgNo;

    }

    roomBuf.vp.msg[0].rbmsgNo = ERROR;   /* mark new slot at end as free */

    /* store revised room to disk before we forget...    */

    noteRoom();
    putRoom(thisRoom, &roomBuf);

    /* note in Aide>: */

    putString(msgBuf.mbtext, "Following message deleted by %s:", logBuf.lbname);
    aideMessage( /* noteDeletedMessage== */ TRUE);

}

/************************************************************************/
/*    putWord() writes one word to modem & console                      */
/************************************************************************/
void putWord(char *st) {

    char *s;
    int  newColumn;

    for (newColumn = crtColumn, s = st; *s; s++) {

        if (*s != TAB) ++newColumn;
        else while (++newColumn % 8);

    }
    
    if (newColumn > termWidth) doCR();

    for (;  *st;  st++) {

        if (*st != TAB) ++crtColumn;
        else while (++crtColumn % 8);

        /* worry about words longer than a line:    */

        if (crtColumn > termWidth) doCR();

        if (prevChar != NEWLINE || (*st > ' ')) {
            
            oChar(*st);
            
        } else {
            
            doCR();
            oChar(*st);

        }

    }

}

/************************************************************************/
/*    showMessages() is routine to print roomful of msgs                */
/************************************************************************/
void showMessages(char whichMess, char revOrder) {

    int i;
    int start, finish, increment;
    long lowLim, highLim, msgNo;

    setUp(FALSE);

    /* Allow for reverse retrieval: */

    if (!revOrder) {

        start     = 0;
        finish    = MSGSPERRM;
        increment = 1;

    } else {

        start     = (MSGSPERRM - 1);
        finish    = -1;
        increment = -1;

    }

    switch (whichMess) {
        case newOnly:
            lowLim  = logBuf.lbvisit[ logBuf.lbgen[thisRoom] & CALLMASK] + 1;
            highLim = newestLo;
            break;
        case oldAndNew:
            lowLim  = oldestLo;
            highLim = newestLo;
            break;
        case oldOnly:
            lowLim  = oldestLo;
            highLim = logBuf.lbvisit[ logBuf.lbgen[thisRoom] & CALLMASK];
            break;
    }

    /* stuff may have scrolled off system unseen, so: */
    /* was "if (lowLim < oldestLo)...", rigged for wraparound: */

    if ((oldestLo - lowLim) < 0x8000) {

        lowLim = oldestLo;

    }

    for (i = start; i != finish; i += increment) {

        /* "<" comparison with 64K wraparound in mind: */

        msgNo = roomBuf.vp.msg[i].rbmsgNo;

        if (((msgNo - lowLim) < 0x8000) && ((highLim - msgNo) < 0x8000)) {

            printMessage(roomBuf.vp.msg[i].rbmsgLoc, msgNo);

            /* Pull current message from room if flag set */

            if (pullMessage) {

                pullMessage = FALSE;
                pullIt(i);
                if (revOrder) i++;

            }

            if (thisRoom == MAILROOM && whichMess == newOnly && getYesNo("respond")) {

                if (makeMessage( /* uploading== */ FALSE)) i--;

            }

        }

    }

}

/************************************************************************/
/*    unGetMsgChar() returns (at most one) char to getMsgChar()    */
/************************************************************************/
void unGetMsgChar(unsigned char c) {

    GMCCache = c;

}

