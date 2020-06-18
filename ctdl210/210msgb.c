
/************************************************************************/
/*                msgb.c                                                */
/*                                                                      */
/*    Message handling for Citadel bulletin board system                */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 20Jun18 KLE   Split out from msg.c                                   */
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
/*    startAt()        setup to read a message off disk                 */
/*    flushMsgBuf()    wraps up message-to-disk store                   */
/*    getMessage()     load message into RAM                            */
/*    getMsgChar()     returns successive chars off disk                */
/*    getMsgStr()      reads a string out of message.buf                */
/*    putMessage()     write message to disk                            */
/*    putMsgChar()     writes successive message chars to disk          */
/*    zapMsgFile()     initialize ctdlmsg.sys                           */
/*    msgInit()        sets up catChar, catSect etc.                    */
/************************************************************************/

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

        if ((hereHi < oldestHi) || 
            ((hereHi == oldestHi) && (hereLo < oldestLo))) {

            oldestHi = hereHi;
            oldestLo = hereLo;

            putString(" oldest=%d %d\n", oldestHi, oldestLo);

        }

        if ((hereHi > newestHi) || 
            ((hereHi == newestHi) && (hereLo > newestLo))) {

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
            case 'A': getMsgStr(msgBuf.mbauth,  NAMESIZE);    break;
            case 'D': getMsgStr(msgBuf.mbdate,  NAMESIZE);    break;
            case 'M': getMsgStr(msgBuf.mbtext,  MAXTEXT);     break;
            case 'N': getMsgStr(msgBuf.mboname, NAMESIZE);    break;
            case 'O': getMsgStr(msgBuf.mborig,  NAMESIZE);    break;
            case 'R': getMsgStr(msgBuf.mbroom,  NAMESIZE);    break;
            case 'S': getMsgStr(msgBuf.mbsrcId, NAMESIZE);    break;
            case 'T': getMsgStr(msgBuf.mbto,    NAMESIZE);    break;
            default: break;
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

        thisSector = ++thisSector % maxMSector;
        toReturn = loadMsgBuf();

    }

    return(toReturn);

}

/************************************************************************/
/*    getMsgStr() reads a string from message.buf                       */
/************************************************************************/
void getMsgStr(char *dest, int lim) {

    unsigned char c;

    while ((c = getMsgChar())) {  /* read the complete string    */

        if (lim) {              /* if we have room then        */

            lim--;
            *dest++ = c;        /* copy char to buffer         */

        }

    }

    *dest = '\0';            /* tie string off with null    */

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
        allOk = flushMsgBuf();

    } else {

        allOk = flushMsgBuf();        /* so message count is ok    */

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

    int toReturn, offset;

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

        toReturn = flushMsgBuf();

        if (toReturn != ERROR) {

            thisSector = ++thisSector % maxMSector;
            toReturn = loadMsgBuf();

        }

    }

    return toReturn;

}

/************************************************************************/
/*    startAt() sets location to begin reading message from             */
/************************************************************************/
int startAt(unsigned short sect, unsigned short byt) {

    int toReturn = 0;

    GMCCache = '\0';    /* cache to unGetMsgChar() into */

    if (sect >= maxMSector) {

        putError(" ?startAt s=%d,b=%d\n", sect, byt);
        return ERROR;

    }

    thisChar   = byt;
    thisSector = sect;
    toReturn   = loadMsgBuf();

    return toReturn;

}

/************************************************************************/
/*    flushMsgBuf() wraps up writing a message to disk                  */
/************************************************************************/
int flushMsgBuf(void) {

    int i;
    int toReturn = 0;
    int offset = thisSector * SECTSIZE;

    /* pad the buffer with nulls */

fprintf(stderr, "thisChar = %d\n", thisChar);
fprintf(stderr, "thisSector = %d\n", thisSector);

    for (i = thisChar; i < SECTSIZE; i++) sectBuf[i] = '\0';

    /* crypte(sectBuf, SECTSIZE, 0); */

    errno = 0;
    if (lseek(msgfl, offset, SEEK_SET) < 0) {

        putError(" ?flushMsgBuf - seek fail, reason; %d", errno);

    }

    errno = 0;
    if (write(msgfl, sectBuf, SECTSIZE) < 0) {

        putError(" ?flushMsgBuf - write fail, reason; %d", errno);
        toReturn = ERROR;

    }

    /* crypte(sectBuf, SECTSIZE, 0); */

    return toReturn;

}

/************************************************************************/
/*    loadMsgBuf() load the buffer from disk                            */
/************************************************************************/
int loadMsgBuf(void) {

    int toReturn = 0;
    int offset = thisSector * SECTSIZE;

    memset(sectBuf, '\0', SECTSIZE);

    errno = 0;
    if ((lseek(msgfl, offset, SEEK_SET)) < 0) {

        putError(" ?loadMsgBuf - seek fail, reason: %d\n", errno);

    }

    errno = 0;
    if (read(msgfl, sectBuf, SECTSIZE) == 0) {

        putError(" ?loadMsgBuf - read fail, reason: %d\n", errno);
        toReturn = ERROR;

    }

    /* crypte(sectBuf, SECTSIZE, 0); */

    return toReturn;

}

/************************************************************************/
/*    zapMsgFl() initializes message.buf                                */
/************************************************************************/
void zapMsgFile(void) {

    int i;

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

    flushMsgBuf();

}

