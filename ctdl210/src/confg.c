
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

#include "ctdl.h"
#include "protos.h"
#include "common.c"

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

    if ((msgfl = open("../ctdlmsg.sys", O_RDWR)) == ERROR) {

        putString(" ctdlmsg.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((msgfl = creat("../ctdlmsg.sys", 0660)) == ERROR) {

            putString("?can't create ctdlmsg.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(msgfl);    /* can't create in mode 2! */
        msgfl = open("../ctdlmsg.sys", O_RDWR);

    }

    /* open room file */

    if ((roomfl = open("../ctdlroom.sys", O_RDWR)) == ERROR) {

        putString(" ctdlroom.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((roomfl = creat("../ctdlroom.sys", 0660)) == ERROR) {

            putString("?can't create ctdlroom.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(roomfl);     /* can't create in mode 2! */
        roomfl = open("../ctdlroom.sys", O_RDWR);

    }

    /* open userlog file */

    if ((logfl = open("../ctdllog.sys", O_RDWR)) == ERROR) {

        putString(" ctdllog.sys not found, creating new file. \n");
        putString(" (Be sure to initialize it!)\n");

        if ((logfl = creat("../ctdllog.sys", 0660)) == ERROR) {

            putString("?can't create ctdllog.sys!!\n");
            exit(EXIT_FAILURE);

        }

        close(logfl);    /* can't create in mode 2! */
        logfl = open("../ctdllog.sys", O_RDWR);

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

    putString("messages init\n");
    msgInit();
    putString("Press <return> to continue.\n");
    getCh();

    putString("rooms init\n");
    indexRooms();
    putString("Press <return> to continue.\n");
    getCh();

    putString("log init\n");
    logInit();
    putString("Press <return> to continue.\n");
    getCh();

    putString("\nCreating the sysop account\n");
    initSysop();
    putString("Press <return> to continue.\n");
    getCh();

}

/************************************************************************/
/*    main() for confg.c                                                */
/************************************************************************/

int main(int argc, char **argv) {

    char c;
    char opts[] = "ch?";
    extern char *optarg;
    extern int optind;
    char *configs = "../ctdlcnfg.sys";

    opterr = 0;

    while ((c = getopt(argc, argv, opts)) != -1) {

        switch(c) {
            case 'c':
                configs = argv[optind];
                break;
            case 'h':
                printf("\n");
                printf("Citadel 2.10 - Linux port\n");
                printf("\n");
                printf("Usage: 210confg [-h] [-c <filename>]\n");
                printf("\n");
                printf("    -h - display help.\n");
                printf("    -c <filename> - use this configuration file.\n");
                printf("\n");
                return EXIT_SUCCESS;
                break;
            case '?':
                printf("\n");
                printf("Usage: 210confg [-h] [-c <filename>]\n");
                printf("\n");
                return EXIT_SUCCESS;
                break;
        }

    }

    initTerminal();
    loadConfig(configs);
    init();
    wrapup();
    endTerminal();

    return(EXIT_SUCCESS);

}

