
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.c"

int main(int argc, char **argv) {

    initTerminal();

    expert = TRUE;
    tutorial("helpopt.hlp");
    getCh();

    endTerminal();

    return(EXIT_SUCCESS);

}

