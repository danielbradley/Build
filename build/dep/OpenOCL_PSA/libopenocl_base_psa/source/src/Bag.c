/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Bag.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _IBag
{
	void** objects;
	unsigned int count;
	unsigned int size;
};

//  Internal functions

static int Bag_expand( IBag*const self );

IBag* new_Bag()
{
	IBag* self = (IBag*) CRuntime_calloc( 1, sizeof( IBag ) );
	self->count = 0;
	self->size = 10;

	self->objects = (void**) CRuntime_calloc( 10, sizeof( void* ) );
	return self;
}

void
free_Bag( IBag* self )
{
	CRuntime_free( self->objects );
	CRuntime_free( self );
} 

void
Bag_add( IBag*const self, const void* anElement )
{
	if ( self->count == self->size - 1 )
	{
		Bag_expand( self );
	}

	self->objects[ self->count ] = (void*) anElement;
	self->count++;
}

const void*
Bag_get( const IBag* self, unsigned int index )
{
	return self->objects[index];
} 

int
Bag_contains( const IBag* self, const void* anElement )
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
Bag_count( const IBag* self )
{
	return self->count;
}

int
Bag_expand( IBag*const self )
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
