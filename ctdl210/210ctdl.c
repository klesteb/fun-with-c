
/************************************************************************/
/*                ctdl.c                                                */
/*        Command-interpreter code for Citadel                          */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.c"

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 83Mar08 CrT    Aide-special functions installed & tested...          */
/* 83Feb24 CrT/SB Menus rearranged.                                     */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov05 CrT    removed main() from room2.c and split into sub-fn()s  */
/************************************************************************/
/************************************************************************/
/*                Contents                                              */
/*                                                                      */
/*    doAide()      handles Aide-only commands                          */
/*    doChat()      handles C(hat)        command                       */
/*    doEnter()     handles E(nter)       command                       */
/*    doGoto()      handles G(oto)        command                       */
/*    doHelp()      handles H(elp)        command                       */
/*    doKnown()     handles K(nown rooms) command                       */
/*    doLogin()     handles L(ogin)       command                       */
/*    doLogout()    handles T(erminate)   command                       */
/*    doRead()      handles R(ead)        command                       */
/*    doRegular()   fanout for above commands                           */
/*    doSysop()     handles sysop-only commands                         */
/*    getCommand()  prints prompt and gets command char                 */
/*    greeting()    System-entry blurb etc                              */
/*    main()        has the central menu code                           */
/************************************************************************/

/************************************************************************/
/*    doAide() handles the aide-only menu                               */
/*        return FALSE to fall invisibly into default error msg         */
/************************************************************************/
char doAide(char moreYet, char first) {
/* moreYet -                        */
/* first - first parameter if TRUE  */

    char oldName[NAMESIZE];
    int  rm;

    if (!aide) return FALSE;

    if (moreYet) first = '\0';

    putString("Aide special fn: ");

    if (first) oChar(first);

    switch (toupper(first ? first : iChar())) {
        case 'D':
            putString("Delete empty rooms\n ");
            strcpy(oldName, roomBuf.rbname);
            indexRooms();

            if ((rm = roomExists(oldName)) != ERROR) {

                getRoom(rm, &roomBuf);

            } else {

                getRoom(LOBBY, &roomBuf);

            }

            sprintf(msgBuf.mbtext, "Empty rooms deleted by %s", logBuf.lbname);
            aideMessage( /* noteDeletedMessage== */ FALSE );
            break;
        case 'E':
            putString("Edit room\n  \n");
            strcpy(oldName, roomBuf.rbname);
            if (!renameRoom())   break;
            sprintf(
                    msgBuf.mbtext,
                    "%s> edited to %s> by %s",
                    oldName,
                    roomBuf.rbname,
                    logBuf.lbname
            );
            aideMessage( /* noteDeletedMessage == */ FALSE);
            break;
        case 'I':
            putString("Insert message\n ");
            if (thisRoom == AIDEROOM || pulledMLoc == ERROR)   {
                putString("nope!");
                break;
            }
            note2Message(pulledMId, pulledMLoc);
            putRoom(thisRoom, &roomBuf);
            noteRoom();
            sprintf(
                msgBuf.mbtext,
                "Following message inserted in %s> by %s",
                roomBuf.rbname,
                logBuf.lbname
            );
            aideMessage( /* noteDeletedMessage == */ TRUE);
            break;
        case 'K':
            putString("Kill room\n ");
            if (thisRoom == LOBBY || 
                thisRoom == MAILROOM || 
                thisRoom == AIDEROOM ) {
                putString(" not here!");
                break;
            }
            if (!getYesNo("confirm")) break;

            sprintf(
                msgBuf.mbtext,
                "%s> killed by %s",
                roomBuf.rbname,
                logBuf.lbname
            );
            aideMessage( /* noteDeletedMessage == */ FALSE);

            roomBuf.rbflags ^= INUSE;
            putRoom(thisRoom, &roomBuf);
            noteRoom();
            getRoom(LOBBY, &roomBuf);
            break;
        case '?':
            tutorial("aide.mnu");
            break;
        default:
            if (!expert) putString(" ?(Type '?' for menu)\n "    );
            else putString(" ?\n "             );
           break;
    }

    return TRUE;

}

/************************************************************************/
/*    doChat()                            */
/************************************************************************/
void doChat(char moreYet, char first) {
/* moreYet - TRUE to accept following parameters */
/* first -   first paramter if TRUE              */

    putString("not implemented yet\n");
    
}

/************************************************************************/
/*    doEnter() handles E(nter) command                */
/************************************************************************/
void doEnter(char moreYet, char first) {
/* moreYet - TRUE to accept following parameters */
/* first -   first parameter if TRUE             */

#define CONFIGURATION    0
#define MESSAGE     1
#define PASSWORD    2
#define ROOM        3
#define FILE        4

    char what;            /* one of above five */
    char abort, done, WC;


    if (moreYet) first = '\0';

    abort = FALSE;
    done  = FALSE;
    WC    = FALSE;
    what  = MESSAGE;

    putString("Enter ");

    if (!loggedIn && !unlogLoginOk && thisRoom != MAILROOM) {
        
        putString("--Must log in to enter (except in Mail>)\n ");
        return;

    }

    if (first) oChar(first);

    do {

        switch (toupper(first ? first : iChar())) {
            case '\r':
            case '\n':
               moreYet = FALSE;
               break;
            case 'F':
                if (roomBuf.rbflags & CPMDIR) {
                    putString("File upload ");
                    what = FILE;
                    done = TRUE;
                    break;
                }
            default:
                putString("? ");
                abort    = TRUE;
                if (expert)  break;
            case '?':
                tutorial("entopt.mnu");
                abort    = TRUE;
                break;
            case 'C':
                putString("Configuration ");
                what    = CONFIGURATION;
                done    = TRUE;
                break;
            case 'M':
                putString("Message ");
                what    = MESSAGE;
                done    = TRUE;
                break;
            case 'P':
                putString("Password ");
                what    = PASSWORD;
                done    = TRUE;
                break;
            case 'R':
                putString("Room ");
                if (!nonAideRoomOk && !aide) {
                    putString(" ?-- must be aide to create room\n ");
                    abort    = TRUE;
                    break;
                }
                what    = ROOM;
                done    = TRUE;
                break;
        }

        first = '\0';

    } while (!done && moreYet && !abort);

    doCR();

    if (!abort) {

        switch (what) {
            case CONFIGURATION:
                configure();
                break;
            case MESSAGE:
                makeMessage(WC);
                break;
            case PASSWORD:
                newPW();
                break;
            case ROOM:
                makeRoom();
                break;
            case FILE:
                upLoad();
                break;
        }

    }

}

/************************************************************************/
/*    doGoto() handles G(oto) command                                   */
/************************************************************************/
void doGoto(char expand, char first) {
/* expand - TRUE to accept following parameters */
/* first -  first parameter if TRUE             */

    char roomName[NAMESIZE];

    putString("Goto ");

    if (!expand) {
        gotoRoom("");
        return;
    }

    getString("", roomName, NAMESIZE);
    normalizeString(roomName);

    if (roomName[0] == '?') {

        listRooms(/* doDull== */ TRUE);

    } else {

        gotoRoom(roomName);

    }

}

/************************************************************************/
/*    doHelp() handles H(elp) command                                   */
/************************************************************************/
void doHelp(char expand, char first) {
/* expand - TRUE to accept following parameters */
/* first -  first parameter if TRUE             */

    char fileName[NAMESIZE];

    if (!expand) {

        putString("Help\n \n ");
        tutorial("dohelp.hlp");
        return;

    }

    putString("Help ");
    getString("", fileName, NAMESIZE);
    normalizeString(fileName);

    if (fileName[0] == '?') {

        tutorial("helpopt.hlp");

    } else {

        /* adding the extention makes things look simpler for        */
        /* the user... and restricts the files which can be read    */

        strcat(fileName, ".hlp");
        tutorial(fileName);

    }

}

/************************************************************************/
/*    doKnown() handles K(nown rooms) command.                          */
/************************************************************************/
void doKnown(char expand, char first) {
/* expand - TRUE to accept following parameters */
/* first -  first parameter if TRUE             */

    putString("Known rooms\n ");
    listRooms(/* doDull== */ TRUE);

}

/************************************************************************/
/*    doLogin() handles L(ogin) command                                 */
/************************************************************************/
void doLogin(char moreYet, char first) {
/* moreYet - TRUE to accept following parameters */
/* first -   first parameter if TRUE             */

    char passWord[NAMESIZE];

    putString("Login ");

    if (!moreYet) {

        putString("\n");

    }

    if (loggedIn) {

        putString("\n ?Already logged in!\n ");
        return;

    }

    getString(moreYet ? "" : " password", passWord, NAMESIZE);
    normalizeString(passWord);
    login(passWord);

}

/************************************************************************/
/*    doLogout() handles T(erminate) command                */
/************************************************************************/
void doLogout(char expand, char first) {
/* expand - TRUE to accept following parameters */
/* first -  first parameter if TRUE             */

    if (expand) first = '\0';

    putString("Terminate ");

    if (first) oChar(first);

    switch (toupper(first ? first : iChar())) {
        case '?':
            putString("\n Logout options:\n\n");
            putString("Quit-also\n");
            putString("Stay\n");
            break;
        case 'Q':
            putString("Quit-also\n");
            if (!expand)   {
                if (!getYesNo("confirm"))   break;
            }
            terminate( /* hangUp == */ TRUE);
            break;
        case 'S':
            putString("Stay\n ");
            terminate( /* hangUp == */ FALSE);
            break;
    }

}

/************************************************************************/
/*    doRead() handles R(ead) command                                   */
/************************************************************************/
void doRead(char moreYet, char first) {
/* moreYet - TRUE to accept following parameters */
/* first -   first parameter if TRUE             */

    char abort, doDir, done, hostFile, whichMess, revOrder, status, WC;
    char fileName[NAMESIZE];


    if (moreYet) first = '\0';

    putString("\bRead ");

    if (!loggedIn  &&  !unlogReadOk)   {

        putString("Must log in to read\n ");
        return;

    }

    if (first) oChar(first);

    abort     = FALSE;
    doDir     = FALSE;
    done      = FALSE;
    hostFile  = FALSE;
    revOrder  = FALSE;
    status    = FALSE;
    whichMess = newOnly;

    do {

        switch (toupper(first ? first : iChar())) {
            case '\n':
            case '\r':
                moreYet = FALSE;
                break;
            case 'A':
                putString("All ");
                whichMess = oldAndNew;
                break;
            case 'F':
                putString("Forward ");
                revOrder  = FALSE;
                whichMess = oldAndNew;
                break;
            case 'G':
                putString("Global new-messages ");
                whichMess = globalNew;
                break;
            case 'N':
                putString("New ");
                whichMess = newOnly;
                break;
            case 'O':
                putString("Old ");
                revOrder  = TRUE;
                whichMess = oldOnly;
                break;
            case 'R':
                putString("Reverse ");
                revOrder  = TRUE;
                whichMess = oldAndNew;
                break;
            case 'S':
                putString("Status ");
                status = TRUE;
                done   = TRUE;
                break;
            case 'W':
                putString("WC protocol ");
                WC = TRUE;
                break;
            case 'B':
                if (roomBuf.rbflags & CPMDIR) {
                    putString("Binary file(s) ");
                    done     = TRUE;
                    hostFile = TRUE;
                    break;
                }
            case 'D':
                if (roomBuf.rbflags & CPMDIR) {
                    putString("Directory ");
                    doDir = TRUE;
                    done  = TRUE;
                    break;
                }
            case 'T':
                if (roomBuf.rbflags & CPMDIR) {
                    putString("Textfile(s) ");
                    done     = TRUE;
                    hostFile = TRUE;
                    break;
                }
            case '?':
                tutorial("readopt.mnu");
                abort = TRUE;
                break;
            default:
                putString("? ");
                abort = TRUE;
                setUp(FALSE);
                if (expert) break;
        }

        first = '\0';

    } while (!done && moreYet && !abort);

    if (abort) return;

    if (status) {

        systat();
        return;

    }

    if (doDir) {

        FDSectCount = 0;    /* global fileDir() totals sectors in    */

        getString("", fileName, NAMESIZE);

        normalizeString(fileName);

        if (strlen(fileName)) {

            wildCard(fileDir, fileName);

        } else {

            wildCard(fileDir, "*.*");

        }

        putString("\n %d sectors total\n ", FDSectCount);
        return;

    }

    if (hostFile) {

        getString("afn", fileName, NAMESIZE);
        normalizeString(fileName);
        /* wildCard(transmitFile, fileName); */
        return;

    }

    doCR();

    if (whichMess != globalNew) {

        showMessages(whichMess, revOrder);

    } else {

        while (gotoRoom("")) {

            givePrompt();
            putString("read new\n ");
            showMessages(newOnly, revOrder);

        }

    }

}

/************************************************************************/
/*    doRegular()                            */
/************************************************************************/
char doRegular(char x, char c) {

    char toReturn;

    toReturn = FALSE;

    switch (c) {
        case 'C': doChat(  x, '\0');            break;
        case 'E': doEnter( x, 'm' );            break;
        case 'F': doRead(  x, 'f' );            break;
        case 'G': doGoto(  x, '\0');            break;
        case 'H': doHelp(  x, '\0');            break;
        case 'K': doKnown( x, '\0');            break;
        case 'L': doLogin( x, '\0');            break;
        case 'N': doRead(  x, 'n' );            break;
        case 'O': doRead(  x, 'o' );            break;
        case 'R': doRead(  x, 'r' );            break;
        case 'T': doLogout(x, 'q' );            break;
        case 0:
            if (newCarrier)   {
                greeting();
                newCarrier = FALSE;
            }
            if (justLostCarrier) {
                justLostCarrier = FALSE;
                if (loggedIn) terminate(FALSE);
            }
            break;    /* irrelevant value */
        case '?':
            tutorial("mainopt.mnu");
            break;
        case 'A': 
            if (!doAide(x, 'E')) {
                toReturn = TRUE; 
            }
            break;
        default:
            if (!expert) {
                putString("\n ? (Type '?' for menu)\n \n");
            } else {
                putString("\n ?\n \n");
            }
            toReturn = TRUE;
            break;
    }

    return toReturn;

}

/************************************************************************/
/*    doSysop() handles the sysop-only menu                             */
/*        return FALSE to fall invisibly into default error msg         */
/************************************************************************/
char doSysop(char x, char c) {

    char   who[NAMESIZE];
    struct logBuffer lBuf;
    int    logNo, ltabSlot;
    char toReturn;

    toReturn = FALSE;

    putString("\n privileged fn: ");

    switch (c) {
        case 'A': 
            putString("Abort\n "); 
            sysop = FALSE;
            break;
        case 'C':  
            putString("Chat mode disabled\n"); 
            break;
        case 'D':
            debug = !debug;
            putString("Debug switch=%d\n \n", debug);
            break;
        case 'K':
            putString("Kill account\n ");
            if (!getYesNo("Confirm")) break;
            getString("who", who, NAMESIZE);
            normalizeString(who);
            logNo = findPerson(who, &lBuf);
            if (logNo == ERROR)   {
                putString("No such person\n ");
                break;
            }
            putString("%s deleted\n ", who);
            ltabSlot = PWSlot(lBuf.lbpw);
            lBuf.lbname[0] = '\0';
            lBuf.lbpw[0  ] = '\0';
            putLog(&lBuf, logNo);
            logTab[ltabSlot].ltpwhash = 0;
            logTab[ltabSlot].ltnmhash = 0;
            break;
        case 'M':
            putString("\bSystem now on MODEM\n ");
            setUp(FALSE);
            putString("Chat mode disabled\n");
            if (debug)       putString("Debug mode on\n "  );
            if (visibleMode) putString("Visible mode on\n ");
            return FALSE;
        case 'P':
            putString("\bAide privilege set/clear\n ");
            getString("who", who, NAMESIZE);
            normalizeString(who);
            logNo = findPerson(who, &lBuf);
            if (logNo == ERROR) {
                putString("No such person\n ");
                break;
            }
            lBuf.lbflags ^= AIDE;
            putString("%s %s aide privileges\n ", who,
                     (lBuf.lbflags & AIDE) ? "gets" : "loses"
            );
            if (!getYesNo("Confirm")) break;
            putLog(&lBuf, logNo);
            /* see if it is us: */
            if (loggedIn && strcasecmp(logBuf.lbname, who)==SAMESTRING) {
                aide = lBuf.lbflags & AIDE;
            }
            break;
        case 'R':
            putString("not implemented\n");
            break;
        case 'S':
            putString("Set date\n \n");
            break;
        case 'V':
            putString(" VisibleMode==%d\n ",  visibleMode = !visibleMode);
            break;
        case 'T': 
            doLogout(x, 'q' );            
            toReturn = FALSE;
            break;
        case '?':
            tutorial("ctdlopt.mnu");
            break;
        default:
            if (!expert) {
                putString("\n ?(Type '?' for menu)\n");
            } else {
                putString("\n ?\n");
            }
            toReturn = TRUE;
            break;
    }

    return toReturn;

}

/************************************************************************/
/*    getCommand() prints menu prompt and gets command char             */
/*    Returns: char via parameter and expand flag as value  --          */
/*         i.e., TRUE if parameters follow else FALSE.                  */
/************************************************************************/
char getCommand(char *c) {

    char expand;

    /*if (*c)*/ givePrompt();
    
    while (BBSCharReady()) iChar();          /* eat type-ahead       */

    *c = toupper(iChar());

    expand = (
        *c == ' ' ||
        *c == '.' ||
        *c == ',' ||
        *c == '/'
    );

    if (expand) *c = toupper(iChar());

    return expand;

}

/************************************************************************/
/*    greeting() gives system-entry blurb etc                           */
/************************************************************************/
void greeting(void) {

    int year, month, day;

    if (loggedIn) terminate(FALSE);

    setUp(TRUE);     
    usleep(10);

    putString("\n Welcome to Citadel");
    putString("\n V2.1 at %s \n \n ", nodeTitle);
    getDate(&year, &month, &day);
    printDate(year, month, day);

    putString("\n H for Help\n ");

    putString("\n 'MODEM' mode.\n");
    putString("(<ESC> for CONSOLE mode.)\n");

    gotoRoom("Lobby");

}

/************************************************************************/
/*    main() contains the central menu code                             */
/************************************************************************/
int main(int argc, char **argv) {

    char c, x;
    char opts[] = "csh?";
    extern char *optarg;
    extern int optind;
    char *configs = "ctdlcnfg.sys";

    opterr = 0;

    while ((c = getopt(argc, argv, opts)) != -1) {

        switch(c) {
            case 'c':
                configs = argv[optind];
                break;
            case 's':
                sysop = TRUE;
                break;
            case 'h':
                printf("\n");
                printf("Citadel 2.10 - Linux port\n");
                printf("\n");
                printf("Usage: 210ctdl [-h] [-s] [-c <filename>]\n");
                printf("\n");
                printf("    -h - display help.\n");
                printf("    -s - enable sysop options.\n");
                printf("    -c <filename> - use this configuration file.\n");
                printf("\n");
                return EXIT_SUCCESS;
                break;
            case '?':
                printf("Usage: 210ctdl [-h] [-s] [-c <filename>]\n");
                return EXIT_SUCCESS;
                break;
        }

    }

    initTerminal();
    loadConfig(configs);
    initCitadel();
    weAre = CITADEL;
    greeting();

    while (!exitToCpm) {

        x = getCommand(&c);

        if (c == CNTRLp) sysop = TRUE;

        (sysop) ? doSysop(x, c) : doRegular(x, c);

    }

    if (loggedIn) terminate( /* hangUp == */ TRUE);

    endTerminal();

    return(EXIT_SUCCESS);

}

