
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1999 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "report.h"
#include "tools.h"
#include "dates.h"

#define PAGE_WIDTH   80
#define PAGE_LENGTH  58
#define COMPANY_NAME "THE ACME GADGET CO."

/*-Local-Global-Variables---------------------------------------------------*/

/*-Local-Prototypes---------------------------------------------------------*/

static void NewPage(FILE *, ReportControl *);
static void SysDate(char *);

/*--------------------------------------------------------------------------*/

void report(FILE *fd, char *buffer, ReportControl *rc) {
/*
 * Function: report.c
 * Version : v1.0
 * Created : 16-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synopsis
 *
 *    This function will print a formated report that is descripted by the
 *    ReportControl structure. This is take off of report.bas.
 *
 *    The defaults are the following:
 *
 *        rc->PageLength = 58
 *        rc->PageWidth  = 80
 *        rc->CompanyName = "THE ACME GADGET CO."
 *
 * Modification History
 *
 * Variables Used
 */

    int x = 0;

/*
 * Main part of funtion.
 */

    if (rc->PageCount == 0) {

        if (rc->PageWidth == 0)  rc->PageWidth = PAGE_WIDTH;
        if (rc->PageLength == 0) rc->PageLength = PAGE_LENGTH;
        if (rc->CompanyName == NULL) rc->CompanyName = COMPANY_NAME;
        NewPage(fd, rc);

    }

    if (rc->LineCount > rc->PageLength) NewPage(fd, rc);

    fputs(buffer, fd);
    fputs("\n", fd);

    if (rc->LineCtl != 0) {

        for (x = 1; x < rc->LineCtl; x++)
            fputs("\n", fd);

        x -= x;

    }

    rc->LineCount += (1 + x);

}

static void NewPage(FILE *fd, ReportControl *rc) {
/*
 * Function: NewPage.c
 * Version : v1.0
 * Created : 16-Dec-1992
 * Author  : Kevin Esteb
 *
 * Synopsis
 *
 *    This routine will print the headings and legends for the new page.
 *
 * Modification History
 *
 * Variables Used
 */

    char header_line[134];
    char page_count[6];
    char page_count_fmt[6];
    static char run_date[12];

/*
 * Main part of funtion.
 */

    fputs("\n\f", fd);                          /* form feed to new page    */
    rc->PageCount++;                            /* Increase PageCount       */
    sprintf(page_count, "%5d", rc->PageCount); /* Format the page number   */
/*    fmtasc(page_count, "x,xxx", page_count_fmt); */
    memset(header_line, ' ', sizeof(header_line) - 1);
    header_line[133] = '\0';
    SysDate(run_date);

    insert(header_line, "RUN DATE:", 0, 9);
    insert(header_line, run_date, 10, strlen(run_date));

    if (rc->PageWidth == 132) {

        insert(header_line, rc->CompanyName, 58, strlen(rc->CompanyName));
        insert(header_line, "PAGE:", 120, 5);
        insert(header_line, page_count, 126, strlen(page_count));

    } else {

        insert(header_line, rc->CompanyName, 31, strlen(rc->CompanyName));
        insert(header_line, "PAGE:", 66, 5);
        insert(header_line, page_count, 73, strlen(page_count));

    }

    fputs(header_line, fd);                 /* Print the header line        */
    fputs("\n\n", fd);                      /* Print blank line             */
    rc->LineCount = 2;                      /* Set line counter             */

    if (rc->ReportTitle != NULL) {          /* Print the report title       */

        fputs(rc->ReportTitle, fd);
        fputs("\n", fd);
        rc->LineCount++;

    }

    if (rc->LegendCtl == TRUE) {            /* Check the legend control     */

        if (rc->Legend_1 != NULL) {         /* Print first legend           */

            fputs(rc->Legend_1, fd);
            fputs("\n", fd);
            rc->LineCount++;

        }

        if (rc->Legend_2 != NULL) {         /* Print second legend          */

            fputs(rc->Legend_2, fd);
            fputs("\n", fd);
            rc->LineCount++;

        }

        if (rc->Legend_3 != NULL) {         /* Print third legend           */

            fputs(rc->Legend_3, fd);
            fputs("\n", fd);
            rc->LineCount++;

        }

        if (rc->Legend_4 != NULL) {         /* Print fourth legend          */

            fputs(rc->Legend_4, fd);
            fputs("\n", fd);
            rc->LineCount++;

        }

        fputs("\n", fd);
        rc->LineCount++;

    }

    if (rc->Header_1 != NULL) {             /* Print first header           */

        fputs(rc->Header_1, fd);
        fputs("\n", fd);
        rc->LineCount++;

    }

    if (rc->Header_2 != NULL) {             /* Print second header          */

        fputs(rc->Header_2, fd);
        fputs("\n", fd);
        rc->LineCount++;

    }

    if (rc->Header_3 != NULL) {             /* Print third header           */

        fputs(rc->Header_3, fd);
        fputs("\n", fd);
        rc->LineCount++;

    }

    if (rc->Header_4 != NULL) {             /* Print fourth header          */

        fputs(rc->Header_4, fd);
        fputs("\n", fd);
        rc->LineCount++;

    }

}

static void SysDate(char *run_date) {

    DATE jdate;
    int day;
    int month;
    int year;
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
/*
 * Main part of function.
 */
 
    jdate = today();
    calendar_date(jdate, &day, &month, &year);

    sprintf(run_date, "%d-%s-%d", day, months[month - 1], year);
    
}

