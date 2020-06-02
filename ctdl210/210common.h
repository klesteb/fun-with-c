
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2020 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

/************************************************************************/
/*                             common.h                                 */
/*                                                                      */
/*       Common variables for the Citadel bulletin board system         */
/************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <ncurses.h>

extern char firstExtern;

extern char mData;
extern char megaHz;
extern char nodeName[32];
extern char nodeTitle[32];
extern char nodeId[32];
extern int  cryptSeed;
extern char rcpm;
extern char homeDisk;
extern char homeUser;
extern char msgDisk;
extern char ourDisk;
extern char ourUser;
extern char unlogEnterOk;
extern char unlogReadOk;
extern char unlogLoginOk;
extern char nonAideRoomOk;
extern WINDOW *outWin;
extern WINDOW *inpWin;
extern char weAre;
extern struct roomTable roomTab[MAXROOMS];
extern struct roomBuffer roomBuf;
extern int  thisRoom;
extern int  roomfl;
extern struct logBuffer logBuf;
extern int  thisLog;
extern int  thisSlot;
extern int  logfl; 
extern struct logTable logTab[MAXLOGTAB];
extern int  sizeLTentry;
extern char loggedIn;
extern char *monthTab[13];
extern char prevChar; 
extern char expert;
extern char termWidth;
extern char termUpper;
extern char termLF;
extern char termTab;
extern char aide;
extern char termNulls;
extern char justLostCarrier;
extern char crtColumn;
extern char newCarrier;
extern char onConsole;
extern struct msgBuffer msgBuf;
extern int  msgfl;
extern unsigned char sectBuf[SECTSIZE];
extern unsigned char GMCCache;
extern unsigned char  oldChar;
extern unsigned short oldSector;
extern unsigned char  thisChar;
extern unsigned short thisSector;
extern unsigned char  catChar;
extern unsigned short catSector;
extern unsigned short oldestLo;
extern unsigned short oldestHi;
extern unsigned short newestLo;
extern unsigned short newestHi;
extern int  maxMSector;
extern char visibleMode;
extern char exitToCpm;
extern char *fpc1;
extern char *fpc2;
extern char *fpc3;
extern char fc1;
extern char fc2;
extern char fc3;
extern int  fi1;
extern int  fi2;
extern int  fi3;
extern char pullMessage;
extern int  pulledMLoc;
extern int  pulledMId;
extern int  FDSectCount;
extern int  debug;

extern char lastExtern;

#endif

