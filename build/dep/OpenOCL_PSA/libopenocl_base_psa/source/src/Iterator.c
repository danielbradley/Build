/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/Bag.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Iterator.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _IIterator
{
	IBag* elements;
	unsigned int posn;
	unsigned int size;
};

IIterator*
new_Iterator()
{
	IIterator* self = (IIterator*) CRuntime_calloc( 1, sizeof( IIterator ) );
	self->elements = new_Bag();
	self->posn = 0;
	self->size = 0;
	return self;
}

void
free_Iterator( IIterator* self )
{
	free_Bag( self->elements );
	CRuntime_free( self );
}

void
Iterator_addReference( IIterator* self, const void* element )
{
	Bag_add( self->elements, element );
	self->size++;
}

const void*
Iterator_next( IIterator* self )
{
	const void* obj = NULL;
	if ( self->posn < self->size )
	{
		obj = Bag_get( self->elements, self->posn );
		self->posn++;
	}
	return obj;
}

bool
Iterator_hasNext( const IIterator* self )
{
	return (self->posn < self->size);
}
