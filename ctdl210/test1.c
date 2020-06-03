
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.c"

int main(int argc, char **argv) {

    int i;
    int x;

    initTerminal();
    loadConfig("ctdlcnfg.sys");

    openFile("ctdlmsg.sys",  &msgfl);
    openFile("ctdlroom.sys", &roomfl);
    openFile("ctdllog.sys",  &logfl);

    for (i = 0; i < MAXROOMS; i++) {

        getRoom(i, &roomBuf);

        putString("generation: %d\n", roomBuf.rbgen);
        putString("flags     : %d\n", roomBuf.rbflags);
        putString(" inuse    : %d\n", roomBuf.rbflags & INUSE);
        putString(" public   : %d\n", roomBuf.rbflags & PUBLIC);
        putString(" cpmdir   : %d\n", roomBuf.rbflags & CPMDIR);
        putString(" permroom : %d\n", roomBuf.rbflags & PERMROOM);
        putString("name      : %s\n", roomBuf.rbname);
        putString("disk      : %d\n", roomBuf.rbdisk);
        putString("user      : %d\n", roomBuf.rbuser);
        putString("messages\n");

        putString("\nPress <return> to continue\n");
        getch();
        
        for (x = 0; x < MSGSPERRM; x++) {

            putString("    message number: %d\n", roomBuf.vp.msg[x].rbmsgNo);
            putString("    message sector: %d\n", roomBuf.vp.msg[x].rbmsgLoc);

        }

        putString("\nPress <return> to continue\n");
        getch();
        
    }

    endTerminal();
    
    return(EXIT_SUCCESS);
    
}

