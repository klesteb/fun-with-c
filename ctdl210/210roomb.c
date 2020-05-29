
/************************************************************************/
/*                roomb.c                                               */
/*        room code for Citadel bulletin board system                   */
/************************************************************************/

/************************************************************************/
/*                History                                               */
/*                                                                      */
/* 83Feb26 CrT    bug in makeRoom when out of rooms fixed.              */
/* 83Feb26 CrT    matchString made caseless, normalizeString()          */
/* 83Feb26 CrT    "]" directory prompt, user name before prompts        */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov02 CrT    Cleanup prior to V1.2 mods.                           */
/* 82Nov01 CrT    Proofread for CUG distribution.                       */
/* 82Mar27 dvm    conversion to v. 1.4 begun                            */
/* 82Mar25 dvm    conversion for TRS-80/Omikron test started            */
/* 81Dec21 CrT    Log file...                                           */
/* 81Dec20 CrT    Messages...                                           */
/* 81Dec19 CrT    Rooms seem to be working...                           */
/* 81Dec12 CrT    Started.                                              */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "210ctdl.h"
#include "210protos.h"

/************************************************************************/
/*                Contents                                              */
/*                                                                      */
/*    editText()       handles the end-of-message-entry menu            */
/*    findRoom()       find a free room                                 */
/*    getNumber()      prompt user for a number, limited range          */
/*    getRoom()        load given room into RAM                         */
/*    getString()      read a string in from user                       */
/*    getText()        reads a message in from user                     */
/*    getYesNo()       prompts for a yes/no response                    */
/*    givePrompt()     gives usual "THISROOM>" prompt                   */
/*    indexRooms()     build RAM index to ctdlroom.sys                  */
/*    makeRoom()       make new room via user dialogue                  */
/*    matchString()    search for given string                          */
/*    noteRoom()       enter room into RAM index                        */
/*    putRoom()        store room to given disk slot                    */
/*    renameRoom()     sysop special to rename rooms                    */
/*    replaceString()  string-substitute for message entry              */
/*    zapRoomFile()    erase & re-initialize ctdlroom.sys               */
/************************************************************************/

/************************************************************************/
/*    editText() handles the end-of-message-entry menu.                 */
/*    return TRUE  to save message to disk,                             */
/*           FALSE to abort message, and                                */
/*           ERROR if user decides to continue                          */
/************************************************************************/
int editText(char *buf, int lim) {

    char c;
    int year, month, day;

    for (;;) {

        putString("\n entry cmd: ");

        switch (c = toupper(iChar())) {
            case 'A':
                putString("bort\n ");
                if (getYesNo(" confirm")) return FALSE;
                break;
            case 'C':
                putString("ontinue\n ");
                return ERROR;
            case 'P':
                putString("rint formatted\n ");
                doCR();
                putString("   ");
                getDate(&year, &month, &day);
                printDate(year, month, day);
                if (loggedIn)  putString(" from %s", msgBuf.mbauth);
                doCR();
                mFormat(buf);
                break;
            case 'R':
                putString("eplace string\n ");
                replaceString(buf, lim);
                break;
            case 'S':
                putString("ave buffer\n ");
                return TRUE;
            default:
                tutorial("edit.mnu");
                break;
        }

    }

    return FALSE;

}

/************************************************************************/
/*    findRoom() returns # of free room if possible, else ERROR         */
/************************************************************************/
int findRoom(void) {

    int roomRover;

    for (roomRover = 0; roomRover < MAXROOMS; roomRover++) {

        if (!(roomTab[roomRover].rtflags & INUSE)) return roomRover;

    }

    return ERROR;

}

/************************************************************************/
/*    getNumber() prompts for a number in (bottom, top) range.          */
/************************************************************************/
int getNumber(char *prompt, unsigned bottom, unsigned top) {

    unsigned try;
    char numstring[NAMESIZE];

    do {

        getString(prompt, numstring, NAMESIZE);

        try = atoi(numstring);

        if (try < bottom) {

            putString("Sorry, must be at least %d\n", bottom);

        }

        if (try > top) {

            putString("Sorry, must be no more than %d\n", top);

        }

    } while ((try < bottom ||  try > top));

    return  try;

}

/************************************************************************/
/*    getRoom()                                                         */
/************************************************************************/
void getRoom(int rm, struct roomBuffer *buf) {

    unsigned val;

    /* load room #rm into memory starting at buf */

    thisRoom = rm;
    lseek(roomfl, rm * SECSPERROOM, SEEK_SET);

    if ((val = read(roomfl, &roomBuf, SECSPERROOM)) >= 1000) {

        putString(" ?getRoom(): read failed, val=%d\n", val);

    }

    crypte((char *)&roomBuf, (SECSPERROOM * SECTSIZE), rm);

}

/************************************************************************/
/*    getString() gets a string from the user.                          */
/************************************************************************/
void getString(char *prompt, char *buf, int lim) {

    char c;
    int  i;

    if (strlen(prompt) > 0) {

        doCR();
        putString("Enter %s\n : ", prompt);

    }

    i = 0;

    while (c = iChar(), c != NEWLINE && i < lim) {

        /* handle delete chars: */

        if (c == BACKSPACE) {

            oChar(' ');
            oChar(BACKSPACE);
            if (i > 0) {

                i--;

            } else {

                oChar(' ');
                oChar(BELL);

            }

        } else {

            buf[i++] = c;

        }

        if (i >= lim) {

            oChar(BELL);
            oChar(BACKSPACE); i--;

        }

        /* kludge to return immediately on single '?': */

        if (*buf == '?') {

            doCR();
            break;

        }

    }

    buf[i]  = '\0';

}

/************************************************************************/
/*    getText() reads a message from the user                           */
/*    Returns TRUE if user decides to save it, else FALSE               */
/************************************************************************/
char getText(char *prompt, char *buf, int lim) {

    char c, sysopAbort;
    int  i, toReturn, year, month, day;

    if (!expert) {

        tutorial("entry.blb");
        putString("Enter %s (end with empty line)", prompt);

    }

    doCR();
    putString("   ");
    getDate(&year, &month, &day);
    printDate(year, month, day);

    if (loggedIn) {

        putString("from %s", msgBuf.mbauth);

    }

    doCR();

    lim--;
    i = 0;
    toReturn   = TRUE;
    sysopAbort = FALSE;

    do {

        /* this code would handle the modem as well...    */
        /* fastIn() is a later addition to handle people  */
        /* who like to upload fast without handshaking    */

        while (!((c = iChar()) == NEWLINE && buf[i - 1] == NEWLINE ) && 
                   i < lim) {

            if (debug) putChar(visible(c));

            if (c != BACKSPACE) {

                buf[i++] = c;

            } else {

                /* handle delete chars: */
                oChar(' ');
                oChar(BACKSPACE);

                if (i > 0 && buf[i-1] != NEWLINE) {

                    i--;

                } else {

                    oChar(BELL);
                        
                }

            }

        }

        buf[i] = 0x00;           /* null to terminate message    */

        if (i == lim) {

            putString(" buffer overflow\n ");

        }

        toReturn = (sysopAbort) ? FALSE : editText(buf, lim);

    } while ((toReturn == ERROR));

    return toReturn;

}

/************************************************************************/
/*    getYesNo() prompts for a yes/no response                          */
/************************************************************************/
char getYesNo(char *prompt) {

    int toReturn;

    for (doCR(), toReturn = ERROR; toReturn == ERROR;) {

        putString("%s? (Y/N): ", prompt);

        switch (toupper(iChar())) {
            case 'Y': toReturn = TRUE ; break;
            case 'N': toReturn = FALSE; break;
        }

        doCR();

    }

    return toReturn;

}

/************************************************************************/
/*    givePrompt() prints the usual "CURRENTROOM>" prompt.              */
/************************************************************************/
void givePrompt(void) {

    doCR();

    if (loggedIn) {

        putString("(%s)\n", logBuf.lbname);

    }

    if (roomBuf.rbflags & CPMDIR) {

        putString("%s] ", roomBuf.rbname);

    } else {

        putString("%s> ", roomBuf.rbname);

    }

}

/************************************************************************/
/*    indexRooms() -- build RAM index to room.buf                       */
/************************************************************************/
void indexRooms(void) {

    int goodRoom, m, roomCount, slot;

    roomCount = 0;

    for (slot = 0; slot < MAXROOMS; slot++) {

        getRoom(slot, &roomBuf);

        if (roomBuf.rbflags & INUSE) {

            roomBuf.rbflags ^= INUSE;        /* clear "inUse" flag */

            for (m = 0, goodRoom = FALSE; m < MSGSPERRM && !goodRoom; m++) {

                /* comparison done with 64K wraparound in mind: */

                if (roomBuf.vp.msg[m].rbmsgNo - oldestLo < 0x800) {

                    goodRoom = TRUE;

                }

            }

            if (goodRoom || (roomBuf.rbflags & PERMROOM)) {

                roomBuf.rbflags |= INUSE;

            }

            if (roomBuf.rbflags & INUSE) {

                roomCount++;

            } else {

                roomBuf.rbflags = 0;
                putRoom(slot, &roomBuf);

            }

        }

        noteRoom();

    }
#ifdef XYZZY
    putString(" %d of %d rooms in use\n", roomCount, MAXROOMS);
#endif
}

/************************************************************************/
/*    makeRoom() constructs a new room via dialogue with user.          */
/************************************************************************/
void makeRoom(void) {

    char nm[NAMESIZE];
    char oldName[NAMESIZE];
    int  i;

    /* update lastMessage for current room: */

    logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;

    strcpy(oldName, roomBuf.rbname);

    if ((thisRoom = findRoom()) == ERROR) {

        indexRooms();    /* try and reclaim an empty room    */

        if ((thisRoom = findRoom()) == ERROR) {

            putString(" ?no room");

            /* may have reclaimed old room, so: */

            if (roomExists(oldName) == ERROR) {

                strcpy(oldName, "Lobby");
                
            }

            getRoom(roomExists(oldName), &roomBuf);
            return;

        }

    }

    getString("name for new room", nm, NAMESIZE);
    normalizeString(nm);

    if (roomExists(nm) >= 0) {

        putString(" A '%s' already exists.\n", nm);

        /* may have reclaimed old room, so: */

        if (roomExists(oldName) == ERROR) {

            strcpy(oldName, "Lobby");

        }

        getRoom(roomExists(oldName), &roomBuf);
        return;

    }

    if (!expert) tutorial("newroom.blb");

    roomBuf.rbflags = INUSE;

    if (getYesNo(" Make room public")) {

        roomBuf.rbflags |= PUBLIC;

    }

    putString("'%s', a %s room", 
           nm, (roomBuf.rbflags & PUBLIC) ? "public" : "private" );

    if (!getYesNo("Install it")) {

        /* may have reclaimed old room, so: */

        if (roomExists(oldName) == ERROR) {

            strcpy(oldName, "Lobby");

        }

        getRoom(roomExists(oldName), &roomBuf);
        return;

    }

    strcpy(roomBuf.rbname, nm);

    for (i = 0; i < MSGSPERRM; i++) {

        roomBuf.vp.msg[i].rbmsgNo   = 0;    /* mark all slots empty */
        roomBuf.vp.msg[i].rbmsgLoc  = ERROR;

    }

    roomBuf.rbgen = (roomBuf.rbgen +1) % MAXGEN;

    noteRoom();             /* index new room    */
    putRoom(thisRoom, &roomBuf);

    /* update logBuf: */

    logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;

}

/************************************************************************/
/*    matchString() searches for match to given string.  Runs backward  */
/*    through buffer so we get most recent error first.                 */
/*    Returns loc of match, else ERROR                                  */
/************************************************************************/
char *matchString(char *buf, char *pattern, char *bufEnd) {

    char *loc, *pc1, *pc2;
    char foundIt;

    for (loc = bufEnd, foundIt = FALSE; !foundIt && --loc >= buf;) {

        for (pc1 = pattern, pc2 = loc, foundIt=TRUE;  *pc1 && foundIt;) {

            if (! (tolower(*pc1++) == tolower(*pc2++))) {

                foundIt = FALSE;

            }

        }

    }

    return (foundIt) ? loc : NULL;

}

/************************************************************************/
/*    normalizeString() deletes leading & trailing blanks etc.    */
/************************************************************************/
void normalizeString(char *s) {

    char *pc;

    pc = s;

    /* find end of string   */

    while (*pc) {

        if (*pc < ' ') *pc = ' ';   /* zap tabs etc... */
        pc++;

    }

    /* no trailing spaces: */

    while (pc > s && isspace(*(pc - 1))) pc--;

    *pc = '\0';

    /* no leading spaces: */

    while (*s == ' ') {

        for (pc=s; *pc; pc++) *pc = *(pc + 1);

    }

    /* no double blanks */

    for (; *s; s++) {

        if (*s == ' ' && *(s + 1) == ' ') {

            for (pc = s; *pc; pc++) *pc = *(pc + 1);

        }

    }

}

/************************************************************************/
/*    noteRoom() -- enter room into RAM index array.                    */
/************************************************************************/
void noteRoom(void) {

    int i, last;

    last = 0;

    for (i = 0; i < MSGSPERRM; i++) {

        if (roomBuf.vp.msg[i].rbmsgNo > last) {

            last = roomBuf.vp.msg[i].rbmsgNo;

        }

    }

    roomTab[thisRoom].rtlastMessage = last;
    strcpy(roomTab[thisRoom].rtname, roomBuf.rbname);
    roomTab[thisRoom].rtgen = roomBuf.rbgen;
    roomTab[thisRoom].rtflags = roomBuf.rbflags;

}

/************************************************************************/
/*    putRoom() stores room in buf into slot rm in room.buf             */
/************************************************************************/
void putRoom(int rm, struct roomBuffer *buf) {

    unsigned val;

    crypte((char *)&roomBuf, (SECSPERROOM * SECTSIZE), rm);
    lseek(roomfl, rm * SECSPERROOM, SEEK_SET);

    if ((val = write(roomfl, &roomBuf, SECSPERROOM)) != SECSPERROOM) {

        putString("?putRoom()%d", val);

    }

    crypte((char *)&roomBuf, (SECSPERROOM * SECTSIZE), rm);

}

/************************************************************************/
/*    renameRoom() is sysop special fn                                  */
/*    Returns:    TRUE on success else FALSE                            */
/************************************************************************/
int renameRoom(void) {

    char nm[NAMESIZE];
    char c, goodOne, wasDirectory;
    int  r;

    /* clearer than "thisRoom <= AIDEROOM"*/    

    if (thisRoom == LOBBY || thisRoom == MAILROOM || thisRoom == AIDEROOM) {

        putString("? -- may not edit this room.\n ");
        return FALSE;

    }

    if (!getYesNo("confirm")) {

        return FALSE;

    }

    if (getYesNo("Change name")) {

        getString("new room name", nm, NAMESIZE);
        normalizeString(nm);
        r = roomExists(nm);

        if (r >= 0 && r != thisRoom) {

            putString("A %s exists already!\n", nm);

        } else {

            strcpy(roomBuf.rbname, nm);   /* also in room itself  */

        }

    }

    putString("%s, ", (roomBuf.rbflags & PUBLIC) ? "public" : "private");
    putString("%s, ", (roomBuf.rbflags & PERMROOM) ? " permanent" : " temporary");

    wasDirectory = roomBuf.rbflags & CPMDIR;
    putString("%sdirectory room\n ", (wasDirectory) ? "" : "non");

    roomBuf.rbflags = INUSE;

    if (getYesNo("Public room")) {

        roomBuf.rbflags |= PUBLIC;

    } else {

        roomBuf.rbgen = (roomBuf.rbgen +1) % MAXGEN;

    }

    if (!onConsole) {

        roomBuf.rbflags |= wasDirectory;

    } else if (getYesNo("Directory room")) {

        roomBuf.rbflags |= CPMDIR;

        putString(" now space %c%c\n", 'A'+roomBuf.rbdisk, '0'+roomBuf.rbuser);

        for (goodOne = FALSE; !goodOne;) {

            getString("disk", nm, NAMESIZE);
            c = toupper(nm[0]);

            if (c >= 'A' && c <= 'P') {

                roomBuf.rbdisk = c - 'A';
                goodOne        = TRUE;

            } else {

                putString("?");

            }

        }

        roomBuf.rbuser = getNumber("user", 0, 31);
        putString(" space %c%c\n", 'A'+roomBuf.rbdisk, '0'+roomBuf.rbuser);

    }

    if (roomBuf.rbflags & CPMDIR || getYesNo("permanent")) {

        roomBuf.rbflags |= PERMROOM;

    }

    noteRoom();
    putRoom(thisRoom, &roomBuf);

    return TRUE;

}

/************************************************************************/
/*    replaceString() corrects typos in message entry                   */
/************************************************************************/
void replaceString(char *buf, int lim) {

    char oldString[2 * SECTSIZE];
    char newString[2 * SECTSIZE];
    char *loc, *textEnd;
    char *pc;
    int  incr;

    for (textEnd = buf; *textEnd; textEnd++);    /* find terminal null    */

    getString("string", oldString, (2 * SECTSIZE));

    if ((loc = matchString(buf, oldString, textEnd)) == NULL) {

        putString("?not found.\n ");
        return;

    }

    getString("replacement", newString, (2*SECTSIZE));

    if ((strlen(newString) - strlen(oldString)) >= (&buf[lim] - textEnd)) {

        putString("?Overflow!\n ");
        return;

    }

    /* delete old string: */

    for (pc = loc, incr = strlen(oldString); *pc = *(pc + incr); pc++);

    textEnd -= incr;

    /* make room for new string: */

    for (pc = textEnd, incr = strlen(newString);  pc >= loc; pc--) {

        *(pc + incr) = *pc;

    }

    /* insert new string: */

    for (pc = newString; *pc; *loc++ = *pc++);

}

/************************************************************************/
/*    zapRoomFile() erases and re-initilizes room.buf                   */
/************************************************************************/
void zapRoomFile(void) {

    int i;

    if (getYesNo("\nWipe room file")) return;

    roomBuf.rbflags   = 0;
    roomBuf.rbgen     = 0;
    roomBuf.rbdisk    = 0;
    roomBuf.rbuser    = 0;
    roomBuf.rbname[0] = 0;    /* unnecessary -- but I like it...    */

    for (i = 0; i < MSGSPERRM; i++) {

        roomBuf.vp.msg[i].rbmsgNo = roomBuf.vp.msg[i].rbmsgLoc = 0;

    }

    putString("maxrooms = %d\n", MAXROOMS);

    for (i = 0; i < MAXROOMS; i++) {

        putString("clearing room %d\n", i);
        putRoom(i, &roomBuf);

    }

    /* Lobby> always exists -- guarantees us a place to stand! */

    thisRoom = 0;
    strcpy(roomBuf.rbname, "Lobby");
    roomBuf.rbflags = (PERMROOM | PUBLIC | INUSE);
    putRoom(LOBBY, &roomBuf);

    /* Mail> is also permanent...    */

    thisRoom = MAILROOM;
    strcpy(roomBuf.rbname, "Mail");
    roomBuf.rbflags = (PERMROOM | PUBLIC | INUSE);
    putRoom(MAILROOM, &roomBuf);

    /* Aide> also...            */

    thisRoom = AIDEROOM;
    strcpy(roomBuf.rbname, "Aide");
    roomBuf.rbflags = (PERMROOM | INUSE);
    putRoom(AIDEROOM, &roomBuf);

}

