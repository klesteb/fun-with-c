
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

#include "210ctdl.h"
#include "210protos.h"

/************************************************************************/
/*                   contents                                           */
/*                                                                      */
/*    configure()    sets terminal parameters via dialogue              */
/*    doCR()         newline on modem and console                       */
/*    patchDebug()   display/patch byte                                 */
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
/*    patchDebug()                                                      */
/************************************************************************/
void patchDebug(void) {
    
    unsigned  i, j, page;
    char finished, *pc;

    printf("\bpatch\n ");
    page = getNumber("page", 0, 255);
    finished = FALSE;

    do {

        for (i = 16, pc = (page * 256);  i; i--) {

            printf("%s ", pc);

            for (j = 16; j; j--) {
                
                printf("%c", visible(*pc++));
                
            }

            printf("\n ");
            
        }

        switch (tolower(iChar()))   {
            case 'r':
                pc  = getNumber("adr", 0, 65355);
                *pc = getNumber("val", 0, 255);
                break;
            case 'n':    
                page++;
                break;
            case 'p':    
                page--;         
                break;
            default:    
                finished = TRUE;    
                break;
        }
        
    } while (!finished);

}

/************************************************************************/
/*    printDate() prints out a date packed in two-byte format     */
/************************************************************************/
void printDate(char year, char month, char day) {

    printf("%d%s%02d ", year, monthTab[month], day);

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
    outFlag  = OUTOK;

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

