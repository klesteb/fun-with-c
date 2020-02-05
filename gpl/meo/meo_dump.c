
/*---------------------------------------------------------------------------*/
/*  Copyright (c) 1996 by Charles A. Measday                                 */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that the above copyright notice appears in all copies. The      */
/*  author makes no representations about the suitability of this software   */
/*  for any purpose. It is provided "as is" without express or implied       */
/*  warranty.                                                                */
/*---------------------------------------------------------------------------*/

#include "meo_priv.h"

/*----------------------------------------------------------------------*/

/**/

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

{
/*
 * Function: meo_dump.c
 * Version : 1.0
 * Created : 19-Nov-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function meo_dump() formats the binary contents of a memory region 
 *    in ASCII and writes the ASCII dump to a file. Each output line looks 
 *    as follows:
 *
 *        <address>:  <data1> <data2> ... <dataN>  "data/ASCII"
 *
 *    The data items, DATA1, DATA2, etc., are formatted in decimal, 
 *    hexadecimal, or octal notation.
 *
 *    Invocation:
 *
 *        status = meo_dump(file, indentation, base, numBytesPerLine,
 *                          address, buffer, numBytesToDump) ;
 *
 *    where
 *
 *        <file>              - I
 *            Is the Unix FILE* handle for the output file.  If FILE is NULL,
 *            the dump is written to standard output.
 *
 *        <indentation>       - I
 *            Is a text string used to indent each line of the dump.  The
 *            string is used as the format string in an FPRINTF(3) statement,
 *            so you can embed anything you want. This argument can be NULL.
 *
 *        <base>              - I
 *            Specifies the output base for the dump: "MeoOctal" for octal,
 *            "MeoDecimal" for decimal, "MeoHexadecimal" for hexadecimal,
 *            and "MeoText" for text. (These are enumerated values defined
 *            in the "meo_util.h" header file.)
 *
 *        <numBytesPerLine>   - I
 *            Specifies the number of bytes in the buffer to be formatted and
 *            dumped on a single line of output. Good values are 8 values per
 *            line for octal, 8 for decimal, 16 for hexadecimal, and 40 for 
 *            text.
 *
 *        <address>           - I
 *            Is the value to be displayed in the address field.  It can be
 *            different than the actual buffer address.
 *
 *        <buffer>            - I
 *            Is the buffer of data to be dumped.
 *
 *        <numBytesToDump>    - I
 *            Is exactly what it says it is!
 *
 *        <status>            - O
 *            Returns the status of generating the dump, zero if there were
 *            no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  c, *out, *outbuf;
    int  alignment, integer, numBytesDumped, numBytesThisLine;
    unsigned  char  *buf, *inbuf;

/*
 * Main part of function.
 */

    if (file == NULL)  file = stdout ;

    /* Allocate an input buffer to hold the bytes to be dumped. Doing   */
    /* this prevents bus errors should the input buffer be misaligned   */
    /* (i.e., on an odd-byte boundary).                                 */

    if ((inbuf = (unsigned char *)malloc(numBytesPerLine)) == NULL) {

        vperror("(meo_dump) Error allocating temporary, %d-byte input buffer.\nmalloc: ",
                 numBytesPerLine);
        return(errno);

    }

    /* Allocate a string in which to build each output line.            */

    if ((outbuf = (char *)malloc(numBytesPerLine * 6)) == NULL) {

        vperror("(meo_dump) Error allocating temporary, %d-byte output buffer.\nmalloc: ",
                 numBytesPerLine * 6);
        return(errno);

    }

    /* Generate each line of the dump.                                  */

    while (numBytesToDump > 0) {

        numBytesThisLine = (numBytesToDump > numBytesPerLine)
                           ? numBytesPerLine : numBytesToDump;

        memset(inbuf, '\0', numBytesPerLine);         /* Zero trailing bytes. */
        memcpy(inbuf, buffer, numBytesThisLine);

        /* Output the line indentation and the memory address.          */

        if (indentation != NULL)  fprintf(file, indentation);
#ifdef YOU_WANT_VARIABLE_LENGTH_ADDRESS
        fprintf(file, "%p:\t", address);
#else
        fprintf(file, "%08lX: ", (long) address);     /* Assumes 32-bit address. */
#endif

        /* Format the data in the requested base.                       */

        buf = inbuf;  
        numBytesDumped = 0;
        out = outbuf;

        switch (base) {
            case MeoOctal: /* Base 8 - display the contents of each byte as an octal number. */
                while (numBytesDumped < numBytesPerLine) {

                    if (numBytesDumped++ < numBytesThisLine) {

                        sprintf (out, " %3.3o", *buf++);

                    } else sprintf (out, " %3s", " ");

                    out = out + strlen(out);

                }
                break;

            case MeoDecimal: /* Base 10 - display the contents of each byte as a decimal number. */
                while (numBytesDumped < numBytesPerLine) {

                    if (numBytesDumped++ < numBytesThisLine) {

                        sprintf(out, " %3u", *buf++);

                    } else sprintf(out, " %3s", " ");

                    out = out + strlen(out);

                }
                break;

            case MeoHexadecimal: /* Base 16 - display the contents of each integer as a hexadecimal number. */
                while (numBytesDumped < numBytesPerLine) {

                    if (numBytesDumped < numBytesThisLine) {

                        alignment = (long) buf % sizeof(int);

                        if (alignment > 0) {

                            for (integer = 0; alignment > 0;  alignment--)
                                integer = (integer << 8) | (*buf++ & 0x0FF);

                        } else {

                            integer = *((int *) buf);
                            buf = buf + sizeof(int);

                        }

                        sprintf(out, " %08X", integer);

                    } else sprintf(out, " %8s", " ");

                    out = out + strlen(out);
                    numBytesDumped = numBytesDumped + sizeof(int);

                }
                break;

            case MeoText: /* "Base 26" - treat the data as ASCII text. */
            default:
                break;

        }

        /* Append the ASCII version of the buffer.                      */

        if (base != MeoText) {

            *out++ = ' ';  
            *out++ = ' ';

        }

        *out++ = '"';
        memcpy(out, buffer, numBytesThisLine);
        numBytesDumped = 0;

        while (numBytesDumped++ < numBytesThisLine) {

            if (isascii(*out) && isprint(*out)) {

                c = *out;  
                *out++ = c;

            } else *out++ = '.';

        }

        *out++ = '"';  
        *out++ = '\0';

        /* Output the dump line to the file.                            */

        fprintf(file, "%s\n", outbuf);

        /* Adjust the pointers and counters for the next line.          */

        address = (char *) address + numBytesThisLine;
        buffer = (char *) buffer + numBytesThisLine;
        numBytesToDump -= numBytesThisLine;

    }

    /* Deallocate the temporary input/output buffers.                   */

    free(inbuf);
    free(outbuf);

    return(0);

}

