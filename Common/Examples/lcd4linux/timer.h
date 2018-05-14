/* $Id: timer.h,v 1.6 2005/05/08 04:32:45 reinelt Exp $
 *
 * generic timer handling
 *
 * Copyright (C) 2003, 2004 Michael Reinelt <reinelt@eunet.at>
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
 * $Log: timer.h,v $
 * Revision 1.6  2005/05/08 04:32:45  reinelt
 * CodingStyle added and applied
 *
 * Revision 1.5  2005/01/18 06:30:24  reinelt
 * added (C) to all copyright statements
 *
 * Revision 1.4  2004/06/20 10:09:56  reinelt
 *
 * 'const'ified the whole source
 *
 * Revision 1.3  2004/06/01 06:45:30  reinelt
 *
 * some Fixme's processed
 * documented some code
 *
 * Revision 1.2  2004/03/03 03:47:04  reinelt
 * big patch from Martin Hejl:
 * - use qprintf() where appropriate
 * - save CPU cycles on gettimeofday()
 * - add quit() functions to free allocated memory
 * - fixed lots of memory leaks
 *
 * Revision 1.1  2004/01/13 08:18:20  reinelt
 * timer queues added
 * liblcd4linux deactivated turing transformation to new layout
 *
 */


#ifndef _TIMER_H_
#define _TIMER_H_

int timer_add(void (*callback) (void *data), void *data, const int interval, const int one_shot);
int timer_process(struct timespec *delay);
void timer_exit();

#endif
