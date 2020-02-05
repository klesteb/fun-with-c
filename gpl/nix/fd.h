#ifndef fd_DEFINED
#define fd_DEFINED

#ifndef FD_SET                  /* Desired macros defined in TYPES.H? */


/*******************************************************************************
    This INCLUDE file defines macro's for manipulating file descriptor bit
    masks (see SELECT(2)), if they are not already defined in the system
    INCLUDE file, TYPES.H.  The definitions were initially taken from the
    SunOS 4.0 "/usr/include/sys/types.h", in which was found the following
    copyright notice:

	Copyright (c) 1982, 1986 Regents of the University of California.
	All rights reserved.  The Berkeley software License Agreement
	specifies the terms and conditions for redistribution.

*******************************************************************************/

#ifdef VXWORKS
#    include <types.h>
#    define  memset(s,c,n)  bfill(s,n,c)
#else
#    include  <sys/types.h>             /* System type definitions. */
#endif

#ifndef NBBY
#   define      NBBY    8               /* number of bits in a byte */
#endif

/*
 * Select uses bit masks of file descriptors in longs.
 * These macros manipulate such bit fields (the filesystem macros use chars).
 * FD_SETSIZE may be defined by the user, but the default here
 * should be >= NOFILE (param.h).
 */

/*******************************************************************************
    The constant FD_SETSIZE controls the size of the "fd_set" structure
    defined further down.  The commented-out definition of "fd_set" is from
    the SunOS 4.0 "types.h" include file; it conflicts with the SunOS 3.4
    "types.h" definition of "fd_set" (whose member is a single integer).
    On SunOS 3.4, we'll set the FD_SETSIZE to 32 to be consistent with the
    "types.h" definition of "fd_set".  Broader experience with UNIX on some
    other machines might lead to a better, all-purpose "fd.h" file.
*******************************************************************************/

/* Under VMS, the documentation for SELECT() (part of UCX) suggests a limit
   of 32 on the size of the "fd_set" bit mask, but the actual implementation
   works with "fd_set"s of arbitrary size.  A width of 128 bits is chosen
   here; if I remember correctly, SunOS 4.0 used a value of 256.  Because VMS
   SELECT()'s method of scanning bit masks is inefficient, it is recommended
   that applications calling SELECT() specify the highest socket number (plus
   one) instead of FD_SETSIZE as the width of the "fd_set" bit mask. */

#ifdef vms
#    define  FD_SETSIZE  128
#endif

#ifndef FD_SETSIZE
#   define  FD_SETSIZE  32                      /* Default size. */
#endif

typedef long    fd_mask;

#define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */

#ifndef howmany
#   ifdef sun386
#       define howmany(x, y)   ((((u_int)(x))+(((u_int)(y))-1))/((u_int)(y)))
#   else
#       define  howmany(x, y)   (((x)+((y)-1))/(y))
#   endif
#endif


/*******************************************************************************
****  SunOS 4.0 Definition:  ***************************************************
****  Defined under SunOS 3.4 as a structure with member "INT FDS_BITS[1]"  ****
*******************************************************************************/

#ifdef vms
    typedef  struct fd_set {
	fd_mask  fds_bits[howmany(FD_SETSIZE, NFDBITS)] ;
    } fd_set ;
#endif


#ifdef vaxc             /* Take advantage of shifting instead of division. */

#    define     FD_SET(n, p)    \
	((p)->fds_bits[(n) >> 5]  |=  (1 << ((n) & (32-1))))
#    define     FD_CLR(n, p)    \
	((p)->fds_bits[(n) >> 5]  &=  ~(1 << ((n) & (32-1))))
#    define     FD_ISSET(n, p)  \
	((p)->fds_bits[(n) >> 5]  &  (1 << ((n) & (32-1))))
#    pragma builtins
#    define     FD_ZERO(p)      \
	_MOVC5 (0, (char *) NULL, '\0', sizeof (*(p)), (char *) (p))

#else

#    define     FD_SET(n, p)    \
	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#    define     FD_CLR(n, p)    \
	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#    define     FD_ISSET(n, p)  \
	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#    define     FD_ZERO(p)      memset ((char *)(p), '\0', sizeof(*(p)))

#endif


#endif /* FD_SET */


#endif                          /* If this file was not INCLUDE'd previously. */
