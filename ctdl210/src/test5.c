
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "ctdl.h"
#include "protos.h"
#include "common.c"

int main(int argc, char **argv) {

    int c;
    int sysop = FALSE;
    char opts[] = "acsh?";
    extern char *optarg;
    extern int optind;
    char *configs = "../ctdlcnfg.sys";

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
                printf("Citadel 2.10 - Linux port\n");
                printf("\n");
                printf("Usage: ctdl [-a] [-s] [-c <filename>]\n");
                printf("\n");
                printf("    -a - enable aide options.\n");
                printf("    -s - enable sysop options.\n");
                printf("    -c <filename> - use this configuration file.\n");
                printf("\n");
                return EXIT_SUCCESS;
                break;
            case '?':
                printf("Usage: ctdl [-a] [-s] [-c <filename>]\n");
                return EXIT_SUCCESS;
                break;
        }
        
    }

    loadConfig(configs);

    printf("nodeTitle: %s\n", nodeTitle);
    printf("nodeName : %s\n", nodeName);
    printf("nodeId   : %s\n", nodeId);
    printf("cryptseed: %d\n", cryptSeed);
    printf("mdata    : %d\n", mData);
    printf("megahz   : %d\n", megaHz);
    printf("messagek : %d\n", maxMSector);
    printf("rcpm     : %d\n", rcpm);
    printf("homedisk : %d\n", homeDisk);
    printf("homeuser : %d\n", homeUser);
    printf("msgdisk  : %d\n", msgDisk);
    printf("loginok  : %d\n", unlogLoginOk);
    printf("readok   : %d\n", unlogReadOk);
    printf("enterok  : %d\n", unlogEnterOk);
    printf("roomok   : %d\n", nonAideRoomOk);
    printf("msgPath  : %s\n", msgPath);
    printf("helpPath : %s\n", helpPath);
    printf("menuPath : %s\n", menuPath);

    return EXIT_SUCCESS;

}

