
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

#include "fnm_priv.h"

/*----------------------------------------------------------------------*/

/**/

FileName  fnm_fill_parts(

#    if  __STDC__
        int expand,
        const  FileName  fileName,
        const  FileName  defaults)
#    else
        expand, fileName, defaults)

        int expand;
        FileName  fileName;
        FileName  defaults;
#    endif

{
/*
 * Function: fnm_fill_parts.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    Function fnm_fill_parts() fills the missing parts of a file name with 
 *    the corresponding parts from a defaults file name.
 *
 *    Invocation:
 *
 *        result = fnm_fill_parts(expand, fileName, defaults);
 *
 *    where
 *
 *        <expand>        - I
 *            A flag to indicate wither Unix environment variables should
 *            be expanded in the filespec.
 *
 *        <fileName>      - I
 *            is the handle returned by fnm_create() for the file name in 
 *            question.
 *
 *        <defaults>      - I
 *            is the handle returned by fnm_create() for the file name 
 *            containing the defaults.
 *
 *        <result>        - O
 *            returns a handle for a new file name consisting of the old file
 *            name with missing parts supplied by the defaults file name. NULL
 *            is returned in the event of an error.
 *
 * Modification History
 *
 *    K.Esteb 23-Apr-2002
 *        Added the "expand" parameter to signal wither to expand Unix
 *        environment variabls.
 *
 * Variables Used
 */

    char  *ddef, *dnew, pathname[PATH_MAX];
    FileName  result;
    int  ldef, lnew;

/*
 * Main part of function.
 */

    strcpy (pathname, "") ;

    /* Substitute the node name.                                        */

    if ((fileName == NULL) || (fileName->node == NULL)) {

        if (defaults->node != NULL)  strcat(pathname, defaults->node);

    } else {

        strcat(pathname, fileName->node) ;

    }

    /* Substitute the directory.  First, process dot directories in the */
    /* new file specification ("fileName"). Single dots (current        */
    /* directory) are replaced by the current working directory; double */
    /* dots (parent directory) remove successive child directories from */
    /* the default file specfication ("defaults"). Dot directories in   */
    /* the default FS have no effect, unless the new FS has no directory*/
    /*  yet.                                                            */

    dnew = ((fileName == NULL) || (fileName->directory == NULL))
           ? "" : fileName->directory;
    lnew = strlen(dnew);
    ddef = (defaults->directory == NULL) ? "" : defaults->directory;
    ldef = strlen(ddef);
    ddef = defaults->directory + ldef;          /* DDEF points at '\0'. */

    /* Prior to loop: DDEF points to end of (N+1)-th component of       */
    /* directory. Be careful making changes to this code - it's not very*/
    /* straightforward. It should handle cases like the "/" directory,  */
    /* no dot directories, and so on.                                   */

    while ((lnew > 0) && (ldef > 0)) {

        if (strncmp(dnew, "../", 3) == 0) {        /* Up one directory. */

            dnew = dnew + 3 ;  lnew = lnew - 3;
            do { ldef-- ; } while ((ldef > 0) && (*--ddef != '/'));

        } else if (strncmp(dnew, "..", 2) == 0) {  /* Up one directory. */

            dnew = dnew + 2;
            lnew = lnew - 2;
            do { ldef-- ; } while ((ldef > 0) && (*--ddef != '/'));

        } else if (strncmp(dnew, "./", 2) == 0) {  /* Current directory.*/

            dnew = dnew + 2; lnew = lnew - 2;

        } else if (strncmp(dnew, ".", 1) == 0) {   /* Current directory.*/

            dnew = dnew + 1; lnew = lnew - 1;

        } else {                                   /* No dot directory. */

            break;

        }

    }

    /* After loop:  DDEF points to end of (N+1-M)-th component of       */
    /* directory, where M is the number of ".." (parent) directories    */
    /* processed.  Get rid of the "+1"-th component.                    */

    while ((ldef > 0) && (*--ddef != '/'))  ldef--;

    ddef = (defaults->directory == NULL) ? "" : defaults->directory;

    /* After processing the dot directories, perform the actual         */
    /* directory substitutions.  This procedure is complicated by the   */
    /* two types of directories, absolute and relative.  If the new     */
    /* directory and the default directory are both absolute or both    */
    /* relative, use the new directory. If one directory is relative    */
    /* and the other absolute, append the relative directory to the     */
    /* absolute directory.                                              */

    if (lnew == 0) {                    /* No previous directory spec.  */

        strncat(pathname, ddef, ldef);

    } else if ((strcmp(dnew, ".") == 0) || (strncmp(dnew, "./", 2) == 0)) {

        /* Dot directories in default FS won't have any effect; use new FS. */

        strncat(pathname, dnew, lnew); 

    } else if ((strcmp(dnew, "..") == 0) || (strncmp(dnew, "../", 3) == 0)) {

        /* Dot directories in default FS won't have any effect; use new FS. */

        strncat(pathname, dnew, lnew);

    } else if (*ddef == '/') {

        if (*dnew == '/') {               /* Two absolute directory specs. */

            strncat(pathname, dnew, lnew);

        } else {                          /* Append relative to absolute. */

            strncat(pathname, ddef, ldef);
            strncat(pathname, dnew, lnew);

        }

    } else {

        if (*dnew == '/') {             /* Append relative to absolute. */

            strncat(pathname, dnew, lnew);
            strncat(pathname, ddef, ldef);

        } else {                            /* Two relative directory specs. */

            strncat(pathname, dnew, lnew);

        }

    }

    /* Substitute the file name.                                        */

    if ((fileName == NULL) || (fileName->name == NULL)) {

        if (defaults->name != NULL) strcat(pathname, defaults->name);

    } else {

        strcat(pathname, fileName->name);

    }

    /* Substitute the extension.                                        */

    if ((fileName == NULL) || (fileName->extension == NULL)) {

        if (defaults->extension != NULL) strcat(pathname, defaults->extension);

    } else {

        strcat(pathname, fileName->extension);

    }

    /* Substitute the version number.                                   */

    if ((fileName == NULL) || (fileName->version == NULL)) {

        if (defaults->version != NULL)  strcat(pathname, defaults->version);

    } else {

        strcat(pathname, fileName->version);

    }

    /* Construct a file name structure for the resulting file name.     */

    if ((result = fnm_new(pathname, expand)) == NULL) {

        vperror("(fnm_fill_parts) Error creating result: %s\nfnm_new: ", pathname);
        return(NULL);

    }

    fnm_locate_parts(result);

    return(result);

}

/**/

int  fnm_locate_parts(

#    if  __STDC__
        FileName  fileName)
#    else
        fileName)

        FileName  fileName;
#    endif

{
/*
 * Function: fnm_locate_parts.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_locate_parts() function determines the locations of the different
 *    parts of a file name, e.g., the directory, the name, the extension, etc.
 *
 *    Invocation:
 *
 *        status = fnm_locate_parts(fileName);
 *
 *    where
 *
 *        <fileName>      - I
 *            is the file name handle returned by fnm_create().
 *
 *        <status>        - O
 *            returns the status of dissecting the file name, zero if there
 *            were no errors and ERRNO otherwise.
 *
 * Modification History
 *
 * Variables Used
 */

    char  *fs, pathname[PATH_MAX], *s ;

/*
 * Main part of function.
 */

    /* Advance the "fs" pointer as you scan the file specification.     */

    strcpy(pathname, fileName->path);
    fs = pathname;

    /* First, if the file specification contains multiple pathnames,    */
    /* separated by commas or spaces, discard the trailing pathnames.   */

    if ((s = strchr(fs, ' ')) != NULL)  *s = '\0';
    if ((s = strchr(fs, ',')) != NULL)  *s = '\0';

    /* Locate the node.  The node name is separated from the rest of    */
    /* the file name by a colon (":").                                  */

    if ((s = strchr(fs, ':')) != NULL) {

        fileName->node = strndup(fs, (int)(s - fs + 1));
        if (fileName->node == NULL) {

            vperror("(fnm_locate_parts) Error duplicating node of %s.\nstrndup: ",
                     fileName->path);

            return(errno);

        }

        fs = ++s;

    }

    /* Locate the directory. The directory extends through the last     */
    /* "/" in the file name.                                            */

    if ((s = strrchr(fs, '/')) != NULL) {

        fileName->directory = strndup(fs, (int)(s - fs + 1));
        if (fileName->directory == NULL) {

            vperror ("(fnm_locate_parts) Error duplicating directory of %s.\nstrndup: ",
                     fileName->path);
            return(errno);

        }

        fs = ++s ;

    }

    /* The remainder of the pathname is the combined name, extension,   */
    /* and version number.                                              */

    if (*fs != '\0') {

        fileName->file = strdup(fs);
        if (fileName->file == NULL) {

            vperror("(fnm_locate_parts) Error duplicating file of %s.\nstrdup: ",
                    fileName->path);
            return(errno);

        }

    }

    /* Locate the version number. Since version numbers are not part of */
    /* UNIX, these version numbers are a user convention. Any file      */
    /* extension that can be converted to an integer is considered a    */
    /* version number; e.g., ".007", etc.  (So we can make this test, a */
    /* version number of zero is not allowed.                           */

    if (((s = strrchr(fs, '.')) != NULL) && (atoi(++s) != 0)) {

        fileName->version = strdup(--s);
        if (fileName->version == NULL) {

            vperror("(fnm_locate_parts) Error duplicating version of %s.\nstrdup: ",
                    fileName->path);
            return(errno);

        }

        *s = '\0';                      /* Exclude version temporarily. */

    }

    /* Locate the extension.  The extension is the last part of the     */
    /* file name preceded by a "." (not including the version number,   */
    /* though).                                                         */

    if ((s = strrchr(fs, '.')) != NULL) {

        fileName->extension = strdup(s) ;
        if (fileName->extension == NULL) {

            vperror("(fnm_locate_parts) Error duplicating extension of %s.\nstrdup: ",
                    fileName->path);
            return(errno);

        }

        *s = '\0';                     /* Exclude extension temporarily.*/

    }

    /* Locate the name. The name is the rest of the file name, excluding*/
    /* the last extension and the version number, if any.               */

    if (*fs != '\0') {

        fileName->name = strdup(fs);
        if (fileName->name == NULL) {

            vperror("(fnm_locate_parts) Error duplicating name of %s.\nstrdup: ",
                    fileName->path);
            return(errno);

        }

    }

    /* Restore extension and version.                                   */

    if (fileName->extension != NULL)  fs[strlen(fs)] = '.';
    if (fileName->version != NULL)  fs[strlen(fs)] = '.';

    return(0);

}

/**/

FileName  fnm_new(

#    if  __STDC__
        const  char  *pathname, int expand)
#    else
        pathname, expand)

        char  *pathname;
        int expand;
#    endif

{
/*
 * Function: fnm_new.c
 * Version : 1.0
 * Created : 12-Jul-1996
 * Author  : Charles Measday
 *
 * Description
 *
 *    The fnm_new() function allocates and initializes a file name structure.
 *
 *    Invocation:
 *
 *        fileName = fnm_new(pathname, expand);
 *
 *    where
 *
 *        <pathname>          - I
 *            Is the initial pathname.
 *
 *        <expand>            - I
 *            Flag to indicate wither any Unix environment variables will be 
 *            expanded.
 *
 *        <fileName>          - O
 *            Returns a pointer to the allocated file name structure.  The
 *            caller is responible for FREE(3)ing the structure when it is
 *            no longer needed. NULL is returned in the event of an error.
 *
 * Modification History
 *
 *    K.Esteb 23-Apr-2002
 *        Added the "expand" parameter to signal wither to expand Unix
 *        environment variabls.
 *
 * Variables Used
 */

    char  expandedPathname[PATH_MAX];
    FileName  fileName;
    int  status;

/*
 * Main part of function.
 */

    /* Allocate the file name structure.                                */

    fileName = (FileName)malloc(sizeof (_FileName));
    if (fileName == NULL) {

        vperror("(fnm_new) Error allocating structure for %s.\n", pathname);
        return(NULL);

    }

    /* Initialize the structure.                                        */

    if (pathname == NULL) {

        fileName->path = NULL ;

    } else {

        if (expand) {

            str_env(pathname, -1, expandedPathname, sizeof expandedPathname);
            fileName->path = strdup(expandedPathname);

        } else {

            fileName->path = strdup(pathname);

        }

        if (fileName->path == NULL) {

            vperror("(fnm_new) Error duplicating pathname: %s\n",
                    expandedPathname);
            status = errno;
            free(fileName);
            errno = status;
            return(NULL);

        }

    }

    fileName->node = NULL;
    fileName->directory = NULL;
    fileName->file = NULL;
    fileName->name = NULL;
    fileName->extension = NULL;
    fileName->version = NULL;

    return(fileName);

}

