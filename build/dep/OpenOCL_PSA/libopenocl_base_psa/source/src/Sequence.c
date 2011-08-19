/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Sequence.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _ISequence
{
	void** objects;
	unsigned int count;
	unsigned int size;
};

//  Internal functions

static int  Sequence_expand( ISequence*const self );
static void Sequence_shuffleUp( ISequence*const self, unsigned int index );
static void Sequence_shuffleDown( ISequence*const self, unsigned int index );

ISequence* new_Sequence()
{
	ISequence* self = (ISequence*) CRuntime_calloc( 1, sizeof( ISequence ) );
	self->count = 0;
	self->size = 10;

	self->objects = (void**) CRuntime_calloc( 10, sizeof( void* ) );
	return self;
}

void
free_Sequence( ISequence* self )
{
	CRuntime_free( self->objects );
	CRuntime_free( self );
} 

void
Sequence_add( ISequence*const self, const void* anElement )
{
	if ( self->count == self->size - 1 )
	{
		Sequence_expand( self );
	}

	self->objects[ self->count ] = (void*) anElement;
	self->count++;
}

void
Sequence_insert( ISequence*const self, const void* anElement, unsigned int index )
{
	if ( self->count == self->size - 1 )
	{
		Sequence_expand( self );
	}

	if ( index >= self->count )
	{
		self->objects[ self->count ] = (void*) anElement;
		self->count++;
	}
	else
	{
		Sequence_shuffleUp( self, index );
		self->objects[index] = (void*) anElement;
		self->count++;
	}
}

const void*
Sequence_remove_element( ISequence*const self, const void* anElement )
{
	const void* obj = NULL;
	int i;
	for ( i=0; i < self->count; i++ )
	{
		if ( anElement == self->objects[i] )
		{
			obj = Sequence_remove_from_index( self, i );
			break;
		}
	}
	return obj;
}

const void*
Sequence_remove_from_index( ISequence*const self, unsigned int index )
{
	const void* obj = Sequence_get( self, index );
	if ( obj )
	{
		Sequence_shuffleDown( self, index );
	}
	return obj;
}

const void*
Sequence_get( const ISequence* self, unsigned int index )
{
	return self->objects[index];
} 

int
Sequence_contains( const ISequence* self, const void* anElement )
{
	int result = 0;

	unsigned int i;
	for ( i=0; i < self->count; i++ )
	{
		if ( anElement == self->objects[i] )
		{
			result = 1;
			break;
		}
	}
	return result;
}

unsigned int
Sequence_count( const ISequence* self )
{
	return self->count;
}

int
Sequence_expand( ISequence*const self )
{
	int status = 1;

	unsigned int new_size = self->size * 2;
	void** new_objects = (void**) CRuntime_realloc( self->objects, new_size * sizeof(void*) );
	if ( NULL != new_objects )
	{
		self->objects = new_objects;
		self->size = new_size;
		{
			unsigned int i;
			for ( i = self->count; i < self->size; i++ )
			{
				self->objects[i] = NULL;
			}
		}
		status = 0;
	}	
	return status;
}

void
Sequence_shuffleUp( ISequence*const self, unsigned int index )
{
	int u_index = index;
	int i;
	if ( self->count == (self->size - 1) )
	{
		Sequence_expand( self );
	}

	for( i=self->count; i > u_index; i-- )
	{
		self->objects[i] = self->objects[i-1];
	}
	self->objects[u_index] = NULL;
}

void
Sequence_shuffleDown( ISequence*const self, unsigned int index )
{
	unsigned int i;
	self->count--;
	for ( i=index; i < self->count; i++ )
	{
		self->objects[i] = self->objects[i+1];
	}
	self->objects[i+1] = NULL;
}
