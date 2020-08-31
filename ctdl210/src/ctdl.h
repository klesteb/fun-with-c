
/************************************************************************/
/*                Ctdl210.h                                             */
/*                                                                      */
/*    #include file for all Citadel C files.                            */
/************************************************************************/

#ifndef _CTDL_H
#define _CTDT_H

/************************************************************************/
/*                History                                               */
/*                                                                      */
/* 20May29 KLE    Reformatted to work on Linux                          */
/* 83Mar03 CrT    Changed from 2 to 3 sectors / log-record (more mail)  */
/* 83Feb23        termTab, TABMASK                                      */
/* 82Dec16 dvm    Place for #defines added                              */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov02 CrT    General cleanup begun, preparatory to V1.2 mods.      */
/* 82Nov01 CrT    Proofread for distribution version.                   */
/* 82Oct05 dvm    distribution version mods begun                       */
/* 82Jun03 dvm    MAXROOMS reduced to 64                                */
/* 82Apr22 dvm    mods for HiTech/NMI version completed                 */
/* 81Dec19 CrT    Rooms seem to be working...                           */
/* 81Dec12 CrT    Started.                                              */
/************************************************************************/

/************************************************************************/
/*    stuff to distinguish the various Citadel programs                 */
/************************************************************************/

#define CITADEL  0      /* prinipal program               */
#define xxxxx    1      /* unused                         */
#define NET      2      /* network downloader    (future) */
#define ARCHIVE  3      /* backup program        (future) */

/************************************************************************/
/*            Stuff to size the system with:                            */
/************************************************************************/

#define NAMESIZE  20   /* length of room names                      */
#define SECTSIZE  128  /* sector size for messages (don't change)   */
#define FILSPERRM 10   /* roombuf must be 256 bytes or less.        */
#define MAXVISIT  8    /* #visits we remember old newestLo for      */
#define MAXTEXT   3500 /* maximum chars in edit buffer              */

/* WARNING!!! if you expand MAXROOMS beyond 128, horrible, horrible */
/* things will happen to the userlog entries, which will no longer  */
/* fit in the assigned 256-byte records and will overwrite things   */
/* with normally undesirable results. 82Nov10CrT                    */

#ifdef TEST
#define MAXROOMS  6    /* number of rooms allowed in system    */
#define MAXLOGTAB 4    /* number of log entries supported      */
#define MSGSPERRM 58   /* should be >= MAILSLOTS               */
#define MAILSLOTS 58   /* twiddle to fit logBuf in 384 bytes   */
#else
#define MAXROOMS  64   /* number of rooms allowed in system    */
#define MAXLOGTAB 180  /* number of log entries supported      */
#define MSGSPERRM 58   /* roombuf must be 256 bytes or less.   */
#define MAILSLOTS 58   /* twiddle to fit logBuf in 384 bytes   */
#endif

/* #define SECSPERROOM 2 */  /* sectors/room                      */
/* #define SECSPERLOG  3 */  /* three sectors per log record      */
/* #define ROOMSECTORS (MAXROOMS * SECSPERROOM) */
/* #define LOGSECTORS  (MAXLOGTAB * SECSPERLOG) */

/************************************************************************/
/*        Stuff nowadays usually in bdscio.h:                           */
/************************************************************************/

/* values for functions to return: */
#ifndef TRUE
#define TRUE   (1)
#define FALSE  (0)
#endif
#ifndef ERR
#define ERR    (-1)
#define OK     (0)
#endif
#define ERROR  (-1)
#define SAMESTRING (0)    /* value for strcmp() & friend        */

/* ASCII characters: */
#define NUL        0
#define SOH        1
#define CNTRLC     3
#define EOT        4
#define ACK        6
#define BELL       7
#define BACKSPACE  8
#define CNTRLI     9    /* aka tab                    */
#define TAB        9    /* aka ^I                     */
#define NEWLINE    10   /* "linefeed" to philistines. */
#define CNTRLO     15
#define CNTRLp     16
#define XOFF       19   /* control-s                  */
#define NAK        21
#define CNTRLZ     26
#define CPMEOF     CNTRLZ
#define ESC        27   /* altmode                    */
#define DEL        0x7F /* delete char                */

/* for packing & unpacking ints: */
#define BYTE 256

/************************************************************************/
/*            Stuff for rooms:                                          */
/************************************************************************/

#define LOBBY    0      /* Lobby> is >always< room 0. */
#define MAILROOM 1      /* Mail>  is >always< room 1. */
#define AIDEROOM 2      /* Aide> is >always< room 2.  */

#define INUSE    1      /* flags mask */
#define PUBLIC   2      /* flags mask */
#define CPMDIR   4      /* flags mask */
#define PERMROOM 8      /* flags mask */

struct roomTable {
    char rtgen;                 /* generation # of room             */
    char rtflags;               /* public/private flag etc          */
    char rtname[NAMESIZE];      /* name of room                     */
    unsigned short rtlastMessage;   /* # of most recent message in room */
};

struct roomBuffer {
    char rbgen;                 /* generation # of room            */
    char rbflags;               /* same bits as flags above        */
    char rbname[NAMESIZE];      /* name of room                    */
    char rbdisk;                /* disk this rooms files are in 0=>none */
    char rbuser;                /* user area for this rooms files  */
    union {
        struct {
            unsigned short rbmsgNo;  /* every message gets unique#  */
            unsigned short rbmsgLoc; /* sector message starts in    */
        } msg[MSGSPERRM];
    } vp;                       /* variable-part               */
};

/************************************************************************/
/*                  userlog stuff                                       */
/************************************************************************/

#define CRYPTADD  117   /*                                      */
#define UCMASK    1     /* bit uppercase flag is in             */
#define LFMASK    2     /* bitmask for linefeed flag            */
#define EXPERT    4     /* bitmask for expert-user flag         */
#define TABMASK   8     /* bitmask for tab flag                 */
#define AIDE      16    /* bitmask for aide flag                */
#define GENSHIFT  3     /* lbgen >> GENSHIFT gives generation   */
#define CALLMASK  7     /* lbgen &  CALLMASK gives last visit   */
#define MAXGEN    32    /* six bits of generation => 64 of them */

/* logbuf must be 384 bytes or less... including lbOverFlow, 384 or MORE */

struct logBuffer {
    char lbnulls;                      /* #nulls, lCase, lFeeds                */
    char lbflags;                      /* UCMASK, LFMASK, EXPERT, TABMASK, AIDE*/
    char lbwidth;                      /* terminal width                       */
    char lbname[NAMESIZE];             /* caller's name                        */
    char lbpw[NAMESIZE];               /* caller's password                    */
    char lbgen[MAXROOMS];              /* 6 bits gen, two bits lastvisit       */
    unsigned short lbvisit[MAXVISIT];  /* newestLo for this and 3 prev. visits */
    unsigned short lbslot[MAILSLOTS];  /* for private mail                     */
    unsigned short lbId[MAILSLOTS];    /* for private mail               */
#ifdef TEST
    char      lbOverFlow[256];
#else
    char      lbOverFlow[84];
#endif
};

struct logTable {
    unsigned short ltpwhash;        /* hash of password          */
    unsigned short ltnmhash;        /* hash of name              */
    unsigned short ltlogSlot;       /* location in userlog.buf   */
    unsigned short ltnewest;        /* last message on last call */
};

/************************************************************************/
/*            terminal stuff                                            */
/************************************************************************/

#define SPECIAL     27  /* <ESC>    console escape char         */
#define MONTHOFFSET 32  /* for packing month and year in a byte */

/************************************************************************/
/*            message stuff                                             */
/************************************************************************/

struct msgBuffer {
    char mbtext[MAXTEXT];           /* buffer text is edited in           */
    unsigned char  mbheadChar;      /* start of message                   */
    unsigned short mbheadSector;    /* start of message                   */
    char mbauth[NAMESIZE];          /* name of author                     */
    char mbdate[NAMESIZE];          /* creation date                      */
    char mbId[NAMESIZE];            /* local number of message            */
    char mboname[NAMESIZE];         /* short human name for origin system */
    char mborig[NAMESIZE];          /* US 206 633 3282 style ID           */
    char mbroom[NAMESIZE];          /* creation room                      */
    char mbsrcId[NAMESIZE];         /* message ID on system of origin     */
    char mbto[NAMESIZE];            /* private message to                 */
};

/* values for showMess routine */

#define newOnly   0
#define oldAndNew 1
#define oldOnly   2
#define globalNew 3

/************************************************************************/
/*        buffered file I/O stuff                                       */
/************************************************************************/
#define NSECTS  8       /* 8 sectors == 1024 byte buffer */
#define BUFSIZE (NSECTS * SECTSIZE + 6)

#endif

