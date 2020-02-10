/* +++Date last modified: 05-Jul-1997 */

/*====================================================================

    _MSC_VER        Microsoft C 6.0 and later
    _QC             Microsoft Quick C 2.51 and later
    __TURBOC__      Borland Turbo C, Turbo C++ and BC++
    __BORLANDC__    Borland C++
    __ZTC__         Zortech C and C++
    __SC__          Symantec C++
    __WATCOMC__     WATCOM C
    __POWERC        Mix Power C
    __GNUC__        Gnu C
    __EMX__         Emx Gnu C

    Revised:

    Original   Scott Ladd   Now obsolete
    14-Sep-93  Fred Cole    Moved MK_FP() macro to end of file to avoid
                            redefinition error when dos.h gets included
                            at the in/outport definitions for __TURBOC__
    15-Sep-93  Thad Smith   Add conditional code for TC 2.01
                            Fix findfirst/findnext support for ZTC 3.0
    15-Oct-93  Bob Stout    Revise find first/next support
    02-Dec-93  David Nugent Additions for findfirst/findnext support for
                            MSC6 (& 7) for OS/2
                            Added FIND_END macro for use under OS/2 to
                            be nice about closing the directory handle
                            DOSFileData members should be accessed via
                            the new ff_*() macros for portability
                            Note: use -DOS2 when compiling under OS/2
    03-Apr-94  Bob Stout    Add Power C support, FAR
    19-Aug-95  Bob Stout    Add NEAR, PASCAL, CDECL, and portable attributes
    06-Sep-95  Phi Nguyen   Add DOSFileTime, DOSFileDate, & supporting macros
               Bob Stout    (ff_yr/mo/day(), ff_hr/min/tsec())
    08-Sep-95  Bob Nelson   Add __GNUCC__ and PAK macro
    21-Sep-95  Bob Stout    Renamed to PC-PORT.H, revised directory stuff
    25-Sep-95  Bob Stout    Split out EXTKWORD.H
    26-Sep-95  Jerry Coffin Added Win32 support.
    21-Oct-95  Bob Stout    Resolve struct dirent incompatibilities,
                            move port I/O macros to PCHWIO.H,
                            move MK_FP() cover to SNIPTYPE.H,
                            rename to DIRPORT.H(!!!)
    15-May-96  David Nugent Added 32-bit OS/2 support (Watcom/emx)
    04-Jul-96  Bob Stout    Fixed attribute redefinition problems w/ TC 3.x
    28-Jul-96  Bob Stout    Fixed error in definition of ff_sec() in DOS
    20-Aug-96  Bob Stout    Eliminate Win32 conflicts
======================================================================*/


/* prevent multiple inclusions of this header file */

#if !defined(DIRPORT__H)
#define DIRPORT__H

#if defined(__OS2__)
 #define OS2
#endif

#if defined(__EMX__)
 #undef OS2
 #define OS2
#endif

#if defined(OS2)
 #undef MSDOS
#endif

#include "extkword.h"         /* Extended keywords header   */

#undef dirent
#define dirent DIRENT_

/*
**  Correct file attributes
*/

#if defined(__POWERC) || (defined(__TURBOC__) && !defined(__BORLANDC__))
 #ifndef FA_NORMAL
  #define FA_NORMAL 0x00
 #endif

 #undef  _A_NORMAL
 #undef  _A_RDONLY
 #undef  _A_HIDDEN
 #undef  _A_SYSTEM
 #undef  _A_VOLID
 #undef  _A_SUBDIR
 #undef  _A_ARCH
 #undef  _A_ANY

 #define _A_NORMAL      FA_NORMAL
 #define _A_RDONLY      FA_RDONLY
 #define _A_HIDDEN      FA_HIDDEN
 #define _A_SYSTEM      FA_SYSTEM
 #define _A_VOLID       FA_LABEL
 #define _A_SUBDIR      FA_DIREC
 #define _A_ARCH        FA_ARCH
 #define _A_ANY   FA_NORMAL | FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_LABEL | \
                  FA_DIREC | FA_ARCH
#else
 #ifdef OS2
  #define _A_NORMAL     FILE_NORMAL
  #define _A_RDONLY     FILE_READONLY
  #define _A_HIDDEN     FILE_HIDDEN
  #define _A_SYSTEM     FILE_SYSTEM
  #define _A_SUBDIR     FILE_DIRECTORY
  #define _A_VOLID      0
  #define _A_ARCH       FILE_ARCHIVED
  #define _A_ANY  FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | \
                  FILE_DIRECTORY | FILE_ARCHIVED
 #elif defined(_WIN32) || defined(WIN32)
  #define WIN32_LEAN_AND_MEAN
  #define NOGDI
  #define NOSERVICE
  #undef INC_OLE1
  #undef INC_OLE2
  #include <windows.h>
  #include <dos.h>    /* #define's most _A_* file attribute macros    */

  #define _A_VOLID   0
  #define _A_ANY     _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | \
     _A_SUBDIR | _A_ARCH

  #define ff_name(x)     (x)->file.cFileName
  #define ff_size(x)     (x)->file.nFileSizeLow
  #define ff_attr(x)     (x)->file.dwFileAttributes

  typedef struct dirent {
     WIN32_FIND_DATA file;
     HANDLE handle;
  } DOSFileData;

  WORD ff_date(DOSFileData *f);
  WORD ff_yr(DOSFileData *f);
  WORD ff_mo(DOSFileData *f);
  WORD ff_day(DOSFileData *f);
  WORD ff_time(DOSFileData *f);
  WORD ff_hr(DOSFileData *f);
  WORD ff_min(DOSFileData *f);
  WORD ff_tsec(DOSFileData *f);

  int FIND_FIRST(char *spec, unsigned attr, DOSFileData *ff);
  int FIND_NEXT(DOSFileData *ff);
  int FIND_END(DOSFileData *ff);
 #elif defined(__MSDOS__) || defined(MSDOS)
  #define _A_ANY  _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_VOLID | \
                  _A_SUBDIR | _A_ARCH
 #endif
#endif

/*--------------------------------------------------------------------
    Directory search macros and data structures

    DOSFileData         MS-DOS file data structure
    DOSFileDate         MS-DOS file date structure
    DOSFileTime         MS-DOS file time structure
    FIND_FIRST          MS-DOS function 0x4E -- find first matching spec
    FIND_NEXT           MS-DOS function 0x4F -- find subsequent files
----------------------------------------------------------------------*/

/* make sure the structure is packed on byte boundary */

#if defined(_MSC_VER) || defined(_QC) || defined(__WATCOMC__)
    #pragma pack(1)
#elif defined(__ZTC__)
    #pragma ZTC align 1
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
    #pragma option -a-
#endif

#ifdef __GNUC__
 #define PAK      __attribute__((packed))
#else
 #define PAK
#endif

/*
**  Structures to access dates and times
*/

typedef struct {
            unsigned tsecs:   5;
            unsigned mins:    6;
            unsigned hours:   5;
      } DOSFileTime;

typedef struct {
            unsigned day:     5;
            unsigned month:   4;
            unsigned year:    7;
      } DOSFileDate;

/*
**  Use this structure in place of compiler-defined file structure.
**  Always use the ff_xxxx() macros to access structure members.
*/

#if defined(OS2)
 #define INCL_DOS
 #include "os2.h"
 #undef TRUE
 #undef FALSE
 #undef  _SYSINT
 #if defined(__FLAT__) || defined(__EMX__)
 #undef  _FILEFINDBUF
 #define _FILEFINDBUF _FILEFINDBUF3
 #define _SYSINT ULONG
 #define FINDLVL FIL_STANDARD
 #else
 #define _SYSINT USHORT
 #define FINDLVL 0L
 #endif

 typedef struct dirent {
      HDIR dh;
      struct _FILEFINDBUF f;
      } DOSFileData;

 #define d_name         achName     /* For struct dirent portability    */
 #define d_size         cbFile

 #define ff_name(x)     (x)->f.d_name
 #define ff_size(x)     (x)->f.d_size
 #define ff_attr(x)     (x)->f.attrFile
 #define ff_date(x)     *((DOSFileDate *)(&(x)->f.fdateLastWrite))
 #define ff_yr(x)       *((DOSFileDate *)(&(x)->f.fdateLastWrite)).year
 #define ff_mo(x)       *((DOSFileDate *)(&(x)->f.fdateLastWrite)).month
 #define ff_day(x)      *((DOSFileDate *)(&(x)->f.fdateLastWrite)).day
 #define ff_time(x)     *((DOSFileTime *)(&(x)->f.ftimeLastWrite))
 #define ff_hr(x)       *((DOSFileTime *)(&(x)->f.ftimeLastWrite)).hours
 #define ff_min(x)      *((DOSFileTime *)(&(x)->f.ftimeLastWrite)).mins
 #define ff_tsec(x)     *((DOSFileTime *)(&(x)->f.ftimeLastWrite)).tsecs
#elif !(defined(WIN32) || defined(_WIN32)) /* Not OS/2  or Win32 */

 typedef struct dirent {
      char              reserved[21];
      char              attrib;
      DOSFileTime       time;
      DOSFileDate       date;
      long              d_size;
      char              d_name[13];
      } DOSFileData;

 #define ff_name(x)     (x)->d_name
 #define ff_size(x)     (x)->d_size
 #define ff_attr(x)     (x)->attrib
 #define ff_date(x)     (x)->date
 #define ff_yr(x)       (x)->date.year
 #define ff_mo(x)       (x)->date.month
 #define ff_day(x)      (x)->date.day
 #define ff_time(x)     (x)->time
 #define ff_hr(x)       (x)->time.hours
 #define ff_min(x)      (x)->time.mins
 #define ff_sec(x)      (x)->time.tsecs
#endif

/* set structure alignment to default */

#if defined (_MSC_VER) || defined(_QC) || defined(__WATCOMC__)
 #pragma pack()
#elif defined (__ZTC__)
 #pragma ZTC align
#elif defined(__TURBOC__) && (__TURBOC__ > 0x202)
 #pragma option -a.
#endif

/* include proper header files and create macros */

#if defined (_MSC_VER) || defined(_QC) || defined(__WATCOMC__) || defined(__EMX__)
 #if defined( OS2 )
  int FIND_FIRST (char * spec, unsigned attr, DOSFileData *ff);
  int FIND_NEXT (DOSFileData *ff);
  int FIND_END (DOSFileData *ff);

 #elif !(defined(_WIN32) || defined(WIN32))
  #include "dos.h"
  #define FIND_FIRST(spec,attr,buf) _dos_findfirst(spec,attr, \
        (struct find_t *)buf)
  #define FIND_NEXT(buf) _dos_findnext((struct find_t *)buf)
  #define FIND_END(buf)
 #endif
#elif defined (__TURBOC__) || defined (__BORLANDC__) || defined (__POWERC)
 #include "dir.h"
 #include "dos.h"
 #define FIND_FIRST(spec,attr,buf) findfirst(spec,(struct ffblk *)buf,attr)
 #define FIND_NEXT(buf) findnext((struct ffblk *)buf)
 #define FIND_END(buf)
#elif defined(__SC__) || defined(__WATCOMC__) || defined(__ZTC__)
 #include "dos.h"
 #define FIND_FIRST(spec,attr,buf) _dos_findfirst(spec,attr, \
       (struct find_t *)buf)
 #define FIND_NEXT(buf) _dos_findnext((struct find_t *)buf)
 #define FIND_END(buf)
#endif

#endif /* DIRPORT__H */
