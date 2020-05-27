
/************************************************************************/
/*                rooma.c                                               */
/*        room code fore Citadel bulletin board system                  */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 83Feb24    Insert check for insufficient RAM, externs too low.       */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov05 CrT    main() splits off to become citadel.c                 */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "210ctdl.h"
#include "210protos.h"

/************************************************************************/
/*                Contents                                              */
/*                                                                      */
/*    dumpRoom()     tells us # new messages etc                        */
/*    fileDir()      prints out a filename for a dir listing            */
/*    fillMailRoom() set up Mail> from log record                       */
/*    gotoRoom()     handles "g(oto)" command for menu                  */
/*    init()         system startup initialization                      */
/*    initCitadel()                                                     */
/*    initSysop()                                                       */
/*    listRooms()    lists known rooms                                  */
/*    openFile()     opens a .sys file                                  */
/*    readSysTab()   restores system state from citadel.tab             */
/*    roomExists()   returns slot# of named room else ERROR             */
/*    setSpace()     set default disk and user#                         */
/*    setUp()                                                           */
/*    systat()       shows current system status                        */
/*    wildCard()     expands ambiguous filenames                        */
/*    writeSysTab()  saves state of system in citadel.tab               */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*    dumpRoom() tells us # new messages etc                            */
/************************************************************************/
void dumpRoom(void) {

    int i, count, loc, newCount, no;

    for (newCount = 0, count = 0, i = 0; i < MSGSPERRM; i++) {

        loc = roomBuf.vp.msg[i].rbmsgLoc;
        no  = roomBuf.vp.msg[i].rbmsgNo;

        if (loc != ERROR) {

            /* comparisons which take 64K wraparound into account:    */

            if (no - oldestLo < 0x8000) {

                count++;

            }

            /* don't boggle -- just checking against newestLo as of */
            /* the last time we were  in this room:            */

            if (no - (logBuf.lbvisit[logBuf.lbgen[thisRoom] & CALLMASK ] + 1) < 0x8000) {

                newCount++;

            }

        }

    }

    /* lazy patch: */

    if (newCount > count) {

        newCount = count;

    }

    printf(" %d messages\n ", count);

    if (loggedIn && newCount > 0) {

        printf(" %d new\n", newCount);

    }

    if ((roomBuf.rbflags & CPMDIR) && !expert) {

        /* list directory also: */

        doCR();
        wildCard(fileDir, "*.*");

    }

}

/************************************************************************/
/*    fileDir() prints out one filename and size, for a dir listing     */
/************************************************************************/
int fileDir(char *fileName) {

    struct stat buffer;
    char tBuf[SECTSIZE];

    if ((stat(fileName, &buffer)) == 0) {

        sprintf(tBuf, "%s%5d  ", fileName, (int)buffer.st_size);
        putWord(tBuf);
        mAbort();        /* chance to next(!)/pause/skip */

    }

    return TRUE;

}

/************************************************************************/
/*    fillMailRoom()                                                    */
/************************************************************************/
void fillMailRoom(void) {

    int i;

    /* mail room -- copy messages in logBuf to room: */

    for (i = 0; i < MSGSPERRM; i++) {

        roomBuf.vp.msg[i].rbmsgLoc = ERROR;
        roomBuf.vp.msg[i].rbmsgNo  = ERROR;

    }

    for (i = 0; i < MAILSLOTS; i++) {
        
        if (i == MSGSPERRM) break;  /* for MSGSPRRM < MAILSLOTS */

        roomBuf.vp.msg[i].rbmsgLoc = logBuf.lbslot[i];
        roomBuf.vp.msg[i].rbmsgNo  = logBuf.lbId[i];

    }

    noteRoom();

}

/************************************************************************/
/*    gotoRoom() is the menu fn to travel to a new room                 */
/*    returns TRUE if room is Lobby>, else FALSE                        */
/************************************************************************/
char gotoRoom(char *nam) {

    int  i, foundit, newStuff, nwest, roomNo;

    if (!strlen(nam)) {

        /* update log entry for current room:    */
        
        logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;
        foundit = FALSE;    /* leaves us in Lobby> if nothing found */
        newStuff= FALSE;

        for (i = 0; i < MAXROOMS && !foundit; i++) {
            
            if ((roomTab[i].rtflags & INUSE) &&
                (roomTab[i].rtgen == (logBuf.lbgen[i]>>GENSHIFT))) {
                
                nwest = logBuf.lbvisit[logBuf.lbgen[i] & CALLMASK] + 1;

                if (roomTab[i].rtlastMessage - nwest  < 0x8000) {
                    
                    if (i != thisRoom && (i != AIDEROOM || aide)) {
                        
                        foundit = i;
                        newStuff= TRUE;

                    }

                }

            }

        }
        
        getRoom(foundit, &roomBuf);
        printf("%s\n ", roomBuf.rbname);
        
    } else {

        /* non-empty room name, so now we look for it: */

        if ((roomNo = roomExists(nam)) == ERROR ||
            (roomNo==AIDEROOM  &&  !aide)) {
            
            printf(" ?no %s room\n", nam);

        } else {
            
            /* update log entry for current room:   */

            if (loggedIn) {
                
                logBuf.lbgen[thisRoom] = roomBuf.rbgen << GENSHIFT;
                
            }

            getRoom(roomNo, &roomBuf);

            /* if may have been unknown... if so, note it:    */
            
            if ((logBuf.lbgen[thisRoom] >> GENSHIFT) != roomBuf.rbgen) {

                logBuf.lbgen[thisRoom] = (
                    (roomBuf.rbgen << GENSHIFT) + (MAXVISIT -1));

            }

        }

    }

    setUp(FALSE);
    dumpRoom();

    return newStuff;    /* was thisRoom */

}

/************************************************************************/
/*    init() -- master system initialization                            */
/************************************************************************/
void init(void) {
    
    char *msgFile;

    setSpace(homeDisk, homeUser);

    if (FALSE) getText(NULL, NULL, 0);  /* dummy to force load        */

    exitToCpm   = FALSE;        /* not time to quit yet!    */
    sizeLTentry = sizeof(logTab[0]);    /* just had to try that feature */
    outFlag     = OUTOK;        /* not p(ausing)        */

    pullMessage = FALSE;        /* not pulling a message    */
    pulledMLoc  = ERROR;        /* haven't pulled one either    */
    pulledMId   = ERROR;

    debug       = FALSE;
    loggedIn    = FALSE;
    thisRoom    = LOBBY;

    whichIO     = CONSOLE;
    loggedIn    = FALSE;

    setUp(TRUE);

    /* we init modem at beginning & end both...*/

    modemInit();

    monthTab[1]  = "Jan";
    monthTab[2]  = "Feb";
    monthTab[3]  = "Mar";
    monthTab[4]  = "Apr";
    monthTab[5]  = "May";
    monthTab[6]  = "Jun";
    monthTab[7]  = "Jul";
    monthTab[8]  = "Aug";
    monthTab[9]  = "Sep";
    monthTab[10] = "Oct";
    monthTab[11] = "Nov";
    monthTab[12] = "Dec";

    if (!clock)   interpret(pInitDate);

    /* open message files: */

    msgFile    = "ctdlmsg.sys";
    *msgFile   += msgDisk;

    openFile(msgFile,        &msgfl);
    openFile("ctdlroom.sys", &roomfl);
    openFile("ctdllog.sys",  &logfl);

    getRoom(0, &roomBuf);    /* load Lobby>    */
    modemInit();

}

/************************************************************************/
/*    initCitadel() does not reformat data files                        */
/************************************************************************/
void initCitadel(void) {
    
    whichIO = MODEM;
    setUp(FALSE);

}

/************************************************************************/
/*    listRooms() lists known rooms                    */
/************************************************************************/
void listRooms(char doDull) {
/* doDull - TRUE to list unchanged rooms also    */

    char str[NAMESIZE + 3];
    char boringRooms, doBoringRooms, hasUnseenStuff, shownHidden;
    int  i;

    shownHidden = FALSE;
    boringRooms = FALSE;
    printf("\n Rooms with unread messages:\n ");

    for (doBoringRooms = FALSE; doBoringRooms <= doDull; doBoringRooms++) {

        for (i = 0; i < MAXROOMS; i++) {

            /* list it if public or previously visited: */

            if ((roomTab[i].rtflags & INUSE) &&  
                ((roomTab[i].rtflags & PUBLIC) ||
                 (loggedIn && roomTab[i].rtgen == 
                  (logBuf.lbgen[i] >> GENSHIFT)) || debug)) {

                /* do only rooms with unseen messages first pass,  */
                /* only rooms without unseen messages second pass: */

                hasUnseenStuff = (roomTab[i].rtlastMessage - 
                                  (logBuf.lbvisit[ logBuf.lbgen[i] & CALLMASK] + 1)  
                                  < 0x8000);

                if (!hasUnseenStuff) {

                    boringRooms  = TRUE;

                }

                if ((!doBoringRooms &&    hasUnseenStuff)  ||
                    ( doBoringRooms && !hasUnseenStuff)) {

                    strcpy(str, roomTab[i].rtname);

                    if (roomTab[i].rtflags & CPMDIR) {

                        strcat(str, ":");

                    } else {

                        strcat(str, ">");

                    }

                    if (! (roomTab[i].rtflags & PUBLIC)) {

                        strcat(str, "*");
                        shownHidden = TRUE;

                    }

                    printf(" %s", str);

                }

            }

        }

        if (boringRooms && !doBoringRooms && doDull) {

            printf("\n No unseen msgs in:\n ");

        }

    }

    if (!expert && shownHidden) printf("\n \n * => hidden room\n ");

}

/************************************************************************/
/*    openFile() opens one of the .sys files.                           */
/************************************************************************/
void openFile(char *filename, int *fd) {

    /* open message file */

    if ((*fd = open(filename, O_RDWR)) == ERROR) {

        printf("?no %s", filename);
        exit(EXIT_FAILURE);

    }

}

/************************************************************************/
/*    roomExists() returns slot# of named room else ERROR        */
/************************************************************************/
int roomExists(char *room) {

    int i;

    for (i = 0; i < MAXROOMS; i++) {

        if (roomTab[i].rtflags & INUSE &&
            strcasecmp(room, roomTab[i].rtname) == SAMESTRING) {

            return(i);

        }

    }

    return(ERROR);

}

/************************************************************************/
/*    setSpace() moves us to a disk and user#                           */
/************************************************************************/
void setSpace(char disk, char user) {

    /* bdos(SETdISK, disk); */
    ourDisk    = disk;

    /* bdos(SETuSER, user); */
    ourUser    = user;

}

/************************************************************************/
/*    setUp()                                                           */
/************************************************************************/
void setUp(char justIn) {

    int g, i, j, ourSlot;

    echo = BOTH;     /* just in case         */
    usingWCprotocol = FALSE;    /* also redundant        */

    if (!loggedIn) {

        prevChar    = ' ';
        termWidth   = 32;
        termLF      = LFMASK;
        termUpper   = TRUE;
        termNulls   = 5;
        expert      = FALSE;
        aide        = FALSE;

        if (justIn) {

            /* set up logBuf so everything is new...        */

            for (i =0 ; i < MAXVISIT; i++) {

                logBuf.lbvisit[i] = oldestLo;

            }

            /* no mail for anonymous folks: */

            roomTab[MAILROOM].rtlastMessage = newestLo;

            for (i = 0; i < MAILSLOTS; i++)  {

                logBuf.lbId[i] = 0;

            }

            logBuf.lbname[0] = 0;

            for (i = 0; i < MAXROOMS; i++) {

                if (roomTab[i].rtflags & PUBLIC) {

                    /* make public rooms known: */

                    g = roomTab[i].rtgen;
                    logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT - 1);

                } else {

                    /* make private rooms unknown: */

                    g = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
                    logBuf.lbgen[i] = (g << GENSHIFT) + (MAXVISIT - 1);

                }

            }

        }

    } else {

        /* loggedIn: */

        termWidth = logBuf.lbwidth;
        termNulls = logBuf.lbnulls;
        termLF    = logBuf.lbflags & LFMASK;
        termUpper = logBuf.lbflags & UCMASK;
        expert    = logBuf.lbflags & EXPERT;
        termTab   = logBuf.lbflags & TABMASK;
        aide      = logBuf.lbflags & AIDE;

        if (justIn) {

            /* set gen on all unknown rooms  --  INUSE or no: */

            for (i = 0; i < MAXROOMS; i++) {

                if (!(roomTab[i].rtflags & PUBLIC)) {

                    /* it is private -- is it unknown? */

                    if ((logBuf.lbgen[i] >> GENSHIFT) != roomTab[i].rtgen) {

                        /* yes -- set    gen = (realgen-1) % MAXGEN */

                        j = (roomTab[i].rtgen + (MAXGEN-1)) % MAXGEN;
                        logBuf.lbgen[ i ] =  (j << GENSHIFT) + (MAXVISIT - 1);

                    }

                } else if ((logBuf.lbgen[i] >> GENSHIFT) !=  roomTab[i].rtgen) {

                    /* newly created public room -- remember to visit it; */

                    logBuf.lbgen[i] = (roomTab[i].rtgen << GENSHIFT) + 1;

                }

            }

            /* special kludge for Mail> room, to signal new mail:   */

            roomTab[MAILROOM].rtlastMessage = logBuf.lbId[MAILSLOTS - 1];

            /* slide lbvisit array down and change lbgen entries to match: */

            for (i = (MAXVISIT - 2); i; i--) {

                logBuf.lbvisit[i] = logBuf.lbvisit[i - 1];

            }

            logBuf.lbvisit[(MAXVISIT - 1)] = oldestLo;

            for (i = 0; i < MAXROOMS; i++) {

                if ((logBuf.lbgen[i] & CALLMASK)  <  (MAXVISIT-2)) {

                    logBuf.lbgen[i]++;

                }

            }

            /* Slide entry to top of log table: */

            ourSlot = logTab[thisSlot].ltlogSlot;
            slideLTab(0, thisSlot);

            logTab[0].ltpwhash  = hash(logBuf.lbpw);
            logTab[0].ltnmhash  = hash(logBuf.lbname);
            logTab[0].ltlogSlot = ourSlot;
            logTab[0].ltnewest  = newestLo;

        }

    }

    logBuf.lbvisit[0] = newestLo;
    onConsole = (whichIO == CONSOLE);

    if (thisRoom == MAILROOM) {

        fillMailRoom();

        if (whichIO == MODEM) {

            echo = CALLER;

        } else  {

            echo = BOTH;

        }

    }

}
    
/************************************************************************/
/*    systat() prints out current system status            */
/************************************************************************/
void systat(void) {
    
    printDate(interpret(pGetYear ), interpret(pGetMonth), interpret(pGetDay));

#ifdef XYZZY
    if (debug) printf(" *catChar=%d catSector=%d\n*", catChar, catSector);
#endif

    printf("\n Width %d, %s%slinefeeds, %d nulls\n",
           termWidth,
           termUpper ? "UPPERCASE ONLY, " : "",
           termLF    ? ""                 : "no ",
           termNulls
    );

#ifdef XYZZY
    if (debug) printf(" *lowId=%u %u, highId=%u %u\n*",
                      oldestHi, oldestLo,
                      newestHi, newestLo
    );
#endif

    if (loggedIn) {
        
        printf(" Logged in as %s\n", logBuf.lbname);
#ifdef XYZZY
        if (debug) {
            for (i=0; i<MAXVISIT; i++) {
                printf("lbvisit[%d]=%u\n", i, logBuf.lbvisit[i]);
            }
            printf("lbgen>>GENSHIFT==%d; roomTab.rtgen==%d; roomBuf.rbgen==%d\n"
                   , logBuf.lbgen[thisRoom]>>GENSHIFT, roomTab[thisRoom].rtgen,
                   roomBuf.rbgen
                   );
            printf("lbgen&CALLMASK==%d\n", logBuf.lbgen[thisRoom]&CALLMASK);
        }
#endif
    }

    printf(" %d messages,",         newestLo - oldestLo + 1);
    printf(" last is %u %u,\n",     newestHi, newestLo);
    printf(" %dK message space,\n", maxMSector >> 3);
    printf(" %d-entry log\n",       MAXLOGTAB);
    
}

/************************************************************************/
/*    unspace() copies a filename, removing blanks            */
/************************************************************************/
void unspace(char *from, char *to) {

    while (*to = *from++)
        if ((*to != ' ')) to++;

}

/************************************************************************/
/*    wildCard()                            */
/* Adapted from YAM which borrowed from Richard Greenlaw's expander.    */
/************************************************************************/
#define UFNsIZE        13    /* size of "filename.ext" plus a null.    */

void wildCard(int (*fn)(char *), char *filename) {

    /* may be ambiguous.  No drive or user numbers. */

    /* struct fcb    block, *pBlock; */

    /* char *p, *q, c, byteAddr, finished; */

    /* int  fileCount, i, m; */

    /* char tBuf[SECTSIZE]; */
    /* struct { */
    /* char unambig[UFNsIZE];    *//* filename                */
    /* } *fp; */

    /* if ( */
    /* filename[1]         == ':'   || */
    /* setFcb(&block, filename) == 255 */
    /* ) { */
    /* bad filename: */
    /* mprintf("%s?\n ", filename); */
    /* return; */
    /* } */

    /* bdos(SETdMA, tBuf); */

    /* setSpace(roomBuf.rbdisk, roomBuf.rbuser); */

    /* if ( (byteAddr=bdos(FINDfIRST, &block))  ==  255)    { */
    /* no such file */
    /* mprintf("no %s\n ", filename); */
    /* setSpace(homeDisk, homeUser); */
    /* return; */
    /* } */

    /* search directory for all matches: */
    /* for ( */
    /* fp=msgBuf.mbtext, fileCount=0, finished=FALSE; */

    /* !finished; */

    /* fp++, fileCount++, */
    /* finished = ((byteAddr=bdos(FINDnEXT, &block)) == 255) */
    /* ) { */

    /* figure out where BDOS stashed our fcb:   */
    /* pBlock = tBuf + 32*(byteAddr % 4); */ /* "% 4"? --CrT            */

    /* p   = fp; */

    /* if (fp > (&msgBuf.mbtext[MAXTEXT-100]))  { */
    /*     printf("too many files!!\n"); */
    /*     setSpace(homeDisk, homeUser); */
    /*     return; */
    /* } */

    /* copy filename from directory:     */
    /* q    = pBlock; */
    /* for (i=8;  i;  i--)   *p++   = toLower(*++q & 0x7F); */
    /* *p++ = '.'; */

    /* get file extention: */
    /* for (i=3;  i;  i--)  *p++    = toLower(*++q & 0x7F); */
    /* *p   = '\0'; */        /* tie off with null */
    /* } */

    /* qSort(msgBuf.mbtext, fileCount, UFNsIZE, strCmp); */

    /* outFlag    = OUTOK; */
    /* for (fp=msgBuf.mbtext;  fileCount-- && outFlag!=OUTSKIP;  fp++) { */
    /* (*fn)(fp); */
    /* } */
    /* setSpace(homeDisk, homeUser); */

}

