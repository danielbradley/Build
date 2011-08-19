/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa.h"
#include "openocl/threads/psa/Mutex.h"
#include "openocl/threads/psa/Mutex.protected.h"

#include <pthread.h>
#include <errno.h>

struct _IMutex
{
	pthread_mutex_t     state;
	pthread_mutexattr_t attributes;
	bool                valid;
};

IMutex* new_Mutex()
{
	IMutex* self = CRuntime_calloc( 1, sizeof( IMutex ) );
	self->valid = !pthread_mutex_init( &self->state, &self->attributes );
	return self;
}

void free_Mutex( IMutex* self )
{
	CRuntime_free( self );
}

bool Mutex_lock( IMutex* self )
{
	return !pthread_mutex_lock( &self->state );
}

bool Mutex_tryLock( IMutex* self )
{
	bool success = 1;
	if ( EBUSY == pthread_mutex_trylock( &self->state ) )
	{
		success = 0;
	}
	return success;
}

bool Mutex_unlock( IMutex* self )
{
	return !pthread_mutex_unlock( &self->state );
}

bool Mutex_isValid( IMutex* self )
{
	return self->valid;
}

void* Mutex_getState( IMutex* self )
{
	return &self->state;
}
