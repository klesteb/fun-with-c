
#ifndef MISC_H
#define MISC_H 1

#include <string.h>
#include <ctype.h>

#include "que_util.h"

/*----------------------------------------------------------------------*/
/* DEC Basic - string handling routines                                 */
/*----------------------------------------------------------------------*/

char *spaces(int);
char *trim(char *);
char *rtrim(char *);
char *ltrim(char *);
char *string(int, int);
char *left(const char *, int);
char *right(const char *, int);
char *mid(const char *, int, int);
char *seg(const char *, int, int);

int pos(const char *, const char *, int);

/*----------------------------------------------------------------------*/
/* Misc string routines                                                 */
/*----------------------------------------------------------------------*/

char *strlwr(char *);
char *strupr(char *);
char *strrev(char *);
char *stripcr(char *);
char *soundex(char *, char *);
char *fmtasc(char *, char *);
char *xstrtok(char *, char *);
char *ltostr(long, char *, size_t, unsigned);

int stricmp(const char *, const char *);
int wordwrap(const char *, int, queue *);
int strnicmp(const char *, const char *, int);

void insert(char *, char *, int, int);

/*----------------------------------------------------------------------*/
/* System routines                                                      */
/*----------------------------------------------------------------------*/

int xgetline(int, char *, int, int);

void *xmalloc(int);
void syserr(char *, ...);

/*----------------------------------------------------------------------*/
/* crc routines                                                         */
/*----------------------------------------------------------------------*/

unsigned long crc32(char *, int);

unsigned short crc16(char *, int);
unsigned short crc_ccitt(char *, int);

/*----------------------------------------------------------------------*/
/* hash routines                                                        */
/*----------------------------------------------------------------------*/

long pal_hash(const unsigned char *);

unsigned long elf_hash(const unsigned char *);

#endif


