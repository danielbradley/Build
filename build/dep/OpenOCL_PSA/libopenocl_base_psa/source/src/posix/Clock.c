/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/Clock.h"

#include <stdlib.h>
#include <time.h>		/** times()    **/
#include <sys/times.h>		/** struct tms **/
#include <sys/time.h>		/** struct timeval timezone **/

struct _Clock
{
	unsigned long start;
	unsigned long stop;
	unsigned long long interval;
};

IClock* new_Clock()
{
	IClock* self = CRuntime_calloc( 1, sizeof( IClock ) );
	return self;
}

void free_Clock( IClock* self )
{
	CRuntime_free( self );
}

/*
 *  Class Methods
 */
int Clock_GetTimeAsInt()
{
	return time( NULL );
}

unsigned long long
Clock_GetMicroSecondsSinceEpoch()
{
	unsigned long seconds = 0;
	struct timeval tv;
	struct timezone tz;
	
	if ( 0 == gettimeofday( &tv, &tz ) )
	{
		seconds = tv.tv_sec * 1000;
		seconds += tv.tv_usec;
	}
	return seconds;
}

unsigned long long
Clock_GetSecondsSinceEpoch()
{
	unsigned long seconds = 0;
	struct timeval tv;
	struct timezone tz;
	
	if ( 0 == gettimeofday( &tv, &tz ) )
	{
		seconds = tv.tv_sec;
	}
	return seconds;
}

unsigned long Clock_GetClockCyclesUsed()
{
	return clock();
}

unsigned long Clock_GetUserTime()
{
	struct tms time;
	times( &time );
	return time.tms_utime;
}

unsigned long Clock_GetSystemTime()
{
	struct tms time;
	times( &time );
	return time.tms_stime;
}

unsigned long Clock_GetUptime()
{
	struct tms time;
	return times( &time );
}
