
/************************************************************************/
/*                modem.c                                               */
/*                                                                      */
/*        modem code for Citadel bulletin board system                  */
/*    NB: this code is rather machine-dependent:  it will typically     */
/*    need some twiddling for each new installation.                    */
/*                  82Nov05 CrT                                         */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 83Mar01 CrT    FastIn() ignores LFs etc -- CRLF folks won't be trapped.*/
/* 83Feb25 CrT    Possible fix for backspace-in-message-entry problem.  */
/* 83Feb18 CrT    fastIn() upload mode cutting in on people.  Fixed.    */
/* 82Dec16 dvm    modemInit revised for FDC-1, with kludge for use with */
/*        Big Board development system                                  */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov15 CrT    readfile() & sendfile() borrowed from TelEdit.c       */
/* 82Nov05 CrT    Individual history file established                   */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <ncurses.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.h"

/************************************************************************/
/*                Contents                                              */
/*                                                                      */
/*    BBSCharReady()  returns true if user input is ready               */
/*    KBReady()       returns TRUE if a console char is ready           */
/*    getCh()         returns a console char                            */
/*    iChar()         top-level user-input function                     */
/*    modemInit()     top-level initialize-all-modem-stuff              */
/*    oChar()         top-level user-output function                    */
/*    putChar()                                                         */
/*    ringSysop()     signal chat-mode request                          */
/*                                                                      */
/*    # == routines you should certainly check when porting system      */
/************************************************************************/

/************************************************************************/
/*        The principal dependencies:                                   */
/*                                                                      */
/*  iChar  modIn                    outMod                              */
/*      modIn  getCh  mIReady  kBReady  outMod  carrDetect      */
/*          getMod                                                      */
/*               getCh                                                  */
/*                   mIReady                                            */
/*                       kBReady                                        */
/*                           carrDetect                                 */
/*                                                                      */
/*  oChar  outMod                                                       */
/*      outMod  mOReady                                                 */
/************************************************************************/


/************************************************************************/
/*    BBSCharReady() returns TRUE if char is available from user        */
/*    NB: user may be on modem, or may be sysop in CONSOLE mode         */
/************************************************************************/
char BBSCharReady(void) {

    return (KBReady());

}

/************************************************************************/
/*    getCh() reads a console char                                      */
/*        In CONSOLE mode, CRs are changed to newlines                  */
/*        Rubouts are changed to backspaces                             */
/*    Returns:    resulting char                                        */
/************************************************************************/
int getCh(void) {

    doupdate();
    return (int)getch();

}


/************************************************************************/
/*    iChar() is the top-level user-input function -- this is the       */
/*    function the rest of Citadel uses to obtain user input            */
/************************************************************************/
int iChar(void) {

    return (int)getCh();

}

/************************************************************************/
/*    KBReady() returns TRUE if a console char is ready                 */
/************************************************************************/
char KBReady(void) {

    chtype ch;
    char stat = ERR;

    if ((ch = getCh()) != ERR) {

        ungetch(ch);
        stat = OK;

    }

    return stat;

}

/************************************************************************/
/*    modemInit() is responsible for all modem-related initialization   */
/*    at system startup                                                 */
/*    Globals modified:    haveCarrier    visibleMode                   */
/*                whichIO     modStat                                   */
/*                exitToCpm    justLostCarrier                          */
/*    modified 82Dec10 to set FDC-1 SIO-B clock speed at                */
/*    300 baud     -dvm                                                 */
/************************************************************************/
void modemInit(void) {

    visibleMode = FALSE;
    exitToCpm   = FALSE;
    
}

/************************************************************************/
/*    oChar() is the top-level user-output function                     */
/*      sends to modem port and console both                            */
/*      does conversion to upper-case etc as necessary                  */
/*      in "debug" mode, converts control chars to uppercase letters    */
/*    Globals modified:    prevChar                                     */
/************************************************************************/
void oChar(char c) {

    addch((chtype)c);
    wnoutrefresh(stdscr);

}

/************************************************************************/
/*    putChar()                                                         */
/************************************************************************/
void putChar(char c) {

    if (thisRoom != 1) {

        addch((chtype)c);
        wnoutrefresh(stdscr);

    }

}

/************************************************************************/
/*    ringSysop() signals a chat mode request.  Exits on input from     */
/*  modem or keyboard.                                                  */
/************************************************************************/
void ringSysop(void) {

    putString("\n Ringing sysop.\n ");

}

/************************************************************************/
/*   initTerminal()  initialize the terminal                            */
/************************************************************************/
void initTerminal(void) {
    
    /* initiize curses */

    initscr();
    cbreak();
    /* noecho(); */
    keypad(stdscr, TRUE);

    erase();
    /* curs_set(0); */
    refresh();

    outWin = newwin(LINES - 2, COLS, 0, 0);
    if (outWin != NULL) {

        scrollok(outWin, TRUE);

    }

    inpWin = newwin(1, COLS, LINES, 1);

}

/************************************************************************/
/*   endTerminal()  closes the terminal                                 */
/************************************************************************/
void endTerminal(void) {

    delwin(inpWin);
    delwin(outWin);
    endwin();
    
}

/************************************************************************/
/************************************************************************/
void upLoad(void) {

    putString("not implemented\n ");

}

