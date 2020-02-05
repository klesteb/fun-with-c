
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

{
/*
 * Function: meo_save.c
 * Version : 1.0
 * Created : 19-Nov-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function meo_save() saves the binary contents of a memory region 
 *    to a disk file. The contents can be reloaded at a later time using 
 *    meo_load().
 *
 *    Invocation:
 *
 *        status = meo_save(startAddress, numBytes, fileName, offset);
 *
 *    where:
 *
 *        <startAddress>      - I
 *            Specifies the start of the memory region that is to be saved.
 *
 *        <numBytes>          - I
 *            Specifies the number of bytes of data to save.
 *
 *        <fileName>          - I
 *            Is the name of the file to which the memory contents will be
 *            written. Environment variables may be embedded in the file name.
 *
 *        <offset>            - I
 *            Is the byte offset within the file at which the save will begin.
 *
 *        <status>            - O
 *            Returns the status of saving the memory to a file, zero if
 *            there were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    FILE  *file;

/*
 * Main part of function.
 */

    /* Open the output file.                                            */

    fileName = fnm_build(TRUE, FnmPath, fileName, NULL);
    file = fopen(fileName, "w");
    if (file == NULL) {

        vperror("(meo_save) Error opening %s to save memory at %p.\n",
                 fileName, startAddress);
        return(errno);

    }

    /* Write the contents of the memory out to the file.                */

    if (fseek(file, offset, SEEK_SET) != offset) {

        vperror("(meo_save) Error positioning to offset %ld in %s.\nfseek: ",
                 offset, fileName);
        return(errno);

    }

    if (fwrite(startAddress, numBytes, 1, file) != 1) {

        vperror("(meo_save) Error saving %ld bytes at %p to %s.\n",
                 numBytes, startAddress, fileName);
        return(errno);

    }

    /* Close the file. */

    if (fclose(file)) {

        vperror("(meo_save) Error closing %s for memory at %p.\n",
                 fileName, startAddress);
        return(errno);

    }

    if (meo_util_debug)  printf("(meo_save) Saved %ld bytes at %p to %s\n",
                                 numBytes, startAddress, fileName);

    return(0);

}

