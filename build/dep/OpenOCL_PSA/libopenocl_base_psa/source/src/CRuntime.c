/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/Stack.h"

#include <stdlib.h>
#include <string.h>

/*
 *	Conditionally defined in "openocl/base/psa/debug.h"
 */

static long long allocated = 0;

void*	CRuntime_malloc( size_t size )
{
	allocated++;
	return malloc( size );
}

void*	CRuntime_calloc( size_t nmemb, size_t size )
{
	allocated++;
	return calloc( nmemb, size );
}

void*	CRuntime_realloc( void* ptr, size_t size )
{
	return realloc( ptr, size );
}

void*	CRuntime_recalloc( void* ptr, size_t oldSize, size_t newSize )
{
	void** new_mem = (void**) CRuntime_calloc( newSize, sizeof( void* ) );
	void** old_mem = (void**) ptr;
	
	if ( new_mem )
	{
		unsigned int i;
		for ( i=0; i < oldSize; i++ )
		{
			new_mem[i] = old_mem[i];
		}
		CRuntime_free( ptr );
	}
	return (void*) new_mem;
}

void*	CRuntime_recalloc_type( void* ptr, size_t oldSize, size_t newSize, size_t type )
{
	char** new_mem = (char**) CRuntime_calloc( newSize, type );
	char** old_mem = (char**) ptr;
	
	if ( new_mem )
	{
		unsigned int max = oldSize * type;
		unsigned int i;
		for ( i=0; i < max; i++ )
		{
			new_mem[i] = old_mem[i];
		}
		CRuntime_free( ptr );
	}
	return (void*) new_mem;
}

void	CRuntime_free( void* ptr )
{
	if ( ptr )
	{
		allocated--;
		free( ptr );
	}
}

void CRuntime_AlterAllocationCount( int delta )
{
	abort();
	//allocated += delta;
}

long long CRuntime_GetAllocationCount()
{
	return (int) allocated;
}

