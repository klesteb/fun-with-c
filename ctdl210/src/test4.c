
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "ctdl.h"
#include "protos.h"
#include "common.c"

int main(int argc, char **argv) {

    int i;
    int x;
    struct logBuffer logBuf;

    initTerminal();
    loadConfig("../ctdlcnfg.sys");

    openFile("../ctdlmsg.sys",  &msgfl);
    openFile("../ctdlroom.sys", &roomfl);
    openFile("../ctdllog.sys",  &logfl);

    for (i = 0; i < MAILSLOTS; i++) {

        getLog(&logBuf, i);

        putString("name    : %s\n", logBuf.lbname);
        putString("password: %s\n", logBuf.lbpw);
        putString("nuls    : %d\n", logBuf.lbnulls);
        putString("flags\n");
        putString("  ucmask : %d\n", logBuf.lbflags & UCMASK);
        putString("  lfmask : %d\n", logBuf.lbflags & LFMASK);
        putString("  expert : %d\n", logBuf.lbflags & EXPERT);
        putString("  tabmask: %d\n", logBuf.lbflags & TABMASK);
        putString("  aide   : %d\n", logBuf.lbflags & AIDE);
        putString("width: %d\n", logBuf.lbwidth);
        putString("generations\n");

        for (x = 0; x < MAXROOMS; x++) {

            putString("%d, ", logBuf.lbgen[x]);

        }
        putString("\n");

        putString("vists\n");
        for (x = 0; x < MAXVISIT; x++) {

            putString("%d, ", logBuf.lbvisit[x]);

        }
        putString("\n");

        putString("slot\n");
        for (x = 0; x < MAILSLOTS; x++) {

            putString("%d, ", logBuf.lbslot[x]);

        }
        putString("\n");

        putString("id\n");
        for (x = 0; x < MAILSLOTS; x++) {

            putString("%d, ", logBuf.lbId[x]);

        }
        putString("\n");

        putString("\nPress <return> to continue\n");
        getCh();

    }

    endTerminal();

    return(EXIT_SUCCESS);

}

