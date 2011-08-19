/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include "openocl/threads/psa/ConditionVariable.h"
#include "openocl/threads/psa/Mutex.h"
#include "openocl/threads/psa/Mutex.protected.h"
//#include "openocl/threads/psa/win32/pthread.h"
#include <openocl/base/psa.h>

//#include <sys/time.h>

#define SIGNAL     0
#define BROADCAST  1
#define MAX_EVENTS 2

struct _IConditionVariable
{
	unsigned int waiters;
	IMutex* waiters_lock;
	HANDLE events[MAX_EVENTS];
	bool                valid;
};

IConditionVariable* new_ConditionVariable()
{
	IConditionVariable* self = CRuntime_calloc( 1, sizeof( IConditionVariable ) );
	self->waiters = 0;
	self->waiters_lock = new_Mutex();
	self->events[SIGNAL] = CreateEvent( NULL, FALSE, FALSE, NULL );
	self->events[BROADCAST] = CreateEvent( NULL, TRUE, FALSE, NULL );
	self->valid = (bool) (self->events[SIGNAL] && self->events[BROADCAST]);
	return self;
}

void free_ConditionVariable( IConditionVariable* self )
{
	free_Mutex( self->waiters_lock );
	if ( self->events[SIGNAL] )
	{
		CloseHandle( self->events[SIGNAL] );
	}
	if ( self->events[BROADCAST] )
	{
		CloseHandle( self->events[BROADCAST] );
	}
	CRuntime_free( self );
}

bool
ConditionVariable_wait( IConditionVariable* self, IMutex* externalMutex )
{
	int result;
	int last_waiter;

	Mutex_lock( self->waiters_lock );
	self->waiters++;
	Mutex_unlock( self->waiters_lock );

	Mutex_unlock( externalMutex );

	result = WaitForMultipleObjects( 2, self->events, FALSE, INFINITE );

	Mutex_lock( self->waiters_lock );
	self->waiters--;
	last_waiter = (result == WAIT_OBJECT_0 + BROADCAST) && (self->waiters == 0);
	Mutex_unlock( self->waiters_lock );

	//	Some thread called <pthread_cond_broadcast>.
	if ( last_waiter )
	{
		//	We're the last waiter to be notified or to stop waiting, so
		//	reset the manual event.
		ResetEvent( self->events[BROADCAST] );
	}

	//	Reacquire the <external_mutex>.
	Mutex_lock( externalMutex );

	return 1;
}

bool
ConditionVariable_timedWait( IConditionVariable* self, IMutex* externalMutex, int seconds )
{
	int result;
	int last_waiter;

	Mutex_lock( self->waiters_lock );
	self->waiters++;
	Mutex_unlock( self->waiters_lock );

	Mutex_unlock( externalMutex );

	result = WaitForMultipleObjects( 2, self->events, FALSE, seconds );

	Mutex_lock( self->waiters_lock );
	self->waiters--;
	last_waiter = (result == WAIT_OBJECT_0 + BROADCAST) && (self->waiters == 0);
	Mutex_unlock( self->waiters_lock );

	//	Some thread called <pthread_cond_broadcast>.
	if ( last_waiter )
	{
		//	We're the last waiter to be notified or to stop waiting, so
		//	reset the manual event.
		ResetEvent( self->events[BROADCAST] );
	}

	//	Reacquire the <external_mutex>.
	Mutex_lock( externalMutex );

	if ( result < 0 )
	{
		result = 0;
	}

	return result;
}

bool
ConditionVariable_signal( IConditionVariable* self )
{
	bool have_waiters;

	//	Avoid race conditions.
	Mutex_lock( self->waiters_lock );
	have_waiters = (self->waiters > 0);
	Mutex_unlock( self->waiters_lock );

	if ( have_waiters )
	{
		SetEvent( self->events[SIGNAL] );
	}
	return 1;
}

bool
ConditionVariable_broadcast( IConditionVariable* self )
{
	bool have_waiters;

	//	Avoid race conditions.
	Mutex_lock( self->waiters_lock );
	have_waiters = (self->waiters > 0);
	Mutex_unlock( self->waiters_lock );

	if ( have_waiters )
	{
		SetEvent( self->events[BROADCAST] );
	}

	return 1;
}

/*
bool ConditionVariable_timedWait( IConditionVariable* self, IMutex* aMutex, int seconds )
{
	/
	struct timeval now;
	struct timespec timeout;
	struct timezone* tz = NULL;
	gettimeofday( &now, tz );
	
	timeout.tv_sec = now.tv_sec + seconds;
	timeout.tv_nsec = now.tv_usec * 1000;
	
	pthread_mutex_t* mutex_state = (pthread_mutex_t*) Mutex_getState( aMutex );
	return !pthread_cond_timedwait( &self->state, mutex_state, &timeout );
	/
}
*/
