/******************************************************************************
 *  bwm-ng parsing and retrive stuff                                          *
 *                                                                            *
 *  Copyright (C) 2004-2007 Volker Gropp (bwmng@gropp.org)                    *
 *                                                                            *
 *  for more info read README.                                                *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the Free Software               *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA *
 *                                                                            *
 *****************************************************************************/

#ifndef __GETIFADDRS_H
#define __GETIFADDRS_H 

#include "retrieve.h"

#ifdef GETIFADDRS
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>

void get_iface_stats_getifaddrs (char verbose);

#endif

#endif
