
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

#include "210ctdl.h"
#include "210protos.h"

/************************************************************************/
/*                   contents                                           */
/*                                                                      */
/*    configure()    sets terminal parameters via dialogue              */
/*    doCR()         newline on modem and console                       */
/*    printDate()    prints out date                                    */
/*    tutorial()     prints a .hlp file                                 */
/*    visible()      convert control chars to letters                   */
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
    expert     =!getYesNo(" Helpful hints") ? EXPERT : 0;

}

/************************************************************************/
/*    doCR() does a newline on modem and console                        */
/************************************************************************/
void doCR(void) {

    printf("\n");

}

/************************************************************************/
/*    printDate() prints out a date packed in two-byte format     */
/************************************************************************/
void printDate(int year, int month, int day) {

    printf("%d%s%02d ", year, monthTab[month], day);

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
    int  toReturn;

    toReturn = TRUE;

    if ((fp = fopen(filename, "r+")) == NULL) {

        printf("\n No %s.\n", filename);
        toReturn = ERROR;
        
    } else {

        if (!expert) {
        
            printf("\n <J>ump <P>ause <S>top\n");
        
        }

        printf(" \n");

        while (fgets(line, MAXWORD, fp)) {
        
            printf("%s", line);
        
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

