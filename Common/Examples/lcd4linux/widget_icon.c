/* $Id: widget_icon.c,v 1.17 2005/05/08 04:32:45 reinelt Exp $
 *
 * icon widget handling
 *
 * Copyright (C) 2003, 2004 Michael Reinelt <reinelt@eunet.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * $Log: widget_icon.c,v $
 * Revision 1.17  2005/05/08 04:32:45  reinelt
 * CodingStyle added and applied
 *
 * Revision 1.16  2005/01/18 06:30:24  reinelt
 * added (C) to all copyright statements
 *
 * Revision 1.15  2004/11/28 15:50:24  reinelt
 * Cwlinux fixes (invalidation of user-defined chars)
 *
 * Revision 1.14  2004/06/26 12:05:00  reinelt
 *
 * uh-oh... the last CVS log message messed up things a lot...
 *
 * Revision 1.13  2004/06/26 09:27:21  reinelt
 *
 * added '-W' to CFLAGS
 * changed all C++ comments to C ones
 * cleaned up a lot of signed/unsigned mistakes
 *
 * Revision 1.12  2004/06/20 10:09:56  reinelt
 *
 * 'const'ified the whole source
 *
 * Revision 1.11  2004/03/11 06:39:59  reinelt
 * big patch from Martin:
 * - reuse filehandles
 * - memory leaks fixed
 * - earlier busy-flag checking with HD44780
 * - reuse memory for strings in RESULT and hash
 * - netdev_fast to wavid time-consuming regex
 *
 * Revision 1.10  2004/03/06 20:31:16  reinelt
 * Complete rewrite of the evaluator to get rid of the code
 * from mark Morley (because of license issues).
 * The new Evaluator does a pre-compile of expressions, and
 * stores them in trees. Therefore it should be reasonable faster...
 *
 * Revision 1.9  2004/03/03 04:44:16  reinelt
 * changes (cosmetics?) to the big patch from Martin
 * hash patch un-applied
 *
 * Revision 1.8  2004/03/03 03:47:04  reinelt
 * big patch from Martin Hejl:
 * - use qprintf() where appropriate
 * - save CPU cycles on gettimeofday()
 * - add quit() functions to free allocated memory
 * - fixed lots of memory leaks
 *
 * Revision 1.7  2004/02/15 21:43:43  reinelt
 * T6963 driver nearly finished
 * framework for graphic displays done
 * i2c_sensors patch from Xavier
 * some more old generation files removed
 *
 * Revision 1.6  2004/02/07 13:45:23  reinelt
 * icon visibility patch #2 from Xavier
 *
 * Revision 1.5  2004/02/04 19:11:44  reinelt
 * icon visibility patch from Xavier
 *
 * Revision 1.4  2004/01/30 20:57:56  reinelt
 * HD44780 patch from Martin Hejl
 * dmalloc integrated
 *
 * Revision 1.3  2004/01/29 04:40:03  reinelt
 * every .c file includes "config.h" now
 *
 * Revision 1.2  2004/01/23 07:04:39  reinelt
 * icons finished!
 *
 * Revision 1.1  2004/01/23 04:54:03  reinelt
 * icon widget added (not finished yet!)
 *
 */

/* 
 * exported functions:
 *
 * WIDGET_CLASS Widget_Icon
 *   the icon widget
 *
 */


#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "debug.h"
#include "cfg.h"
#include "qprintf.h"
#include "evaluator.h"
#include "timer.h"
#include "widget.h"
#include "widget_icon.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif

/* icons always are 8 pixels high  */
#define YRES 8

static void widget_icon_read_bitmap(const char *section, WIDGET_ICON * Icon)
{
    int row, n;
    char key[15];
    char *val, *v;
    unsigned char *map;

    for (row = 0; row < YRES; row++) {
	qprintf(key, sizeof(key), "Bitmap.Row%d", row + 1);
	val = cfg_get(section, key, "");
	map = Icon->bitmap + row;
	n = 0;
	for (v = val; *v != '\0'; v++) {
	    if (n >= Icon->maxmap) {
		Icon->maxmap++;
		Icon->bitmap = realloc(Icon->bitmap, Icon->maxmap * YRES * sizeof(char));
		memset(Icon->bitmap + n * YRES, 0, YRES * sizeof(char));
		map = Icon->bitmap + n * YRES + row;
	    }
	    switch (*v) {
	    case '|':
		n++;
		map += YRES;
		break;
	    case '*':
		(*map) <<= 1;
		(*map) |= 1;
		break;
	    default:
		(*map) <<= 1;
	    }
	}
	free(val);
    }
}


void widget_icon_update(void *Self)
{
    WIDGET *W = (WIDGET *) Self;
    WIDGET_ICON *Icon = W->data;
    RESULT result = { 0, 0, 0, NULL };

    /* evaluate expressions */
    Icon->speed = 100;
    if (Icon->speed_tree != NULL) {
	Eval(Icon->speed_tree, &result);
	Icon->speed = R2N(&result);
	if (Icon->speed < 10)
	    Icon->speed = 10;
	DelResult(&result);
    }

    Icon->visible = 1;
    if (Icon->visible_tree != NULL) {
	Eval(Icon->visible_tree, &result);
	Icon->visible = R2N(&result);
	if (Icon->visible < 1)
	    Icon->visible = 0;
	DelResult(&result);
    }

    /* rotate icon bitmap */
    Icon->curmap++;
    if (Icon->curmap >= Icon->maxmap)
	Icon->curmap = 0;

    /* finally, draw it! */
    if (W->class->draw)
	W->class->draw(W);

    /* add a new one-shot timer */
    timer_add(widget_icon_update, Self, Icon->speed, 1);

}



int widget_icon_init(WIDGET * Self)
{
    char *section;
    WIDGET_ICON *Icon;

    /* prepare config section */
    /* strlen("Widget:")=7 */
    section = malloc(strlen(Self->name) + 8);
    strcpy(section, "Widget:");
    strcat(section, Self->name);

    Icon = malloc(sizeof(WIDGET_ICON));
    memset(Icon, 0, sizeof(WIDGET_ICON));

    /* get raw expressions (we evaluate them ourselves) */
    Icon->speed_expr = cfg_get_raw(section, "speed", NULL);
    Icon->visible_expr = cfg_get_raw(section, "visible", NULL);

    /* compile'em */
    Compile(Icon->speed_expr, &Icon->speed_tree);
    Compile(Icon->visible_expr, &Icon->visible_tree);

    /* sanity check */
    if (Icon->speed_expr == NULL || *Icon->speed_expr == '\0') {
	error("Icon %s has no speed, using '100'", Self->name);
	Icon->speed_expr = "100";
    }

    /* read bitmap */
    widget_icon_read_bitmap(section, Icon);

    free(section);
    Self->data = Icon;

    /* as the speed is evaluatod on every call, we use 'one-shot'-timers.  */
    /* The timer will be reactivated on every call to widget_icon_update().  */
    /* We do the initial call here... */
    Icon->prvmap = -1;

    /* reset ascii  */
    Icon->ascii = -1;

    /* just do it! */
    widget_icon_update(Self);

    return 0;
}


int widget_icon_quit(WIDGET * Self)
{
    if (Self) {
	if (Self->data) {
	    WIDGET_ICON *Icon = Self->data;
	    DelTree(Icon->speed_tree);
	    DelTree(Icon->visible_tree);
	    if (Icon->bitmap)
		free(Icon->bitmap);
	    free(Self->data);
	    Self->data = NULL;
	}
    }

    return 0;

}



WIDGET_CLASS Widget_Icon = {
  name:"icon",
  init:widget_icon_init,
  draw:NULL,
  quit:widget_icon_quit,
};
