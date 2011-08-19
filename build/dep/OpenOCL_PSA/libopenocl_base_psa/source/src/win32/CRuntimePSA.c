/*
 *  Copyright (C) 2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include "openocl/base/psa/CRuntime.h"

void CRuntime_SleepFor( unsigned int seconds )
{
	unsigned int milliseconds = seconds * 1000;
	Sleep( milliseconds );
} 

void CRuntime_MicroSleepFor( unsigned int microseconds )
{
	unsigned int milliseconds = microseconds / 1000;
	Sleep( milliseconds );
} 
