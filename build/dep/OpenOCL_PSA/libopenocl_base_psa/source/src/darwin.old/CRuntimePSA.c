/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"

#include <unistd.h>

void CRuntime_SleepFor( unsigned int seconds )
{
	sleep( seconds );
}

void CRuntime_MicroSleepFor( unsigned int milliseconds )
{
	usleep( milliseconds );
} 
