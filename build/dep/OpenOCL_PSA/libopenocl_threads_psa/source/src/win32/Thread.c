/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include <windows.h>
#include "openocl/base/psa.h"
#include "openocl/threads/psa/ConditionVariable.h"
#include "openocl/threads/psa/Thread.h"
#include <stdio.h>

struct _IThread
{
	void*(*callback)(void*);
	void*                 arg;
	HANDLE                state;
	DWORD                 threadId;
	LPSECURITY_ATTRIBUTES attributes;
	DWORD                 ret;
};

void* Thread_wrapper( void* self );

IThread* new_Thread( void*(*startRoutine)(void*), void* argument )
{
	IThread* self = CRuntime_calloc( 1, sizeof( IThread ) );
	self->callback = startRoutine;
	self->arg = argument;
	return self;
}

void free_Thread( IThread* self )
{
	CRuntime_free( self );
}

bool Thread_run( IThread* self )
{
	bool success = 1;

	LPTHREAD_START_ROUTINE wrapper = (LPTHREAD_START_ROUTINE) Thread_wrapper;

	if ( NULL == (self->state = CreateThread( NULL, 0, wrapper, self, 0, &self->threadId )) )
	{
		success = 0;
	}
	//else
	//{
	//	SetThreadPriority( self->state, THREAD_PRIORITY_IDLE );
	//}
	return success;
	//return !pthread_create( &self->state, &self->attributes, self->callback, self->arg );
}

bool Thread_wait( IThread* self )
{
	bool success = 0;

	bool loop = 0;
	do
	{
		bool success = GetExitCodeThread( self->state, &self->ret );
		if ( success )
		{
			loop = (STILL_ACTIVE == self->ret);
		}
	} while ( loop );

	//while ( (success = GetExitCodeThread( self->state, &self->ret )) && (STILL_ACTIVE == self->ret) );
	return success;
}

void*
Thread_wrapper( void* self )
{
	IThread* ithread = (IThread*) self;
	LPTHREAD_START_ROUTINE callback = (LPTHREAD_START_ROUTINE) ithread->callback;

	ithread->ret = (DWORD) ithread->callback( ithread->arg );
	ExitThread( ithread->ret );
}
