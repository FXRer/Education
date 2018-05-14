/* $Id: qprintf.h,v 1.5 2005/01/18 06:30:23 reinelt Exp $
 *
 * simple but quick snprintf() replacement
 *
 * Copyright (C) 2004 Michael Reinelt <reinelt@eunet.at>
 * Copyright (C) 2004 The LCD4Linux Team <lcd4linux-devel@users.sourceforge.net>
 *
 * derived from a patch from Martin Hejl which is
 * Copyright (C) 2003 Martin Hejl (martin@hejl.de)
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
 * $Log: qprintf.h,v $
 * Revision 1.5  2005/01/18 06:30:23  reinelt
 * added (C) to all copyright statements
 *
 * Revision 1.4  2004/06/26 12:05:00  reinelt
 *
 * uh-oh... the last CVS log message messed up things a lot...
 *
 * Revision 1.3  2004/06/26 09:27:21  reinelt
 *
 * added '-W' to CFLAGS
 * changed all C++ comments to C ones * cleaned up a lot of signed/unsigned mistakes
 *
 * Revision 1.2  2004/03/03 04:44:16  reinelt
 * changes (cosmetics?) to the big patch from Martin
 * hash patch un-applied
 *
 * Revision 1.1  2004/02/27 07:06:26  reinelt
 * new function 'qprintf()' (simple but quick snprintf() replacement)
 *
 */

#ifndef _QPRINTF_H_
#define _QPRINTF_H_

/* size_t */
#include <stdio.h>

int qprintf(char *str, size_t size, const char *format, ...);

#endif
