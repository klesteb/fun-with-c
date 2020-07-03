
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
/*    configure()       sets terminal parameters via dialogue           */
/*    doCR()            newline on modem and console                    */
/*    printDate()       prints out date                                 */
/*    tutorial()        prints a .hlp file                              */
/*    visible()         convert control chars to letters                */
/*    loadConfig()      load config file                                */
/*    crypte()          encrypts/decrypts data blocks                   */
/*    hash()            hashes a string to an integer                   */
/*    matchString()     search for given string                         */
/*    replaceString()   string-substitute for message entry             */
/*    normalizeString() strip crap from a string                        */
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

    FILE *fp = NULL;
    int  arg, args;
    char line[128];               
    char cmd[128], var[128], string[128];


    if ((fp = fopen(filename, "r")) == NULL) {

        putString("?Can't find %s\n", filename);
        exit(EXIT_FAILURE);

    }

    while ((fgets(line, 128, fp) != EOF)) {

        if ((args = sscanf(line, "%s %s %x ", cmd, var, &arg))) {
            
            if ((strcmp(cmd, "#define" ) == 0) && (args == 3)) {

                if (strcmp(var, "MDATA") == 0) {

                    mData = arg;

                } else if (strcmp(var, "MEGAHZ") == 0) {

                    megaHz = arg;

                } else if (strcmp(var, "CRYPTSEED") == 0) {

                    cryptSeed = arg;

                } else if (strcmp(var, "RCPM") == 0) {

                    rcpm = arg;

                } else if (strcmp(var, "CLOCK") == 0) {

                    /* clock = arg; */

                } else if (strcmp(var, "MESSAGEK") == 0) {

                    maxMSector = arg * 8;

                } else if (strcmp(var, "MSGDISK") == 0) {

                    msgDisk = arg;

                } else if (strcmp(var, "HOMEDISK") == 0) {

                    homeDisk = arg;

                } else if (strcmp(var, "HOMEUSER") == 0) {

                    homeUser = arg;

                } else if (strcmp(var, "LOGINOK") == 0) {

                    unlogLoginOk = arg;

                } else if (strcmp(var, "ENTEROK") == 0) {

                    unlogEnterOk = arg;

                } else if (strcmp(var, "READOK") == 0) {

                    unlogReadOk = arg;

                } else if (strcmp(var, "ROOMOK") == 0) {

                    nonAideRoomOk = arg;

                }

            } else if (strcmp(cmd, "#nodeTitle") == 0) {

                sscanf(line, "%s \"%[^\"]", cmd, string);
                strcpy(nodeTitle, string);

            } else if (strcmp(cmd, "#nodeName") == 0) {

                sscanf(line, "%s \"%[^\"]", cmd, string);
                strcpy(nodeName, string);

            } else if (strcmp(cmd, "#nodeId") == 0) {

                sscanf(line, "%s \"%[^\"]", cmd, string);
                strcpy(nodeId, string);

            } else if (strcmp(cmd, "#msgPath") == 0) {

                sscanf(line, "%s \"%[^\"]", cmd, string);
                strcpy(msgPath, string);

            } else if (strcmp(cmd, "#alldone") == 0) {

                break;

            }

        }

    }

    fclose(fp);

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

/************************************************************************/
/*    matchString() searches for match to given string.  Runs backward  */
/*    through buffer so we get most recent error first.                 */
/*    Returns loc of match, else ERROR                                  */
/************************************************************************/
char *matchString(char *buf, char *pattern, char *bufEnd) {

    char *loc, *pc1, *pc2;
    char foundIt;

    for (loc = bufEnd, foundIt = FALSE; !foundIt && --loc >= buf;) {

        for (pc1 = pattern, pc2 = loc, foundIt=TRUE;  *pc1 && foundIt;) {

            if (! (tolower(*pc1++) == tolower(*pc2++))) {

                foundIt = FALSE;

            }

        }

    }

    return (foundIt) ? loc : NULL;

}

/************************************************************************/
/*    normalizeString() deletes leading & trailing blanks etc.    */
/************************************************************************/
void normalizeString(char *s) {

    char *pc;

    pc = s;

    /* find end of string   */

    while (*pc) {

        if (*pc < ' ') *pc = ' ';   /* zap tabs etc... */
        pc++;

    }

    /* no trailing spaces: */

    while (pc > s && isspace(*(pc - 1))) pc--;

    *pc = '\0';

    /* no leading spaces: */

    while (*s == ' ') {

        for (pc = s; *pc; pc++) *pc = *(pc + 1);

    }

    /* no double blanks */

    for (; *s; s++) {

        if (*s == ' ' && *(s + 1) == ' ') {

            for (pc = s; *pc; pc++) *pc = *(pc + 1);

        }

    }

}

/************************************************************************/
/*    replaceString() corrects typos in message entry                   */
/************************************************************************/
void replaceString(char *buf, int lim) {

    char oldString[2 * SECTSIZE];
    char newString[2 * SECTSIZE];
    char *loc, *textEnd;
    char *pc;
    int  incr;

    for (textEnd = buf; *textEnd; textEnd++);    /* find terminal null    */

    getString("string", oldString, (2 * SECTSIZE));

    if ((loc = matchString(buf, oldString, textEnd)) == NULL) {

        putString("?not found.\n ");
        return;

    }

    getString("replacement", newString, (2*SECTSIZE));

    if ((strlen(newString) - strlen(oldString)) >= (&buf[lim] - textEnd)) {

        putString("?Overflow!\n ");
        return;

    }

    /* delete old string: */

    for (pc = loc, incr = strlen(oldString); *pc = *(pc + incr); pc++);

    textEnd -= incr;

    /* make room for new string: */

    for (pc = textEnd, incr = strlen(newString);  pc >= loc; pc--) {

        *(pc + incr) = *pc;

    }

    /* insert new string: */

    for (pc = newString; *pc; *loc++ = *pc++);

}

