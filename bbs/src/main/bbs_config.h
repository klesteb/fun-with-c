
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#ifndef _CONFIG_H
#define _CONFIG_H

/* there is many ways of doing configuration. using constants and compile  */
/* time configuration is as good as any other. especially when there are   */
/* things that can't be changed with just a restart.                       */

#define NODENAME  "mybbs"           /* network node name                   */  
#define HUMANNODE "My BBS"          /* human readable node name            */
#define SERIALNUM "119319218"       /* system serial number                */
#define NETWORKED FALSE             /* this system belongs to a network    */

#define BBSDIR   "../../"           /* root directory of the bbs           */
#define DATAPATH "../../data/"      /* where the data files are located    */
#define DOORPATH "../../doors/"     /* where the doors are located         */
#define MSGPATH  "../../messages/"  /* where the messages are located      */
#define TEXTPATH "../../text/"      /* where the text files are located    */
#define WORKPATH "../../work/"      /* work space                          */

#define MSGBASE 1                   /* base message number                 */
#define TIMEOUT 1                   /* timeout for file locking            */
#define RETRIES 30                  /* retires for file locking            */

#define CREATAIDE FALSE             /* automatic room aide assignments     */
#define LOBBYPOST AX_AIDE           /* AX level for posting in the lobby   */
#define MAKEROOM  AX_AIDE           /* AX level to create a new room       */ 
#define INITAX    AX_NEW            /* AX level for new users              */
#define VALIDAX   AX_NORM           /* AX level for validated users        */
#define REGISCALL TRUE              /* user must register on first call    */
#define TIMELIM   60                /* default time limit on system        */

/* change these at your peril. set them once, they effect the sizes of the */
/* datastores. the bbs will create files on initial startup using these    */
/* numbers. changing the datastore sizes must be done external to the      */
/* bbs. once that is done. then these can be changed to reflect the new    */
/* reality. this new reality will necessitate a recompile.                 */ 

#define ROOMNUM 256                 /* max number of rooms                 */
#define NODENUM 32                  /* max number of nodes                 */
#define USERNUM 256                 /* max number of users                 */
#define DOORNUM 32                  /* max number of doors                 */

#endif

