
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

{
/*
 * Function: meo_load.c
 * Version : 1.0
 * Created : 19-Nov-1998
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function meo_load() loads the binary contents of a memory region 
 *    from a disk file. The contents must have been previously saved 
 *    using meo_save().
 *
 *    Invocation (dynamically allocated buffer):
 *
 *    void  *startAddress = NULL;
 *    ...
 *    status = meo_load(fileName, offset, &startAddress, &numBytes);
 *
 *    Invocation (caller-specified buffer):
 *
 *        void  *startAddress = buffer;
 *        long  numBytes = sizeof buffer;
 *        ...
 *        status = meo_load(fileName, offset, &startAddress, &numBytes);
 *
 *    where:
 *
 *        <fileName>      - I
 *            Is the name of the file from which the memory contents will 
 *            be loaded. Environment variables may be embedded in the file 
 *            name.
 *
 *        <offset>        - I
 *            Is the byte offset within the file from which the load will 
 *            begin.
 *
 *        <startAddress>  - I/O
 *            Is the address of a (VOID *) pointer that specifies or 
 *            returns the address where the contents of the file will be 
 *            stored.  If the (VOID *) pointer is NULL, meo_load() will 
 *            MALLOC(3) a buffer for the file contents and return its address 
 *            through this argument; the caller is responsible for FREE(3)ing 
 *            the memory when it is no longer needed. If the (VOID *) 
 *            pointer is NOT NULL, meo_load() uses it as the address of 
 *            a caller-allocated buffer in which the file contents are to 
 *            be stored; the size of the buffer is specified by the NUMBYTES 
 *            argument.
 *
 *        <numBytes>      - I/O
 *            Is the address of a longword that specifies or returns the 
 *            size of the memory buffer. If the memory buffer is dynamically  
 *            allocated by meo_load(), this argument returns the size of 
 *            the buffer. If meo_load() uses a caller-allocated buffer, this 
 *            argument specifies the size of the buffer.
 *
 *        <status>        - O
 *            Returns the status of loading the memory from a file, zero
 *            if there were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    FILE  *file;
    long  fileSize;
    struct  stat  info;

/*
 * Main part of function.
 */

    /* Open the input file.                                             */

    fileName = fnm_build(TRUE, FnmPath, fileName, NULL);
    file = fopen(fileName, "r");
    if (file == NULL) {

        vperror("(meo_load) Error opening %s.\n", fileName);
        return(errno);

    }

    /* Determine the amount of data to be loaded from the file.         */

    if (fstat(fileno(file), &info)) {

        vperror("(meo_load) Error determining the size of %s.\n", fileName);
        return(errno);

    }

    fileSize = info.st_size - offset;

    /* Allocate a memory buffer, if necessary.                          */

    if (*startAddress == NULL) {

        *numBytes = fileSize;

        if ((*startAddress = malloc(fileSize)) == NULL) {

            vperror("(meo_load) Error allocating %ld-byte memory buffer for %s.\n",
                     fileSize, fileName);
            return(errno);

        }

    }

    /* Read the (possibly truncated) contents of the file into the      */
    /* memory pool.                                                     */

    if (fseek(file, offset, SEEK_SET) != offset) {

        vperror("(meo_load) Error positioning to offset %ld in %s.\nfseek: ",
                 offset, fileName);
        return(errno);

    }

    if (fileSize > *numBytes)  fileSize = *numBytes;

    if (fread(*startAddress, fileSize, 1, file) != 1) {

        vperror("(meo_load) Error loading %ld bytes from %s to %p.\nfread: ",
                 fileSize, fileName, *startAddress);
        return(errno);

    }

    /* Close the file.                                                  */

    fclose(file);

    if (meo_util_debug)  printf("(meo_load) Loaded %ld bytes from %s to %p.\n",
                                 fileSize, fileName, *startAddress);

    return(0);

}

