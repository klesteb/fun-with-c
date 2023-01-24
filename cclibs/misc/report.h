
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1992 by Kevin L. Esteb                                     */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* TITLE:       report.h                                                     */
/*                                                                           */
/* FACILITY:    Global variables and structures for the REPORT package.      */
/*                                                                           */
/* ABSTRACT:    These are the declarations of the data structures and        */
/*              routines to implement the REPORT functions.                  */
/*                                                                           */
/* ENVIRONMENT: VMS                                                          */
/*                                                                           */
/* Version      Author                                           Date        */
/* -------      ---------------------------------------------    ----------- */
/* 1-000        Kevin Esteb                                      16-Dec-2002 */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef REPORTS_H
#define REPORTS_H 1

/*----------------------------------------------------------------------*/
/*    GSC Structures and Definitions.                                   */
/*----------------------------------------------------------------------*/

typedef struct {
    char *CompanyName;      /* Default company name                     */
    char *ReportTitle;      /* The reports title                        */
    char *Legend_1;         /* The first legend line                    */
    char *Legend_2;         /* The second legend line                   */
    char *Legend_3;         /* The third legend line                    */
    char *Legend_4;         /* The fourth legend line                   */
    char *Header_1;         /* The first column header                  */
    char *Header_2;         /* The second column header line            */
    char *Header_3;         /* The third column header line             */
    char *Header_4;         /* The fourthe column header line           */
    int  LineCount;         /* Printed line count                       */
    int  PageCount;         /* Printed page count                       */
    int  LineCtl;           /* Number of lines to skip                  */
    int  PageLength;        /* The page length                          */
    int  PageWidth;         /* The page width                           */
    int  LegendCtl;         /* Print legend with each new page          */
} ReportControl;

/*----------------------------------------------------------------------*/
/*    Miscellaneous declarations.                                       */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*    Public functions.                                                 */
/*----------------------------------------------------------------------*/

void report(FILE *, char *, ReportControl *);

#endif

