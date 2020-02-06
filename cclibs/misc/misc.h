
#ifndef MISC_H
#define MISC_H 1

#include <string.h>
#include <ctype.h>
#include "que_util.h"

/*----------------------------------------------------------------------*/
/* DEC Basic - string handling routines                                 */
/*----------------------------------------------------------------------*/

char *rtrim(char *);
char *ltrim(char *);
char *trim(char *);
char *mid(const char *, int, int);
char *seg(const char *, int, int);
char *left(const char *, int);
char *right(const char *, int);
char *spaces(int);
char *string(int, int);

int pos(const char *, const char *, int);

/*----------------------------------------------------------------------*/
/* Misc string routines                                                 */
/*----------------------------------------------------------------------*/

char *soundex(char *, char *);
char *fmtasc(char *, char *);
char *xstrtok(char *, char *);
char *strlwr(char *);
char *strupr(char *);
char *strrev(char *);

int wordwrap(const char *, int, queue *);
int stricmp(const char *, const char *);
int strnicmp(const char *, const char *, int);

void insert(char *, char *, int, int);

/*----------------------------------------------------------------------*/
/* System routines                                                      */
/*----------------------------------------------------------------------*/

void syserr(char *, ...);
void *xmalloc(int);

#endif

