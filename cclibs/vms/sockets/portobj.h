/*
 * This include file contains the socket/object servers correspondence
 * for a UN*X-to-VMS server conversion.
 *
 * $NCDId: @(#)portobj.h,v 1.3 1992/04/30 17:05:27 hardison Exp $
 *
 * OBH - NCD - 3/11/92
 */
#include descrip

#define X11_PORT         6000
#define FONT_SERVER_PORT 7000
/*
 * Add more here!...
 */

static $DESCRIPTOR (x11_dsc, "X$X0");
static $DESCRIPTOR (font_server_dsc, "FONT$FS");
/*
 * Add more here!...
 */

static struct translationTable
{
        int port;
        struct dsc$descriptor *object;
} transTable[] =
{
        { X11_PORT, &x11_dsc },
        { FONT_SERVER_PORT, &font_server_dsc },
/*
 * ...Then add more here!
 */
};
