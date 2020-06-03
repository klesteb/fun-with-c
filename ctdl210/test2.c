
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

    for (i = 0; i < MAXLOGTAB; i++) {

        getLog(&logBuf, i);

        putString("nulls     : %d\n", logBuf.lbnulls);
        putString("flags     : %d\n", logBuf.lbflags);
        putString("  ucmask  : %d\n", logBuf.lbflags & UCMASK);
        putString("  lfmask  : %d\n", logBuf.lbflags & LFMASK);
        putString("  expert  : %d\n", logBuf.lbflags & EXPERT);
        putString("  tabmask : %d\n", logBuf.lbflags & TABMASK);
        putString("  aide    : %d\n", logBuf.lbflags & AIDE);
        putString("width     : %d\n", logBuf.lbwidth);
        putString("name      : %s\n", logBuf.lbname);
        putString("password  : %s\n", logBuf.lbpw);
        putString("generation: %s\n", logBuf.lbgen);
        putString("visits\n");
        
        for (x = 0; x < MAXVISIT; x++) {
            
            putString("%d  ", logBuf.lbvisit[x]);
            
        }
        putString("\n");
        
        putString("Private Maild\n");
        for (x = 0; x< MAILSLOTS; x++) {
            
            putString("%d  ", logBuf.lbslot[x]);
            
        }
        
        putString("\nPress <return> to continue\n");
        getch();
        
    }

    endTerminal();
    
    return(EXIT_SUCCESS);
    
}

