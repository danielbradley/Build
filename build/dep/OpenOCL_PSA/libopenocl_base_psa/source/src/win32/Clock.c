/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#define _ALL_SOURCE 1
//#include <unistd.h>
#include <stdlib.h>
#include <time.h>		/** time()    **/
#include <sys/types.h>
#include <sys/timeb.h>
#include "openocl/base/psa/Clock.h"

//#include <windows.h>
//#include <sys/times.h>		/** struct tms **/

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
	//__time64_t ltime;
	//_time64( &ltime );
	//return ltime;
	return 0;
}

void
Clock_start( IClock* self )
{
	self->start = Clock_GetMicroSecondsSinceEpoch();
}

void
Clock_stop( IClock* self )
{
	self->stop = Clock_GetMicroSecondsSinceEpoch();
	self->interval = self->stop - self->start;
}

unsigned long long
Clock_getInterval( const IClock* self )
{
	unsigned long long ret = 0;
	unsigned long long now = Clock_GetMicroSecondsSinceEpoch();
	if ( self->interval )
	{
		ret = self->interval;
	} else {
		ret = now - self->start;
	}
	return ret;
}

/*
 *  Class Methods
 */

unsigned long long
Clock_GetMicroSecondsSinceEpoch()
{
	return 0;
}

unsigned long long
Clock_GetSecondsSinceEpoch()
{
	return 0;
}

unsigned long Clock_GetClockCyclesUsed()
{
	return clock();
}

unsigned long Clock_GetUserTime()
{
	return 0;
}

unsigned long Clock_GetSystemTime()
{
	return 0;
}

unsigned long Clock_GetUptime()
{
	return 0;
}
