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
#include "openocl/threads/psa/Mutex.h"
#include "openocl/threads/psa/Mutex.protected.h"

struct _IMutex
{
	HANDLE              handle;	
	bool                valid;
};

IMutex* new_Mutex()
{
	IMutex* self = CRuntime_calloc( 1, sizeof( IMutex ) );
	self->handle = CreateMutex( NULL, 0, NULL );
	self->valid = (bool) self->handle;
	return self;
}

void free_Mutex( IMutex* self )
{
	CloseHandle( self->handle );
	CRuntime_free( self );
}

bool Mutex_lock( IMutex* self )
{
	return (WAIT_FAILED != WaitForSingleObject( self->handle, INFINITE ));
}

bool Mutex_tryLock( IMutex* self )
{
	return (WAIT_FAILED != WaitForSingleObject( self->handle, 100 ));
}

bool Mutex_unlock( IMutex* self )
{
	return ReleaseMutex( self->handle );
}

bool Mutex_isValid( IMutex* self )
{
	return self->valid;
}

void* Mutex_getState( IMutex* self )
{
	return &self->handle;
}
