
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
/*                             common.c                                 */
/*                                                                      */
/* Common variables for the Citadel bulletin board system               */
/* Initialize to known values                                           */
/************************************************************************/

char firstExtern = 0;        /* this should ALWAYS be the first declaration */

/* start config items */

int sysop = FALSE;           /* toggle sysop mode                    */
char mData = 0;              /* Modem data port                      */
char megaHz = 0;             /* Z80 clock rate (for wait loops)      */
char nodeName[32];
char nodeTitle[32];
char nodeId[32];
int  cryptSeed = 0;
char rcpm = FALSE;           /* TRUE to run in RCP/M mode            */
char weAre = 0;              /* set first thing by main()            */

char homeDisk = 0;           /* where we keep our data files         */
char homeUser = 0;           /* where we keep our data files         */
char msgDisk = 0;            /* where we keep our data files         */
char ourDisk = 0;            /* where we are just now                */
char ourUser = 0;            /* where we are just now                */
char msgPath[256];           /* where the messages are stored        */
char helpPath[256];          /* where the help files are stored      */
char menuPath[256];          /* where the menu files are stored      */

char unlogEnterOk = FALSE;   /* TRUE if OK to enter messages anon    */
char unlogReadOk = FALSE;    /* TRUE if unlogged folks can read mess */
char unlogLoginOk = FALSE;   /* TRUE if spontan. new accounts ok.    */
char nonAideRoomOk = FALSE;  /* TRUE general folks can make rooms    */

/* end config items */

char *monthTab[13] = { "", 
     "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" 
};

char visibleMode = FALSE;
char exitToCpm   = FALSE;
int  debug = FALSE;

/* rooms */

struct roomTable roomTab[MAXROOMS];
struct roomBuffer roomBuf;
int thisRoom = LOBBY;        /* room currently in roomBuf            */
int roomfl;                  /* room file handle                     */

/* log */

struct logTable logTab[MAXLOGTAB];
struct logBuffer logBuf;
int sizeLTentry = sizeof(logTab[0]);
int thisLog = 1;            /* entry currently in logBuf             */
int thisSlot = 0;           /* logTab slot logBuf was found via      */
int logfl;                  /* log file handle                       */
char loggedIn = FALSE;      /* global have-caller flag               */

/* messages */

struct msgBuffer msgBuf;
int msgfl;
unsigned char  sectBuf[SECTSIZE];
unsigned char  GMCCache = 0;
unsigned char  oldChar = 0;
long            oldSector = 0;
unsigned char  thisChar = 0;
long           thisSector = 0;
unsigned char  catChar = 0;
long           catSector = 0;
long           oldestLo = 0;
long           oldestHi = 0;
long           newestLo = 0;
long            newestHi = 0;
int maxMSector = 1;
char pullMessage = FALSE;
int  pulledMLoc  = ERROR;
int  pulledMId   = ERROR;
int  FDSectCount = 0;

/* terminal */

char prevChar = 0; 
char expert = FALSE;
char termWidth = 79;
char termUpper = FALSE;
char termLF = FALSE;
char termTab = FALSE;
char aide = FALSE;
char termNulls = FALSE;
char justLostCarrier = FALSE;
char crtColumn = FALSE;
char newCarrier = FALSE;
char onConsole = FALSE;
 
/* general purpose speed improvements? */

char *fpc1 = NULL;
char *fpc2 = NULL;
char *fpc3 = NULL;
char fc1 = 0;
char fc2 = 0;
char fc3 = 0;
int  fi1 = 0;
int  fi2 = 0;
int  fi3 = 0;

char lastExtern = 0;

