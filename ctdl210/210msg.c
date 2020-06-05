
/************************************************************************/
/*                msg.c                                                 */
/*                                                                      */
/*    Message handling for Citadel bulletin board system                */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
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

#include "210ctdl.h"
#include "210protos.h"
#include "210common.h"

/************************************************************************/
/*                contents                                              */
/*                                                                      */
/*    aideMessage()    saves auto message in Aide>                      */
/*    dGetWord()       reads a word off disk                            */
/*    dPrintf()        printf() that writes to disk                     */
/*    fakeFullCase()   converts uppercase message to mixed case         */
/*    findPerson()     load log record for named person                 */
/*    flushMsgBuf()    wraps up message-to-disk store                   */
/*    getMessage()     load message into RAM                            */
/*    getMsgChar()     returns successive chars off disk                */
/*    getMsgStr()      reads a string out of message.buf                */
/*    getWord()        gets one word from message buffer                */
/*    mAbort()         checks for user abort of typeout                 */
/*    makeMessage()    menu-level message-entry routine                 */
/*    mFormat()        formats a string to modem and console            */
/*    mPeek()          sysop debugging tool--shows ctdlmsg.sys          */
/*    mWCprintf()      special mprintf for WC transfers                 */
/*    msgInit()        sets up catChar, catSect etc.                    */
/*    noteLogMessage() enter message into log record                    */
/*    noteMessage()    enter message into current room                  */
/*    note2Message()   noteMessage() local                              */
/*    printMessage()   prints a message on modem & console              */
/*    pullIt()         sysop special message-removal routine            */
/*    putMessage()     write message to disk                            */
/*    putMsgChar()     writes successive message chars to disk          */
/*    putWord()        writes one word to modem & console               */
/*    showMessages()   menu-level show-roomful-of-messages fn           */
/*    startAt()        setup to read a message off disk                 */
/*    unGetMsgChar()   return a char to getMsgChar()                    */
/*    zapMsgFile()     initialize ctdlmsg.sys                           */
/************************************************************************/

/************************************************************************/
/*    aideMessage() saves auto message in Aide>                         */
/************************************************************************/
void aideMessage(char noteDeletedMessage) {

    int ourRoom;

    /* message is already set up in msgBuf.mbtext */

    putRoom(ourRoom = thisRoom, &roomBuf);
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

            getLog(lBuf, logNo = logTab[i].ltlogSlot);

            if (strcasecmp(name, lBuf->lbname) == SAMESTRING) {

                foundIt = TRUE;

            }

        }

    }

    return (foundIt) ? logNo : ERROR;

}

/************************************************************************/
/*    flushMsgBuf() wraps up writing a message to disk                  */
/************************************************************************/
void flushMsgBuf(void) {
    
    crypte(sectBuf, SECTSIZE, 0);

    errno = 0;
    if (write(msgfl, sectBuf, 1) < 0) {
        
        putString("?ctdlmsg.sys write fail, reason; %d", errno);

    }

    crypte(sectBuf, SECTSIZE, 0);

}

/************************************************************************/
/*    getMessage() reads a message off disk into RAM.                   */
/*    a previous call to setUp has specified the message.               */
/************************************************************************/
void getMessage(void) {

    unsigned char c;
    
    /* clear msgBuf out */

    msgBuf.mbauth[0]  = '\0';
    msgBuf.mbdate[0]  = '\0';
    msgBuf.mborig[0]  = '\0';
    msgBuf.mboname[0] = '\0';
    msgBuf.mbroom[0]  = '\0';
    msgBuf.mbsrcId[0] = '\0';
    msgBuf.mbtext[0]  = '\0';
    msgBuf.mbto[0]    = '\0';

    do {

        c = getMsgChar(); 

    } while (c != 0xFF);    /* find start of msg    */

    msgBuf.mbheadChar   = oldChar;        /* record location    */
    msgBuf.mbheadSector = oldSector;

    getMsgStr(msgBuf.mbId, NAMESIZE);

    do {

        c = getMsgChar();

        switch (c) {
            case 'A':    getMsgStr(msgBuf.mbauth,  NAMESIZE);    break;
            case 'D':    getMsgStr(msgBuf.mbdate,  NAMESIZE);    break;
            case 'M':    /* just exit -- we'll read off disk */    break;
            case 'N':    getMsgStr(msgBuf.mboname, NAMESIZE);    break;
            case 'O':    getMsgStr(msgBuf.mborig,  NAMESIZE);    break;
            case 'R':    getMsgStr(msgBuf.mbroom,  NAMESIZE);    break;
            case 'S':    getMsgStr(msgBuf.mbsrcId, NAMESIZE);    break;
            case 'T':    getMsgStr(msgBuf.mbto,    NAMESIZE);    break;
            default:
                getMsgStr(msgBuf.mbtext, MAXTEXT);    /* discard unknown field  */
                msgBuf.mbtext[0] = '\0';
                break;
        }

    } while (c != 'M' && isalpha(c));

}

/************************************************************************/
/*    getMsgChar() returns sequential chars from message on disk        */
/************************************************************************/
unsigned char getMsgChar(void) {

    unsigned char toReturn;

    if (GMCCache) {    /* someone did an unGetMsgChar() --return it    */

        toReturn = GMCCache;
        GMCCache = '\0';
        return toReturn;

    }

    oldChar   = thisChar;
    oldSector = thisSector;
    toReturn  = sectBuf[thisChar];

#ifdef XYZZY
    if (debug) putChar(visible(toReturn));
#endif

    thisChar = ++thisChar % SECTSIZE;

    if (thisChar == 0) {

        /* time to read next sector in: */

        thisSector = ++thisSector % maxMSector;

        errno = 0;
        if ((lseek(msgfl, thisSector, SEEK_SET)) < 0) {

            putString("?nextMsgChar-seek fail, reason: %d\n", errno);

        }

        errno = 0;
        if (read(msgfl, sectBuf, 1) < 0) {

            putString("?nextMsgChar-read fail, reason: %d\n", errno);

        }

        crypte(sectBuf, SECTSIZE, 0);

    }

    return(toReturn);

}

/************************************************************************/
/*    getMsgStr() reads a string from message.buf                       */
/************************************************************************/
void getMsgStr(char *dest, int lim) {

    unsigned char c;

    while ((c = getMsgChar())) {  /* read the complete string    */

        if (c == NULL) break;
        if (lim) {              /* if we have room then        */

            lim--;
            *dest++ = c;        /* copy char to buffer         */

        }

    }

    *dest = '\0';            /* tie string off with null    */

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
            logNo = findPerson(msgBuf.mbto, &lBuf);

            if (logNo == ERROR && hash(msgBuf.mbto) != hash("Sysop")) {

                putString("No '%s' known", msgBuf.mbto);
                return FALSE;

            }

        }

    }

    strcpy(msgBuf.mbauth, logBuf.lbname);        /* record author*/

    if (uploading || getText("message", msgBuf.mbtext, MAXTEXT)) {
        
        if (!uploading) {
            
            for (pc = msgBuf.mbtext, allUpper = TRUE; *pc && allUpper;  pc++) {

                if (toupper(*pc) != *pc)  {
                    
                    allUpper = FALSE;
                    
                }

            }
            
            if (allUpper) {

                fakeFullCase(msgBuf.mbtext);

            }

        }

        if ((toReturn = putMessage(uploading))) {
            
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
    int  col, row, s;

    s = getNumber(" sector to dump", 0, maxMSector - 1);
    lseek(msgfl, s, SEEK_SET);
    read(msgfl, peekBuf, 1);

    for (row = 0; row < 2; row++) {

        putString("\n ");

        for (col = 0; col < 64; col++) {
        
            oChar(visible(peekBuf[row * 64 + col]));

        }

    }

}

/************************************************************************/
/*    msgInit() sets up lowId, highId, catSector and catChar,           */
/*    by scanning over message.buf                                      */
/************************************************************************/
void msgInit(void) {

    unsigned short firstLo, firstHi, hereLo, hereHi;    /* 32 bits by halves    */

    startAt(0, 0);
    getMessage();

    /* get the ID# */

    sscanf(msgBuf.mbId, "%d %d", &firstHi, &firstLo);
    putString("message# %d %d\n", firstHi, firstLo);
    
    newestHi = firstHi;
    newestLo = firstLo;
    oldestHi = firstHi;
    oldestLo = firstLo;

    catSector = thisSector;
    catChar   = thisChar;

    for (
         getMessage();
         sscanf(msgBuf.mbId, "%d %d", &hereHi, &hereLo),
         (hereHi == firstHi && hereLo == firstLo);
         getMessage()

    ) {

        putString("message# %d %d\n", hereHi, hereLo);

        /* find highest and lowest message IDs: */
        /* 32-bit "<" by hand:                  */

        if ((hereHi < oldestHi) || (hereHi == oldestHi && hereLo < oldestLo)) {

            oldestHi = hereHi;
            oldestLo = hereLo;

            putString(" oldest=%d %d\n", oldestHi, oldestLo);

        }

        if ((hereHi > newestHi) || (hereHi == newestHi && hereLo > newestLo)) {

            newestHi = hereHi;
            newestLo = hereLo;

            putString(" newest=%d %d\n", newestHi, newestLo);

            /* read rest of message in and remember where it ends,       */
            /* in case it turns out to be the last message               */
            /* in which case, that's where to start writing next message */

            while (dGetWord(msgBuf.mbtext, MAXTEXT));

            catSector = thisSector;
            catChar   = thisChar;

        }

    }

}

/************************************************************************/
/*    noteLogMessage() slots message into log record                    */
/************************************************************************/
void noteLogMessage(struct logBuffer *lBuf, int logNo) {

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
void noteMessage(struct logBuffer *lBuf, int logNo) {

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
void note2Message(int id, int loc) {

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
void printMessage(int loc, unsigned id) {
/* loc - sector in message.buf        */
/* id  - unique-for-some-time ID#     */

    char moreFollows;
    unsigned short hereHi, hereLo;

    startAt(loc, 0);

    do {

        getMessage(); 

    } while ((
        sscanf(msgBuf.mbId, "%d %d", &hereHi, &hereLo),
        hereLo != id) && thisSector == loc);

    if (hereLo != id) {

        putString("?can't find message");
#ifdef XYZZY
        putString(" loc=%d, id=%u, mbIds=%s, here=%d %d\n",
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
    pulledMId  = roomBuf.vp.msg[m].rbmsgNo ;

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
/*    putMessage() stores a message to disk                             */
/*    Always called before noteMessage() -- newestLo not ++ed yet.      */
/*    Returns: TRUE on successful save, else FALSE                      */
/************************************************************************/
char putMessage(char uploading) {
/* uploading - true to get text via WC modem input, not RAM */

    char *s, allOk;
    int year, month, day;

    startAt(catSector, catChar);    /* tell putMsgChar where to write    */
    putMsgChar(0xFF);               /* start-of-message         */

    /* write message ID */

    dPrintf("%d %d", newestHi, newestLo + 1);
    putMsgChar(0);

    /* write date:    */

    getDate(&year, &month, &day);
    dPrintf("D%d%s%02d", year, monthTab[month], day);
    putMsgChar(0);

    /* write room name out:        */

    dPrintf("R%s", roomBuf.rbname);
    putMsgChar(0);

    if (loggedIn) {

        /* write author's name out:        */

        dPrintf("A%s", msgBuf.mbauth);
        putMsgChar(0);        /* null to end string   */

    }

    if (msgBuf.mbto[0]) {    /* private message -- write addressee    */

        dPrintf("T%s", msgBuf.mbto);
        putMsgChar(0);

    }

    /* write message text by hand because it would overrun dPrintf buffer: */

    putMsgChar('M');    /* M-for-message.    */

    for (s = msgBuf.mbtext; *s; s++) {

        putMsgChar(*s);
        allOk = TRUE;

    }

    if (allOk) {

        putMsgChar(0);        /* null to end text     */
        flushMsgBuf();

    } else {

        flushMsgBuf();        /* so message count is ok    */

        /* erase start-of-message indicator: */
        startAt(catSector, catChar);
        putMsgChar(0);        /* overwrite 0xFF byte    */

    }

    return allOk;

}

/************************************************************************/
/*    putMsgChar() writes successive message chars to disk              */
/*    Globals:    thisChar=    thisSector=                              */
/*    Returns:    ERROR if problems else TRUE                           */
/************************************************************************/
int putMsgChar(char c) {

    int  toReturn;

    toReturn = TRUE;

#ifdef XYZZY
    if (debug) putChar(visible(c));
#endif

    if (sectBuf[thisChar] == 0xFF)  {

        /* obliterating a msg    */

        if (!++oldestLo) ++oldestHi;    /* 32-bit increment by hand    */
        logBuf.lbvisit[(MAXVISIT - 1)] = oldestLo;

    }

    sectBuf[thisChar] = c;

    thisChar = ++thisChar % SECTSIZE;

    if (thisChar == 0) {    /* time to write sector out a get next: */

        crypte(sectBuf, SECTSIZE, 0);

        errno = 0;
        if ((lseek(msgfl, thisSector, SEEK_SET)) < 0) {

            putString("?putMsgChar-seek fail, reason: %d\n", errno);
            toReturn = ERROR;

        }

        errno = 0;
        if (write(msgfl, sectBuf, 1) < 0) {

            putString("?putMsgChar-write fail, reason: %d\n", errno);
            toReturn = ERROR;

        }

        thisSector = ++thisSector % maxMSector;

        errno = 0;
        if ((lseek(msgfl, thisSector, SEEK_SET)) < 0) {

            putString("?putMsgChar-seek fail, reason: %d\n", errno);
            toReturn    = ERROR;

        }

        errno = 0;
        if (read(msgfl, sectBuf, 1) < 0) {

            putString("?putMsgChar-rread fail, reason: %d\n", errno);
            toReturn = ERROR;

        }

        crypte(sectBuf, SECTSIZE, 0);

    }

    return toReturn;

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
    int start, finish, increment, msgNo;
    unsigned short lowLim, highLim;

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

    if (oldestLo - lowLim < 0x8000) {

        lowLim = oldestLo;

    }

    for (i = start; i != finish; i += increment) {

        /* "<" comparison with 64K wraparound in mind: */

        msgNo = roomBuf.vp.msg[i].rbmsgNo;

        if (msgNo - lowLim  < 0x8000 && highLim - msgNo < 0x8000) {

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
/*    startAt() sets location to begin reading message from             */
/************************************************************************/
void startAt(unsigned short sect, unsigned short byt) {

    GMCCache = '\0';    /* cache to unGetMsgChar() into */

    if (sect >= maxMSector) {

        putString("?startAt s=%d,b=%d\n", sect, byt);
        return;

    }

    thisChar   = byt;
    thisSector = sect;

    errno = 0;
    if ((lseek(msgfl, sect, SEEK_SET)) < 0) {

        putString("?startAt lseek fail, reason: %d\n", errno);

    }

    errno = 0;
    if (read(msgfl, sectBuf, SECTSIZE) < 0) {

        putString("?startAt read fail, reason: %d\n", errno);

    }

    crypte(sectBuf, SECTSIZE, 0);

    return;

}

/************************************************************************/
/*    unGetMsgChar() returns (at most one) char to getMsgChar()    */
/************************************************************************/
void unGetMsgChar(unsigned char c) {

    GMCCache = c;

}

/************************************************************************/
/*    zapMsgFl() initializes message.buf                                */
/************************************************************************/
void zapMsgFile(void) {

    int i, val;

    if (!getYesNo("\nDestroy all current messages")) return;

    /* put null message in first sector... */

    sectBuf[0] = 0xFF; /*   \                    */
    sectBuf[1] =  '0'; /*    \                   */
    sectBuf[2] =  ' '; /*     > Message ID "0 1" */
    sectBuf[3] =  '1'; /*    /                   */
    sectBuf[4] = '\0'; /*   /                    */
    sectBuf[5] =  'M'; /*   \    Null messsage   */
    sectBuf[6] = '\0'; /*   /                    */

    for (i = 7;  i < SECTSIZE; i++) sectBuf[i] = 0;

    crypte(sectBuf, SECTSIZE, 0);    /* encrypt    */

    errno = 0;
    if ((lseek(msgfl, 0, SEEK_SET)) < 0) {

        putString("zapMsgFil: lseek failed, reason: %d\n", errno);

    }

    errno = 0;
    if ((val = write(msgfl, sectBuf, SECTSIZE)) < 0) {

        putString("zapMsgFil: write failed, %d records, reason: %d!\n", val, errno);

    }

    crypte(sectBuf, SECTSIZE, 0);    /* decrypt    */

}

