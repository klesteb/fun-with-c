
/************************************************************************/
/*                misc.c                                                */
/*                                                                      */
/*    Random functions...                                               */
/************************************************************************/

/************************************************************************/
/*                history                                               */
/*                                                                      */
/* 83Mar12 CrT    from msg.c                                            */
/* 83Mar03 CrT & SB   Various bug fixes...                              */
/* 83Feb27 CrT    Save private mail for sender as well as recipient.    */
/* 83Feb23        Various.  transmitFile() won't drop first char on WC... */
/* 82Dec06 CrT    2.00 release.                                         */
/* 82Nov05 CrT    Stream retrieval.  Handles messages longer than MAXTEXT.*/
/* 82Nov04 CrT    Revised disk format implemented.                      */
/* 82Nov03 CrT    Individual history begun.  General cleanup.           */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "210ctdl.h"
#include "210protos.h"
#include "210common.h"

/************************************************************************/
/*                   contents                                           */
/*                                                                      */
/*    configure()    sets terminal parameters via dialogue              */
/*    doCR()         newline on modem and console                       */
/*    printDate()    prints out date                                    */
/*    tutorial()     prints a .hlp file                                 */
/*    visible()      convert control chars to letters                   */
/*    loadConfig()   load config file                                   */
/*    crypte()      encrypts/decrypts data blocks                       */
/*    hash()        hashes a string to an integer                       */
/************************************************************************/

/************************************************************************/
/*    configure() sets up terminal width etc via dialogue               */
/************************************************************************/
void configure(void) {

    termWidth = getNumber(" Screen width", 10, 255);
    termNulls = getNumber(" #Nulls", 0, 255);
    termUpper = getYesNo(" Uppercase only") ? UCMASK : 0;
    termLF    = getYesNo(" Linefeeds") ? LFMASK : 0;
    /* termTab    = getYesNo(" Tabs") ? TABMASK : 0; */
    expert    = !getYesNo(" Helpful hints") ? EXPERT : 0;

}

/************************************************************************/
/*    doCR() does a newline on modem and console                        */
/************************************************************************/
void doCR(void) {

    putString("\n");

}

/************************************************************************/
/*    printDate() prints out a date packed in two-byte format     */
/************************************************************************/
void printDate(int year, int month, int day) {

    putString("%d%s%02d ", year, monthTab[month], day);

}

/************************************************************************/
/*    getDate() retrieves the current date                              */
/************************************************************************/
void getDate(int *year, int *month, int *day) {

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    *year = tm->tm_year + 1900;
    *month = tm->tm_mon + 1;
    *day = tm->tm_mday;

}

/************************************************************************/
/*    tutorial() prints file <filename> on the modem & console          */
/*    Returns:    TRUE on success else ERROR                            */
/************************************************************************/
#define MAXWORD 256
int tutorial(char *filename) {

    FILE *fp = NULL;
    char line[MAXWORD];
    int toReturn;

    toReturn = TRUE;

    if ((fp = fopen(filename, "r")) == NULL) {

        putString("\n No %s.\n", filename);
        toReturn = ERROR;

    } else {

        if (!expert) {

            putString("\n <J>ump <P>ause <S>top\n");

        }

        putString(" \n");

        while ((fgets(line, MAXWORD, fp)) != NULL) {

            putString("%s", line);

        }

        fclose(fp);

    }

    return toReturn;

}

/************************************************************************/
/*    visible() converts given char to printable form if nonprinting    */
/************************************************************************/
char visible(char c) {

    if (c == 0xFF) c = '$';       /* start-of-message in message.buf    */
    c            = c & 0x7F;      /* kill high bit otherwise            */
    if (c < ' ') c = c + 'A' - 1; /* make all control chars letters     */
    if (c == 0x7F) c = '~';       /* catch DELETE too                   */

    return(c);

}

/************************************************************************/
/*    loadConfig()  load config file                                    */
/************************************************************************/
void loadConfig(char *filename) {

    int fd;
    char line[128];               
    char cmd[128], var[128], string[128];
    int  arg, args;


    if ((fd = open(filename, O_RDONLY)) == ERROR) {

        putString("?Can't find %s\n", filename);
        exit(EXIT_FAILURE);

    }

    while ((read(fd, line, 128) > 0)) {

        if ((args = sscanf(line, "%s %s %x ", cmd, var, &arg))) {

            if (strcmp(cmd, "#define" ) == SAMESTRING && args == 3) {

                putString("#define '%s' as %x\n", var, arg);

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

                    putString("? -- no variable '%s' known! -- ignored.\n", var);

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

            } else if (cmd[0] == '#') putString("? -- no '%s' command!\n", cmd);

        }

    }

}

/************************************************************************/
/*    crypte() encrypts/decrypts data blocks                            */
/*                                                                      */
/*  This was at first using a full multiply/add pseudo-random sequence  */
/*  generator, but 8080s don't like to multiply.  Slowed down I/O       */
/*  noticably. Rewrote for speed.                                       */
/************************************************************************/

#define b    fpc1
#define c    fi1
#define s    fi2

void crypte(void *buf, unsigned len, unsigned seed) {

    seed = (seed + cryptSeed) & 0xFF;
    b = buf;
    c = len;
    s = seed;

    for (; c; c--) {

        *b++ ^= s;
        s = (s + CRYPTADD) & 0xFF;

    }

}

/************************************************************************/
/*    hash() hashes a string to an integer                */
/************************************************************************/
int hash(char *str) {

    int  h, i, shift;

    for (h = shift = 0; *str; shift = (shift + 1) & 7, str++) {

        h ^= (i = toupper(*str)) << shift;

    }

    return h;

}

