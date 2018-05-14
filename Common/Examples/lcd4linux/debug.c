/* $Id: debug.c,v 1.13 2005/05/08 04:32:43 reinelt Exp $
 *
 * debug() and error() functions
 *
 * Copyright (C) 1999, 2000 Michael Reinelt <reinelt@eunet.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * This file is part of LCD4Linux.
 *
 * LCD4Linux is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * LCD4Linux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * $Log: debug.c,v $
 * Revision 1.13  2005/05/08 04:32:43  reinelt
 * CodingStyle added and applied
 *
 * Revision 1.12  2005/01/18 06:30:22  reinelt
 * added (C) to all copyright statements
 *
 * Revision 1.11  2004/06/26 12:04:59  reinelt
 *
 * uh-oh... the last CVS log message messed up things a lot...
 *
 * Revision 1.10  2004/06/26 09:27:20  reinelt
 *
 * added '-W' to CFLAGS
 * changed all C++ comments to C ones
 * cleaned up a lot of signed/unsigned mistakes
 *
 * Revision 1.9  2004/06/20 10:09:54  reinelt
 *
 * 'const'ified the whole source
 *
 * Revision 1.8  2004/05/26 11:37:36  reinelt
 *
 * Curses driver ported.
 *
 * Revision 1.7  2004/02/10 07:42:35  reinelt
 * cut off all old-style files which are no longer used with NextGeneration
 *
 * Revision 1.6  2003/10/05 17:58:50  reinelt
 * libtool junk; copyright messages cleaned up
 *
 * Revision 1.5  2003/08/24 05:17:58  reinelt
 * liblcd4linux patch from Patrick Schemitz
 *
 * Revision 1.4  2003/08/08 06:58:06  reinelt
 * improved forking
 *
 * Revision 1.3  2001/03/12 12:39:36  reinelt
 *
 * reworked autoconf a lot: drivers may be excluded, #define's went to config.h
 *
 * Revision 1.2  2001/03/09 13:08:11  ltoetsch
 * Added Text driver
 *
 * Revision 1.1  2000/11/28 20:20:38  reinelt
 *
 * added debug.c
 * things like that should not hapen. debug.c exists for a few months now, but was never added to CVS. Shit happens....
 *
 */

/* 
 * exported functions:
 *
 * message (level, format, ...)
 *   passes the arguments to vsprintf() and
 *   writes the resulting string either to stdout
 *   or syslog.
 *   this function should not be called directly,
 *   but the macros info(), debug() and error()
 *
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include "debug.h"

int running_foreground = 0;
int running_background = 0;

int verbose_level = 0;

void message(const int level, const char *format, ...)
{
    va_list ap;
    char buffer[256];
    static int log_open = 0;

    if (level > verbose_level)
	return;

    va_start(ap, format);
    vsnprintf(buffer, sizeof(buffer), format, ap);
    va_end(ap);

    if (!running_background) {

#ifdef WITH_CURSES
	extern int curses_error(char *);
	if (!curses_error(buffer))
#endif
	    fprintf(level ? stdout : stderr, "%s\n", buffer);
    }

    if (running_foreground)
	return;

    if (!log_open) {
	openlog("LCD4Linux", LOG_PID, LOG_USER);
	log_open = 1;
    }

    switch (level) {
    case 0:
	syslog(LOG_ERR, "%s", buffer);
	break;
    case 1:
	syslog(LOG_INFO, "%s", buffer);
	break;
    default:
	syslog(LOG_DEBUG, "%s", buffer);
    }
}
