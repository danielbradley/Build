/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa.h"
#include "openocl/threads/psa/ConditionVariable.h"
#include "openocl/threads/psa/Mutex.h"
#include "openocl/threads/psa/Semaphore.h"

struct _ISemaphore
{
	int value;
	IMutex* mutex;
	IConditionVariable* cv;
};

ISemaphore* new_Semaphore()
{
	ISemaphore* self = CRuntime_calloc( 1, sizeof( ISemaphore ) );
	self->mutex = new_Mutex();
	self->cv = new_ConditionVariable();
	self->value = 0;
	return self;
}

ISemaphore* new_Semaphore_value( int aValue )
{
	ISemaphore* self = CRuntime_calloc( 1, sizeof( ISemaphore ) );
	self->mutex = new_Mutex();
	self->cv = new_ConditionVariable();
	self->value = aValue;
	return self;
}

void free_Semaphore( ISemaphore* self )
{
	free_Mutex( self->mutex );
	free_ConditionVariable( self->cv );
	CRuntime_free( self );
}

void Semaphore_wait( ISemaphore* self )
{
	Mutex_lock( self->mutex );
	{
		while ( self->value <= 0 ) {
			ConditionVariable_wait( self->cv, self->mutex );
		}
		self->value -= 1;
	}
	Mutex_unlock( self->mutex );
}

void Semaphore_signal( ISemaphore* self )
{
	Mutex_lock( self->mutex );
	{
		self->value += 1;
		if ( 0 < self->value ) ConditionVariable_signal( self->cv );
	}
	Mutex_unlock( self->mutex );
}
