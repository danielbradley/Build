/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Set.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _ISet
{
	void** elements;
	unsigned int count;
	unsigned int size;
};

//  Internal functions

static int Set_expand( ISet* self );

ISet* new_Set()
{
	ISet* self = (ISet*) CRuntime_calloc( 1, sizeof( ISet ) );
	self->count = 0;
	self->size = 10;

	self->elements = (void**) CRuntime_calloc( 10, sizeof( void* ) );
	return self;
}

void
free_Set( ISet* self )
{
	CRuntime_free( self->elements );
	CRuntime_free( self );
} 

void
Set_add( ISet* self, const void* anElement )
{
	if ( self->count == self->size - 1 )
	{
		Set_expand( self );
	}

	self->elements[ self->count ] = (void*) anElement;
	self->count++;
}

const void*
Set_get( const ISet* self, unsigned int index )
{
	return self->elements[index];
} 

int
Set_contains( const ISet* self, const void* anElement )
{
	int result = 0;

	unsigned int i;
	for ( i=0; i < self->count; i++ )
	{
		if ( anElement == self->elements[i] )
		{
			result = 1;
			break;
		}
	}
	return result;
}

unsigned int
Set_count( const ISet* self )
{
	return self->count;
}

void
Set_union( ISet* self, const ISet* aSet )
{
	unsigned int count = Set_count( aSet );
	unsigned int i;
	for ( i=0; i < count; i++ )
	{
		Set_add( self, aSet->elements[i] );
	}
}

int
Set_expand( ISet* self )
{
	int status = 1;

	unsigned int new_size = self->size * 2;
	void** new_elements = (void**) CRuntime_realloc( self->elements, new_size * sizeof(void*) );
	if ( NULL != new_elements )
	{
		self->elements = new_elements;
		self->size = new_size;
		{
			unsigned int i;
			for ( i = self->count; i < self->size; i++ )
			{
				self->elements[i] = NULL;
			}
		}
		status = 0;
	}	
	return status;
}
