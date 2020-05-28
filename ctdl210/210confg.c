
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
#include <unistd.h>

#include "210ctdl.h"
#include "210protos.h"

#define MAXCODE 256

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

    exitToCpm   = FALSE;        /* not time to quit yet!    */
    sizeLTentry = sizeof(logTab[0]);    /* just had to try that feature */

    pullMessage = FALSE;        /* not pulling a message    */
    pulledMLoc  = ERROR;        /* haven't pulled one either    */
    pulledMId   = ERROR;

    debug       = FALSE;
    loggedIn    = FALSE;
    termWidth   = 79;
    termUpper   = FALSE;
    termTab     = FALSE;

    /* open message file */

    if ((msgfl = open("ctdlmsg.sys", O_RDWR)) == ERROR) {

        printf(" ctdlmsg.sys not found, creating new file. \n");
        printf(" (Be sure to initialize it!)\n");

        if ((msgfl = creat("ctdlmsg.sys", 0660)) == ERROR) {

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

        if ((roomfl = creat("ctdlroom.sys", 0660)) == ERROR) {

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

        if ((logfl = creat("ctdllog.sys", 0660)) == ERROR) {

            printf("?can't create ctdllog.sys!!\n");
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

}

/************************************************************************/
/*    main() for confg.c                                                */
/************************************************************************/

int main(void) {

    int fd;
    char fBuf[BUFSIZ];               
    char line[128];               
    char codeBuf[MAXCODE];
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

            if (strcmp(cmd, "#define" ) == SAMESTRING && args == 3) {

                printf("#define '%s' as %x\n", var, arg);

                if (strcmp(var, "MDATA") == SAMESTRING) {

                    mData = arg;

                } else if (strcmp(var, "MEGAHZ") == SAMESTRING) {

                    megaHz = arg;

                } else if (strcmp(var, "CRYPTSEED") == SAMESTRING) {

                    cryptSeed = arg;

                } else if (strcmp(var, "RCPM") == SAMESTRING) {

                    rcpm = arg;

                } else if (strcmp(var, "CLOCK") == SAMESTRING) {

                    /* clock = arg; */

                } else if (strcmp(var, "MESSAGEK") == SAMESTRING) {

                    maxMSector = arg * 8;

                } else if (strcmp(var, "MSGDISK") == SAMESTRING) {

                    msgDisk = arg;

                } else if (strcmp(var, "HOMEDISK") == SAMESTRING) {

                    homeDisk = arg;

                } else if (strcmp(var, "HOMEUSER") == SAMESTRING) {

                    homeUser = arg;

                } else if (strcmp(var, "LOGINOK") == SAMESTRING) {

                    unlogLoginOk = arg;

                } else if (strcmp(var, "ENTEROK") == SAMESTRING) {

                    unlogEnterOk = arg;

                } else if (strcmp(var, "READOK") == SAMESTRING) {

                    unlogReadOk = arg;

                } else if (strcmp(var, "ROOMOK") == SAMESTRING) {

                    nonAideRoomOk = arg;

                } else {

                    printf("? -- no variable '%s' known! -- ignored.\n", var);

                }

            } else if (strcmp(cmd, "#nodeTitle") == SAMESTRING) {

                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nodeTitle, string);

            } else if (strcmp(cmd, "#nodeName") == SAMESTRING) {

                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nodeName, string);

            } else if (strcmp(cmd, "#nodeId") == SAMESTRING) {

                /* reparse by ";" terminator: */
                sscanf(line, "%s \"%s\"", cmd, string);
                /* copy string into code buffer: */
                strcpy(nodeId, string);

            } else if (strcmp(cmd, "#alldone") == SAMESTRING) {

                break;

            } else if (cmd[0] == '#') printf("? -- no '%s' command!\n", cmd);

        }

    }

    init();
    wrapup();

    return(EXIT_SUCCESS);

}

