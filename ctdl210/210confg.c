
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
#include "210common.c"

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

    /* open message file */

    if ((msgfl = open("ctdlmsg.sys", O_RDWR)) == ERROR) {

        putString(" ctdlmsg.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((msgfl = creat("ctdlmsg.sys", 0660)) == ERROR) {

            putString("?can't create ctdlmsg.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(msgfl);    /* can't create in mode 2! */
        msgfl = open("ctdlmsg.sys", O_RDWR);

    }

    /* open room file */

    if ((roomfl = open("ctdlroom.sys", O_RDWR)) == ERROR) {

        putString(" ctdlroom.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((roomfl = creat("ctdlroom.sys", 0660)) == ERROR) {

            putString("?can't create ctdlroom.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(roomfl);     /* can't create in mode 2! */
        roomfl = open("ctdlroom.sys", O_RDWR);

    }

    /* open userlog file */

    if ((logfl = open("ctdllog.sys", O_RDWR)) == ERROR) {

        putString(" ctdllog.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((logfl = creat("ctdllog.sys", 0660)) == ERROR) {

            putString("?can't create ctdllog.sys!!\n");
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

    msgInit();
    putString("Press <return> to continue.\n");
    getCh();

    indexRooms();
    putString("Press <return> to continue.\n");
    getCh();

    logInit();
    putString("Press <return> to continue.\n");
    getCh();

}

/************************************************************************/
/*    main() for confg.c                                                */
/************************************************************************/

int main(void) {

    initTerminal();
    loadConfig("ctdlcnfg.sys");
    init();
    wrapup();
    endTerminal();

    return(EXIT_SUCCESS);

}

