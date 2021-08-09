
#ifndef TOOLS_H
#define TOOLS_H 1

#include <string.h>
#include <ctype.h>

#ifndef TRUE
#define TRUE  (1)
#define FALSE (0)
#endif

/*----------------------------------------------------------------------*/
/* Configuration file routines                                          */
/*----------------------------------------------------------------------*/

typedef struct cfg_strings {
    char *name;
    char *data;
} cfg_t;

int cfg_read(char * filename, cfg_t *cfg_info );

/*----------------------------------------------------------------------*/
/* Linked lists - stacks - queue routines                               */
/*----------------------------------------------------------------------*/

#include "cclibs/que_util.h"

/*----------------------------------------------------------------------*/
/* Pattern matching - Unix style regular expressions                    */
/*----------------------------------------------------------------------*/

#include "cclibs/misc/match.h"

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

/* int instr(int, const char *, const char *); */
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

int stricmp(const char *, const char *);
int strnicmp(const char *, const char *, int);

void insert(char *, char *, int, int);

/*----------------------------------------------------------------------*/
/* System routines                                                      */
/*----------------------------------------------------------------------*/

void syserr(char *, ...);
void *xmalloc(int);

#endif

