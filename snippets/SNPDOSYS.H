/* +++Date last modified: 05-Jul-1997 */

/*
**  SNPDOSYS.H - Snippets header file for DOS system-level functions
*/

#ifndef SNPDOSYS__H
#define SNPDOSYS__H

#include <stdio.h>
#include "sniptype.h"
#include "extkword.h"


/*
**  Prototypes
*/

int       isBreakOn(void);                            /* Break.C        */
void      setBreak(int OnOff);                        /* Break.C        */
int       drop_time(void);                            /* Droptime.C     */
unsigned  findIslot(void);                            /* Fndislot.C     */
int       format(char drive, char *switches,
            char *vlabel);                            /* Dosfrmat.C     */
int       is_share(char *arg);                        /* Isshare.C      */
long      memavail(void);                             /* Memavail.C     */
int       getNwLoginName (char * namebuf);            /* Nwlinnam.C     */
int       shell_to_DOS(void);                         /* Shel2Dos.C     */
void FAR *addptr (char FAR *p, unsigned long num);    /* Fptr_Add.C     */
void FAR *farnormal(void FAR *ptr);                   /* Fptr_Add.C     */


/*
**  File: JOYSTICK.C
*/

struct joystick {
        Boolean_T switch_0;
        Boolean_T switch_1;
        Boolean_T switch_2;
        Boolean_T switch_3;

        int     pos_Ax;
        int     pos_Ay;
        int     pos_Bx;
        int     pos_By;
};

extern struct joystick JoyStick;

Boolean_T read_joystick(void);


/*
**  File: OS_ID.C
*/

struct i_os_ver
{
  int maj;
  int min;
};

#define DOS     0
#define OS2     1
#define DV      2
#define WINS    3
#define WIN3    4
#define TOT_OS  5

                        /*   76543210  */
#define is_DOS  0x01    /* b'00000001' */
#define is_OS2  0x02    /* b'00000010' */
#define is_DV   0x04    /* b'00000100' */
#define is_WINS 0x08    /* b'00001000' */
#define is_WIN3 0x10    /* b'00010000' */

#ifndef OS_ID_MAIN
  extern int id_os_type;
  extern int id_os;
  extern const char *id_os_name[TOT_OS];
  extern struct i_os_ver id_os_ver[TOT_OS];
#endif

int  get_os(void);                  /* Determine OS                     */
void t_slice(int t_os);             /* Give up a time slice to the OS   */


/*
**  File: PFOPEN.C
*/

#ifdef unix
 #define SEP_CHARS ":"
#else
 #define SEP_CHARS ";"
#endif

FILE *pfopen(const char *name, const char *mode, const char *dirs);


/*
**  File: REDIRECT.C
*/

typedef  struct {
      char  path[FILENAME_MAX];
      int   which;
      int   what;
      int   oldhandle;
      int   flag;
} REDIR_STRUCT;

extern REDIR_STRUCT redir_stdin, redir_stdout, redir_stderr;

void  start_redirect    ( REDIR_STRUCT *s );
void  stop_redirect     ( REDIR_STRUCT *s );


#endif /* SNPDOSYS__H */
