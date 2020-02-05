/************************************************************************
 *  Copyright (c) 1996 by Charles A. Measday                            *
 *                                                                      *
 *  Permission to use, copy, modify, and distribute this software       *
 *  and its documentation for any purpose and without fee is hereby     *
 *  granted, provided that the above copyright notice appear in all     *
 *  copies.  The author makes no representations about the suitability  *
 *  of this software for any purpose.  It is provided "as is" without   *
 *  express or implied warranty.                                        *
 ************************************************************************/

/*
@(#)  FILE: meo_util.c  RELEASE: 1.7  DATE: 11/19/98, 15:37:03
*/
/*******************************************************************************

File:

    meo_util.c

    Memory Operations Package.


Author:    Alex Measday, ISI


Purpose:

    The MEO_UTIL functions perform various operations on memory regions.

    The meoDump() functions generate VMS-style dumps of arbitrary regions
    of memory.  Each line of output includes the address of the memory being
    dumped, the data (formatted in octal, decimal, or hexadecimal), and the
    ASCII equivalent of the data:

        ...
        00000060:  60616263 64656667 68696A6B 6C6D6E6F  "`abcdefghijklmno"
        00000070:  70717273 74757677 78797A7B 7C7D7E7F  "pqrstuvwxyz{|}~."
        00000080:  80818283 84858687 88898A8B 8C8D8E8F  "................"
        ...

    The MEO_UTIL package also provides a simple means of saving the contents
    of an arbitrary memory region to a file:

        #include  <stdio.h>             -- Standard I/O definitions.
        #include  "meo_util.h"          -- Memory operations.
        char  oldBuffer[1234] ;
        ...
        meoSave (oldBuffer, sizeof oldBuffer, "buffer.dat", 0) ;

    The contents of a file can be loaded into an existing region of memory:

        char  *newBuffer = oldBuffer ;
        int  numBytes = sizeof oldBuffer ;
        ...
        meoLoad ("buffer.dat", 0, &newBuffer, &numBytes) ;

    or into memory dynamically allocated by meoLoad():

        char  *newBuffer = NULL ;
        int  numBytes ;
        ...
        meoLoad ("buffer.dat", 0, &newBuffer, &numBytes) ;
        ... use the new buffer ...
        free (newBuffer) ;


Public Procedures (* defined as macros):

    meoDump() - outputs an ASCII dump of a memory region to a file.
  * meoDumpD() - outputs a decimal ASCII dump to a file.
  * meoDumpO() - outputs an octal ASCII dump to a file.
  * meoDumpT() - outputs a text ASCII dump to a file.
  * meoDumpX() - outputs a hexadecimal ASCII dump to a file.
    meoLoad() - loads the contents of a file into memory.
    meoSave() - saves the contents of memory to a file.

*******************************************************************************/


#include  <ctype.h>                     /* Standard character functions. */
#include  <errno.h>                     /* System error definitions. */
#include  <limits.h>                    /* Maximum/minimum value definitions. */
#ifndef PATH_MAX
#    include  <sys/param.h>             /* System parameters. */
#    define  PATH_MAX  MAXPATHLEN
#endif
#include  <stdio.h>                     /* Standard I/O definitions. */
#include  <stdlib.h>                    /* Standard C library definitions. */
#include  <string.h>                    /* Standard C string functions. */
#if defined(VMS)
#    include  <file.h>                  /* File definitions. */
#    include  <unixio.h>                /* VMS-emulation of UNIX I/O. */
#elif defined(VXWORKS)
#    include  <ioLib.h>                 /* I/O library definitions. */
#elif defined(WIN32)
#    include  <io.h>                    /* Low-level I/O definitions. */
#else
#    include  <fcntl.h>                 /* File control definitions. */
#    include  <unistd.h>                /* UNIX I/O definitions. */
#endif
#include  <sys/stat.h>                  /* File status definitions. */
#ifndef isascii
#    define  isascii(c)  ((unsigned char) (c) <= 0177)
#endif
#include  "fnm_util.h"                  /* Filename utilities. */
#include  "vperror.h"                   /* VPERROR() definitions. */
#include  "meo_util.h"                  /* Memory operations. */


int  meo_util_debug = 0 ;               /* Global debug switch (1/0 = yes/no). */

/*******************************************************************************

Procedure:

    meo_dump ()

    Generate an ASCII Dump of a Memory Region.


Purpose:

    Function meo_dump() formats the binary contents of a memory region in ASCII
    and writes the ASCII dump to a file.  Each output line looks as follows:

        <address>:  <data1> <data2> ... <dataN>  "data/ASCII"

    The data items, DATA1, DATA2, etc., are formatted in decimal, hexadecimal,
    or octal notation.


    Invocation:

        status = meo_dump(file, indentation, base, numBytesPerLine,
                          address, buffer, numBytesToDump) ;

    where

        <file>                  - I
            is the Unix FILE* handle for the output file.  If FILE is NULL,
            the dump is written to standard output.
        <indentation>           - I
            is a text string used to indent each line of the dump.  The
            string is used as the format string in an FPRINTF(3) statement,
            so you can embed anything you want.  This argument can be NULL.
        <base>                  - I
            specifies the output base for the dump: "MeoOctal" for octal,
            "MeoDecimal" for decimal, "MeoHexadecimal" for hexadecimal,
            and "MeoText" for text.  (These are enumerated values defined
            in the "meo_util.h" header file.)
        <numBytesPerLine>       - I
            specifies the number of bytes in the buffer to be formatted and
            dumped on a single line of output.  Good values are 8 values per
            line for octal, 8 for decimal, 16 for hexadecimal, and 40 for text.
        <address>               - I
            is the value to be displayed in the address field.  It can be
            different than the actual buffer address.
        <buffer>                - I
            is the buffer of data to be dumped.
        <numBytesToDump>        - I
            is exactly what it says it is!
        <status>                - O
            returns the status of generating the dump, zero if there were
            no errors and ERRNO otherwise.

*******************************************************************************/


int  meo_dump (

#    if __STDC__
        FILE  *file,
        const  char  *indentation,
        MeoBase  base,
        int  numBytesPerLine,
        void  *address,
        const  char  *buffer,
        int  numBytesToDump)
#    else
        file, indentation, base, numBytesPerLine,
        address, buffer, numBytesToDump)

        FILE  *file ;
        const  char  *indentation ;
        MeoBase  base ;
        int  numBytesPerLine ;
        void  *address ;
        const  char  *buffer ;
        int  numBytesToDump ;
#    endif

{    /* Local variables. */
    char  c, *out, *outbuf ;
    int  alignment, integer, numBytesDumped, numBytesThisLine ;
    unsigned  char  *buf, *inbuf ;





    if (file == NULL)  file = stdout ;


/* Allocate an input buffer to hold the bytes to be dumped.  Doing this
   prevents bus errors should the input buffer be misaligned (i.e., on
   an odd-byte boundary). */

    inbuf = (unsigned char *)malloc(numBytesPerLine) ;
    if (inbuf == NULL) {
        vperror("(meo_dump) Error allocating temporary, %d-byte input buffer.\nmalloc: ",
                 numBytesPerLine) ;
        return(errno) ;
    }


/* Allocate a string in which to build each output line. */

    outbuf = (char *)malloc(numBytesPerLine * 6) ;
    if (outbuf == NULL) {
        vperror("(meo_dump) Error allocating temporary, %d-byte output buffer.\nmalloc: ",
                 numBytesPerLine * 6) ;
        return(errno) ;
    }


/*******************************************************************************
    Generate each line of the dump.
*******************************************************************************/

    while (numBytesToDump > 0) {


        numBytesThisLine = (numBytesToDump > numBytesPerLine)
                           ? numBytesPerLine : numBytesToDump ;

        memset(inbuf, '\0', numBytesPerLine) ;         /* Zero trailing bytes. */
        memcpy(inbuf, buffer, numBytesThisLine) ;


/* Output the line indentation and the memory address. */

        if (indentation != NULL)  fprintf (file, indentation) ;
#ifdef YOU_WANT_VARIABLE_LENGTH_ADDRESS
        fprintf(file, "%p:\t", address) ;
#else
        fprintf(file, "%08lX: ", (long) address) ;     /* Assumes 32-bit address. */
#endif


/* Format the data in the requested base. */

        buf = inbuf ;  numBytesDumped = 0 ;
        out = outbuf ;


        switch (base) {

/* Base 8 - display the contents of each byte as an octal number. */

        case MeoOctal:
            while (numBytesDumped < numBytesPerLine) {
                if (numBytesDumped++ < numBytesThisLine)
                    sprintf (out, " %3.3o", *buf++) ;
                else
                    sprintf (out, " %3s", " ") ;
                out = out + strlen (out) ;
            }
            break ;

/* Base 10 - display the contents of each byte as a decimal number. */

        case MeoDecimal:
            while (numBytesDumped < numBytesPerLine) {
                if (numBytesDumped++ < numBytesThisLine)
                    sprintf (out, " %3u", *buf++) ;
                else
                    sprintf (out, " %3s", " ") ;
                out = out + strlen (out) ;
            }
            break ;

/* Base 16 - display the contents of each integer as a hexadecimal number. */

        case MeoHexadecimal:
            while (numBytesDumped < numBytesPerLine) {
                if (numBytesDumped < numBytesThisLine) {
                    alignment = (long) buf % sizeof (int) ;
                    if (alignment > 0) {
                        for (integer = 0 ;  alignment > 0 ;  alignment--)
                            integer = (integer << 8) | (*buf++ & 0x0FF) ;
                    } else {
                        integer = *((int *) buf) ;
                        buf = buf + sizeof (int) ;
                    }
                    sprintf (out, " %08X", integer) ;
                }
                else {
                    sprintf (out, " %8s", " ") ;
                }
                out = out + strlen (out) ;
                numBytesDumped = numBytesDumped + sizeof (int) ;
            }
            break ;

/* "Base 26" - treat the data as ASCII text. */

        case MeoText:
        default:
            break ;

        }


/* Append the ASCII version of the buffer. */

        if (base != MeoText) {
            *out++ = ' ' ;  *out++ = ' ' ;
        }
        *out++ = '"' ;
        memcpy(out, buffer, numBytesThisLine) ;
        numBytesDumped = 0 ;
        while (numBytesDumped++ < numBytesThisLine) {
            if (isascii(*out) && isprint(*out)) {
                c = *out ;  *out++ = c ;
            } else {
                *out++ = '.' ;
            }
        }
        *out++ = '"' ;  *out++ = '\0' ;


/* Output the dump line to the file. */

        fprintf(file, "%s\n", outbuf) ;


/* Adjust the pointers and counters for the next line. */

        address = (char *) address + numBytesThisLine ;
        buffer = (char *) buffer + numBytesThisLine ;
        numBytesToDump -= numBytesThisLine ;

    }


/* Deallocate the temporary input/output buffers. */

    free(inbuf) ;
    free(outbuf) ;


    return(0) ;

}

/*******************************************************************************

Procedure:

    meo_load ()

    Load Memory from a File.


Purpose:

    Function meo_load() loads the binary contents of a memory region from a
    disk file.  The contents must have been previously saved using meo_save().


    Invocation (dynamically allocated buffer):

        void  *startAddress = NULL ;
        ...
        status = meo_load(fileName, offset, &startAddress, &numBytes) ;

    Invocation (caller-specified buffer):

        void  *startAddress = buffer ;
        long  numBytes = sizeof buffer ;
        ...
        status = meo_load(fileName, offset, &startAddress, &numBytes) ;

    where:

        <fileName>      - I
            is the name of the file from which the memory contents will be
            loaded.  Environment variables may be embedded in the file name.
        <offset>        - I
            is the byte offset within the file from which the load will begin.
        <startAddress>  - I/O
            is the address of a (VOID *) pointer that specifies or returns the
            address where the contents of the file will be stored.  If the
            (VOID *) pointer is NULL, meo_load() will MALLOC(3) a buffer for
            the file contents and return its address through this argument;
            the caller is responsible for FREE(3)ing the memory when it is
            no longer needed.  If the (VOID *) pointer is NOT NULL, meo_load()
            uses it as the address of a caller-allocated buffer in which the
            file contents are to be stored; the size of the buffer is
            specified by the NUMBYTES argument.
        <numBytes>      - I/O
            is the address of a longword that specifies or returns the size of
            the memory buffer.  If the memory buffer is dynamically  allocated
            by meo_load(), this argument returns the size of the buffer.  If
            meo_load() uses a caller-allocated buffer, this argument specifies
            the size of the buffer.
        <status>        - O
            returns the status of loading the memory from a file, zero
            if there were no errors and ERRNO otherwise.

*******************************************************************************/


int  meo_load (

#    if __STDC__
        const  char  *fileName,
        long  offset,
        void  **startAddress,
        long  *numBytes)
#    else
        fileName, offset, startAddress, numBytes)

        char  *fileName ;
        long  offset ;
        void  **startAddress ;
        long  *numBytes ;
#    endif

{    /* Local variables. */
    FILE  *file ;
    long  fileSize ;
    struct  stat  info ;





/* Open the input file. */

    fileName = fnm_build(TRUE, FnmPath, fileName, NULL) ;
    file = fopen(fileName, "r") ;
    if (file == NULL) {
        vperror("(meo_load) Error opening %s.\n", fileName) ;
        return(errno) ;
    }

/* Determine the amount of data to be loaded from the file. */

    if (fstat(fileno(file), &info)) {
        vperror("(meo_load) Error determining the size of %s.\n", fileName) ;
        return(errno) ;
    }

    fileSize = info.st_size - offset ;

/* Allocate a memory buffer, if necessary. */

    if (*startAddress == NULL) {
        *numBytes = fileSize ;
        *startAddress = malloc(fileSize) ;
        if (*startAddress == NULL) {
            vperror("(meo_load) Error allocating %ld-byte memory buffer for %s.\n",
                     fileSize, fileName) ;
            return(errno) ;
        }
    }

/* Read the (possibly truncated) contents of the file into the memory pool. */

    if (fseek(file, offset, SEEK_SET) != offset) {
        vperror("(meo_load) Error positioning to offset %ld in %s.\nfseek: ",
                 offset, fileName) ;
        return(errno) ;
    }

    if (fileSize > *numBytes)  fileSize = *numBytes ;

    if (fread(*startAddress, fileSize, 1, file) != 1) {
        vperror("(meo_load) Error loading %ld bytes from %s to %p.\nfread: ",
                 fileSize, fileName, *startAddress) ;
        return(errno) ;
    }

/* Close the file. */

    fclose(file) ;

    if (meo_util_debug)  printf("(meo_load) Loaded %ld bytes from %s to %p.\n",
                                 fileSize, fileName, *startAddress) ;

    return(0) ;

}

/*******************************************************************************

Procedure:

    meo_save ()

    Save Memory to a File.


Purpose:

    Function meo_save() saves the binary contents of a memory region to a
    disk file.  The contents can be reloaded at a later time using meo_load().


    Invocation:

        status = meo_save(startAddress, numBytes, fileName, offset) ;

    where:

        <startAddress>  - I
            specifies the start of the memory region that is to be saved.
        <numBytes>      - I
            specifies the number of bytes of data to save.
        <fileName>      - I
            is the name of the file to which the memory contents will be
            written.  Environment variables may be embedded in the file name.
        <offset>        - I
            is the byte offset within the file at which the save will begin.
        <status>        - O
            returns the status of saving the memory to a file, zero if
            there were no errors and ERRNO otherwise.

*******************************************************************************/


int  meo_save (

#    if __STDC__
        void  *startAddress,
        long  numBytes,
        const  char  *fileName,
        long  offset)
#    else
        startAddress, numBytes, fileName, offset)

        void  *startAddress ;
        long  numBytes ;
        char  *fileName ;
        long  offset ;
#    endif

{    /* Local variables. */
    FILE  *file ;




/* Open the output file. */

    fileName = fnm_build(TRUE, FnmPath, fileName, NULL) ;
    file = fopen(fileName, "w") ;
    if (file == NULL) {
        vperror("(meo_save) Error opening %s to save memory at %p.\n",
                 fileName, startAddress) ;
        return(errno) ;
    }

/* Write the contents of the memory out to the file. */

    if (fseek(file, offset, SEEK_SET) != offset) {
        vperror("(meo_save) Error positioning to offset %ld in %s.\nfseek: ",
                 offset, fileName) ;
        return(errno) ;
    }

    if (fwrite(startAddress, numBytes, 1, file) != 1) {
        vperror("(meo_save) Error saving %ld bytes at %p to %s.\n",
                 numBytes, startAddress, fileName) ;
        return(errno) ;
    }

/* Close the file. */

    if (fclose(file)) {
        vperror("(meo_save) Error closing %s for memory at %p.\n",
                 fileName, startAddress) ;
        return(errno) ;
    }

    if (meo_util_debug)  printf("(meo_save) Saved %ld bytes at %p to %s\n",
                                 numBytes, startAddress, fileName) ;

    return(0) ;

}

#ifdef  TEST

/*******************************************************************************

    Program to test the MEO_UTIL() functions.

    Under UNIX,
        compile and link as follows:
            % cc -g -DTEST meo_util.c -I<... includes ...> <libraries ...>
        and run with the following command line:
            % a.out <wildcard_file_spec>

    Under VxWorks,
        compile and link as follows:
            % cc -g -c -DTEST -DVXWORKS meo_util.c -I<... includes ...> \
                       -o test_drs.o
            % ld -r test_drs.o <libraries ...> -o test_drs.vx.o
        load as follows:
            -> ld <test_drs.vx.o
        and run with the following command line:
            -> test_drs.vx.o "<wildcard_file_spec>"

*******************************************************************************/

#ifdef VXWORKS

test_meo (fileName)
    char  *fileName ;
{    /* Local variables. */

#else

main (argc, argv)
    int  argc ;
    char  *argv[] ;
{    /* Local variables. */
    char  *fileName = argv[1] ;

#endif

    unsigned  char  buffer[256] ;
    long  i, numBytes ;
    void  *newBuffer ;




    meo_util_debug = 1 ;
    vperror_print = 1 ;

    for (i = 0 ;  i < sizeof buffer ;  i++)
        buffer[i] = i ;

    meo_save(buffer, sizeof buffer, fileName, 0) ;

    newBuffer = NULL ;
    meo_load(fileName, 0, &newBuffer, &numBytes) ;

    printf("\nOctal Dump:\n\n") ;
    meo_dumpo(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nDecimal Dump:\n\n") ;
    meo_dumpd(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nHexadecimal Dump:\n\n") ;
    meo_dumpx(NULL, "    ", 0, newBuffer, numBytes) ;
    printf("\nText Dump:\n\n") ;
    meo_dumpt(NULL, "    ", 0, newBuffer, numBytes) ;

}
#endif
