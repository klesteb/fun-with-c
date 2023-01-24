
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

#ifndef _INCLUDES_H
#define _INCLUDES_H

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "include/when.h"

#include "rms/files.h"
#include "gpl/fnm_util.h"
#include "events/event.h"
#include "errors/errors.h"
#include "tracer/tracer.h"

#include "cclibs/misc/misc.h"
#include "cclibs/misc/dates.h"

#include "widgets/window.h"
#include "widgets/colors.h"
#include "widgets/window.h"
#include "widgets/workbench.h"
#include "widgets/component.h"
#include "widgets/windows/alert.h"
#include "widgets/components/more.h"
#include "widgets/components/text.h"
#include "widgets/components/vline.h"
#include "widgets/windows/bar_menu.h"
#include "widgets/windows/box_menu.h"
#include "widgets/windows/base_window.h"
#include "widgets/windows/query_window.h"
#include "widgets/components/menus/list.h"
#include "widgets/components/menus/menus.h"

#include "bbs/src/bitops.h"
#include "bbs/src/door/door.h"
#include "bbs/src/room/msgs.h"
#include "bbs/src/node/node.h"
#include "bbs/src/room/room.h"
#include "bbs/src/user/user.h"
#include "bbs/src/room/doors.h"
#include "bbs/src/finds/finds.h"
#include "bbs/src/user/profile.h"
#include "bbs/src/main/bbs_protos.h"
#include "bbs/src/main/bbs_config.h"
#include "bbs/src/main/bbs_error_codes.h"

#endif

