
/************************************************************************/
/*                confg.c                                               */
/*    configuration program for Citadel bulletin board system.          */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "210ctdl.h"

/************************************************************************/
/*                History                                               */
/*                                                                      */
/* 20May20 KLE    Reformated, removed cpmisms and ansified              */
/* 82Nov20 CrT    Created.                                              */
/************************************************************************/

/************************************************************************/
/*                Contents                                              */
/*                                                                      */
/*    init()      system startup initialization                         */
/*    main()                                                            */
/*    wrapup()    finishes and writes ctdlTabl.sys                      */
/************************************************************************/

/************************************************************************/
/*    init() -- master system initialization                            */
/************************************************************************/
void init(void) {

    char c, *msgFile;
    int mode = (S_IRWXU & S_IRWXG);
                
    echo            = BOTH;     /* echo input to console too    */
    usingWCprotocol = FALSE;

    exitToCpm   = FALSE;        /* not time to quit yet!    */
    sizeLTentry = sizeof(logTab[0]);    /* just had to try that feature */
    outFlag     = OUTOK;        /* not p(ausing)        */

    pullMessage = FALSE;        /* not pulling a message    */
    pulledMLoc  = ERROR;        /* haven't pulled one either    */
    pulledMId   = ERROR;

    debug       = FALSE;
    loggedIn    = FALSE;
    haveCarrier = FALSE;
    termWidth   = 79;
    termUpper   = FALSE;
    termTab     = FALSE;

    noChat      = FALSE;

    /* shave-and-a-haircut/two bits pause pattern for ringing sysop: */

    shave[0] = 40;
    shave[1] = 20;
    shave[2] = 20;
    shave[3] = 40;
    shave[4] = 80;
    shave[5] = 40;
    shave[6] = 250;

    /* initialize input character-translation table:    */

    for (c = 0; c < '\40'; c++) {
        filter[c] = '\0';        /* control chars -> nulls    */
    }
    for (c = '\40'; c < 128; c++) {
        filter[c] = c;            /* pass printing chars        */
    }

    filter[SPECIAL]   = SPECIAL;
    filter[CNTRLp]    = CNTRLp;
    filter[DEL]       = BACKSPACE;
    filter[BACKSPACE] = BACKSPACE;
/*  filter[CNTRLI]    = CNTRLI;    */
    filter[XOFF]      = 'P';
    filter['\r']      = NEWLINE;
    filter[CNTRLO]    = 'N';

    /* open message file */

    msgFile    = "ctdlmsg.sys";
    *msgFile   += msgDisk;

    if ((msgfl = open(msgFile, O_RDWR)) == ERROR) {

        printf(" %s not found, creating new file. \n", msgFile);
        printf(" (Be sure to initialize it!)\n");

        if ((msgfl = creat("ctdlmsg.sys", mode)) == ERROR) {

            printf("?can't create ctdlmsg.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(msgfl);    /* can't create in mode 2! */
        msgfl = open("ctdlmsg.sys", O_RDWR);

    }

    /* open room file */

    if ((roomfl = open("ctdlroom.sys", O_RDWR)) == ERROR) {

        printf(" ctdlroom.sys not found, creating new file. \n");
        printf(" (Be sure to initialize it!)\n");

        if ((roomfl = creat("ctdlroom.sys", mode)) == ERROR) {

            printf("?can't create ctdlroom.sys!!\n");
            exit(EXIT_FAILURE);

        }
        
        close(roomfl);     /* can't create in mode 2! */
        roomfl = open("ctdlroom.sys", O_RDWR);

    }

    /* open userlog file */

    if ((logfl = open("ctdllog.sys", O_RDWR)) == ERROR) {

        printf(" ctdllog.sys not found, creating new file. \n");
        printf(" (Be sure to initialize it!)\n");

        if ((logfl = creat("ctdllog.sys", mode)) == ERROR) {

            printf("?can't create ctdlLog.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(logfl);    /* can't create in mode 2! */
        logfl = open("ctdllog.sys", O_RDWR);

    }

    if (getYesNo("\n Erase and initialize log, message and/or room files")) {
        
        /* each of these has an additional go/no-go interrogation: */

        zapMsgFile();
        zapRoomFile();
        zapLogFile();

    }

}

/************************************************************************/
/*    wrapup() finishes up and writes ctdlTabl.sys out, finally         */
/************************************************************************/
void wrapup(void) {

    printf("\ncreating ctdlTabl.sys table\n");
    msgInit();
    indexRooms();
    logInit();
    printf("writeSysTab =%d\n", writeSysTab());

}

/************************************************************************/
/*    main() for confg.c                                                */
/************************************************************************/

int main(void) {

    int fd;
    char fBuf[BUFSIZ];               
    char line[128];               
    char cmd[128], var[128], string[128];   
    int  arg, arg2, args;
    union {
        int  *pi;
        char *pc;
    } nextCode;     /* where to deposit next code */


    if ((fd = open("ctdlcnfg.sys", O_RDONLY)) == ERROR) {

        printf("?Can't find ctdlcnfg.sys!\n");
        exit(EXIT_FAILURE);

    }

    nextCode.pc = &codeBuf[0];

    while ((read(fd, line, 128) > 0)) {

        if (args = sscanf(line, "%s %s %x ", cmd, var, &arg)) {
            if (strcmp(cmd, "#define" ) == SAMESTRING  &&  args==3) {
                printf("#define '%s' as %x\n", var, arg);
                if (strcmp(var, "MDATA"       )    == SAMESTRING) {
                    mData    = arg;
                } else if (strcmp(var, "MEGAHZ"    )    == SAMESTRING) {
                    megaHz    = arg;
                } else if (strcmp(var, "CRYPTSEED" )    == SAMESTRING) {
                    cryptSeed    = arg;
                } else if (strcmp(var, "RCPM"       )    == SAMESTRING) {
                    rcpm    = arg;
                } else if (strcmp(var, "CLOCK"       )    == SAMESTRING) {
                    clock    = arg;
                } else if (strcmp(var, "MESSAGEK"  )    == SAMESTRING) {
                    maxMSector    = arg*8;
                } else if (strcmp(var, "MSGDISK"   )    == SAMESTRING) {
                    msgDisk    = arg;
                } else if (strcmp(var, "HOMEDISK"  )    == SAMESTRING) {
                    homeDisk    = arg;
                } else if (strcmp(var, "HOMEUSER"  )    == SAMESTRING) {
                    homeUser    = arg;
                } else if (strcmp(var, "LOGINOK"   )    == SAMESTRING) {
                    unlogLoginOk= arg;
                } else if (strcmp(var, "ENTEROK"   )    == SAMESTRING) {
                    unlogEnterOk= arg;
                } else if (strcmp(var, "READOK"    )    == SAMESTRING) {
                    unlogReadOk = arg;
                } else if (strcmp(var, "ROOMOK"    )    == SAMESTRING) {
                    nonAideRoomOk=arg;
                } else {
                    printf("? -- no variable '%s' known! -- ignored.\n", var);
                }

            } else if (strcmp(cmd, "#start"  ) == SAMESTRING) {
                printf("#start procedure '%s'\n", var);
                if      (strcmp(var, "HANGUP"    ) == SAMESTRING) {
                    pHangUp    = nextCode.pc;
                } else if (strcmp(var, "INITPORT"  ) == SAMESTRING) {
                    pInitPort    = nextCode.pc;
                } else if (strcmp(var, "CARRDETECT") == SAMESTRING) {
                    pCarrDetect = nextCode.pc;
                } else if (strcmp(var, "MIREADY"   ) == SAMESTRING) {
                    pMIReady    = nextCode.pc;
                } else if (strcmp(var, "MOREADY"   ) == SAMESTRING) {
                    pMOReady    = nextCode.pc;
                } else if (strcmp(var, "INITDATE"  ) == SAMESTRING) {
                    pInitDate    = nextCode.pc;
                } else if (strcmp(var, "GETDAY"    ) == SAMESTRING) {
                    pGetDay    = nextCode.pc;
                } else if (strcmp(var, "GETMONTH"  ) == SAMESTRING) {
                    pGetMonth    = nextCode.pc;
                } else if (strcmp(var, "GETYEAR"   ) == SAMESTRING) {
                    pGetYear    = nextCode.pc;
                } else printf("?--no procedure '%s' known!\n", var);
                
            } else if (strcmp(cmd, "#code"   ) == SAMESTRING) {
                printf("#code '%s'\n", var);
                if      (strcmp(var, "LOAD"       ) == SAMESTRING) {
                    *nextCode.pc++    = LOAD;
                    *nextCode.pi++    = arg;
                } else if (strcmp(var, "ANDI"       ) == SAMESTRING) {
                    *nextCode.pc++    = ANDI;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "ORI"       ) == SAMESTRING) {
                    *nextCode.pc++    = ORI;
                    *nextCode.pc++    =arg;
                } else if (strcmp(var, "XORI"       ) == SAMESTRING) {
                    *nextCode.pc++    = XORI;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "STORE"       ) == SAMESTRING) {
                    *nextCode.pc++    = STORE;
                    *nextCode.pi++    = arg;
                } else if (strcmp(var, "LOADI"       ) == SAMESTRING) {
                    *nextCode.pc++    = LOADI;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "RET"       ) == SAMESTRING) {
                    *nextCode.pc++    = RET;
                } else if (strcmp(var, "INP"       ) == SAMESTRING) {
                    *nextCode.pc++    = INP;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "OUTP"       ) == SAMESTRING) {
                    *nextCode.pc++    = OUTP;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "PAUSEI"    ) == SAMESTRING) {
                    *nextCode.pc++    = PAUSEI;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "ARRAY[]="  ) == SAMESTRING) {
                    *nextCode.pc++    = STOREX;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "ARRAY[]"   ) == SAMESTRING) {
                    *nextCode.pc++    = LOADX;
                    *nextCode.pc++    = arg;
                } else if (strcmp(var, "OPR#"       ) == SAMESTRING) {
                    *nextCode.pc++    = OPRNUMBER;
                    
                    /* reparse to pick up string: */
                    sscanf(line, "%s %s \"%s\" %d %d",
                           cmd, var, string, &arg, &arg2
                           );
                    /* copy string into code buffer: */
                    strcpy(nextCode.pc, string);
                    while (*nextCode.pc++);    /* step over string    */
                    *nextCode.pc++    = arg;    /* lower limit        */
                    *nextCode.pc++    = arg2; /* upper limit        */
                    
                } else if (strcmp(var, "OUTSTRING" ) == SAMESTRING) {
                    *nextCode.pc++    = OUTSTRING;
                    
                    /* reparse to pick up string: */
                    sscanf(line, "%s %s \"%s\"", cmd, var, string);
                    /* copy string into code buffer: */
                    strcpy(nextCode.pc, string);
                    while (*nextCode.pc++);    /* step over string    */
                    nextCode.pc--;
                    *nextCode.pc++    = '\r'; /* add a CR        */
                    *nextCode.pc++    = '\0'; /* tie off with null    */
                } else printf("?--no code '%s'!\n", var);

            } else if (strcmp(cmd, "#nodeTitle") == SAMESTRING) {
                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nextCode.pc, string);
                nodeTitle    = nextCode.pc;
                while (*nextCode.pc++);    /* step over string    */
            } else if (strcmp(cmd, "#nodeName" ) == SAMESTRING) {
                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nextCode.pc, string);
                nodeName    = nextCode.pc;
                while (*nextCode.pc++);    /* step over string    */
            } else if (strcmp(cmd, "#nodeId"   ) == SAMESTRING) {
                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nextCode.pc, string);
                nodeId    = nextCode.pc;
                while (*nextCode.pc++);    /* step over string    */
            } else if (strcmp(cmd, "#alldone") == SAMESTRING) {
                break;
            } else if (cmd[0] == '#') printf("? -- no '%s' command!\n", cmd);
        }
    }
    if (nextCode.pc < &codeBuf[MAXCODE]) {

        init();
        wrapup();

    } else {

        printf("\7codeBuf[] overflow! Recompile with larger MAXCODE "    );
        printf("or reduce ctdlcnfg.sys\7"                );

    }

    return(EXIT_SUCCESS);

}

