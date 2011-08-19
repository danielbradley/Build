/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/Time.h"

#include <stdlib.h>

#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <time.h>		/** times()    **/

struct _ITime
{
	struct tm time;
	time_t    sinceEpoch;
	char*     textual;
};

ITime* new_Time( unsigned long long secondsSinceEpoch )
{
	ITime* self = CRuntime_calloc( 1, sizeof( ITime ) );
	time_t t = (time_t) secondsSinceEpoch;
	struct tm* static_time = localtime( &t );
	self->time = *static_time;
	self->sinceEpoch = t;
	
	self->textual = CharString_copy( ctime( &t ) );

	//	This gets rid of the annoying newline
	self->textual[CharString_getLength( self->textual ) - 1] = '\0';
	
	return self;
}

void free_Time( ITime* self )
{
	free_CharString( self->textual );
	CRuntime_free( self );
}

ITime* Time_copy( const ITime* self )
{
	return new_Time( Time_getSecondsSinceEpoch( self ) );
}

const char* Time_getTextual( const ITime* self )
{
	return self->textual;
}

unsigned long long Time_getSecondsSinceEpoch( const ITime* self )
{
	return self->sinceEpoch;
}

ITime* Time_Now()
{
	return new_Time( time( NULL ) );
}
