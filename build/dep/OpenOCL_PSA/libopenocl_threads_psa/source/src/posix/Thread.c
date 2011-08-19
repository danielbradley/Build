/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa.h"
#include "openocl/threads/psa/Thread.h"

#include <pthread.h>

struct _IThread
{
	void*(*callback)(void*);
	void* arg;
	pthread_t      state;
	pthread_attr_t attributes;
	void*          ret;
};

IThread* new_Thread( void*(*startRoutine)(void*), void* argument )
{
	IThread* self = CRuntime_calloc( 1, sizeof( IThread ) );
	self->callback = startRoutine;
	self->arg = argument;
	pthread_attr_init( &self->attributes );
	return self;
}

void free_Thread( IThread* self )
{
	CRuntime_free( self );
}

bool Thread_run( IThread* self )
{
	return !pthread_create( &self->state, &self->attributes, self->callback, self->arg );
}

bool Thread_wait( IThread* self )
{
	return !pthread_join( self->state, &self->ret );
}

