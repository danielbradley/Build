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
#include "openocl/base/psa/Sort.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct _ISort
{
	ISequence* ordered;
	unsigned int count;
};

typedef struct _Tuple Tuple;

struct _Tuple
{
	const char* key;
	const void* value;
};

ISort* new_Sort()
{
	ISort* self = (ISort*) CRuntime_calloc( 1, sizeof( ISort ) );
	self->count = 0;
	self->ordered = new_Sequence();
	
	return self;
}

void
free_Sort( ISort* self )
{
	int i;
	int count = Sequence_count( self->ordered );
	for ( i=0; i < count; i++ )
	{
		Tuple* t = (Tuple*) Sequence_get( self->ordered, i );
		CRuntime_free( t );
	}

	free_Sequence( self->ordered );
	CRuntime_free( self );
}

void
Sort_insert( ISort* self, const char* aKey, const void* aValue )
{
	bool inserted = 0;
	Tuple* tuple = CRuntime_calloc( 1, sizeof( Tuple ) );
	tuple->key = aKey;
	tuple->value = aValue;

	{
		int i;
		int count = self->count;	
		for ( i=0; i < count; i++ )
		{
			const Tuple* existing = Sequence_get( self->ordered, i );
			if ( 0 < CharString_compare( existing->key, aKey ) )
			{
				Sequence_insert( self->ordered, tuple, i );
				inserted = 1;
				break;
			}
		}
		if ( !inserted )
		{
			Sequence_insert( self->ordered, tuple, i );
		}
	}
	self->count++;
}

unsigned int
Sort_count( const ISort* self )
{
	return Sequence_count( self->ordered );
}

IIterator*
Sort_iterator_keys( const ISort* self )
{
	IIterator* it = new_Iterator();

	int i;
	int count = Sequence_count( self->ordered );
	for ( i=0; i < count; i++ )
	{
		Tuple* tuple = (Tuple*) Sequence_get( self->ordered, i );
		Iterator_addReference( it, tuple->key );
	}
	
	return it;
}

IIterator*
Sort_iterator_values( const ISort* self )
{
	IIterator* it = new_Iterator();

	int i;
	int count = Sequence_count( self->ordered );
	for ( i=0; i < count; i++ )
	{
		Tuple* tuple = (Tuple*) Sequence_get( self->ordered, i );
		Iterator_addReference( it, tuple->value );
	}
	
	return it;
}

