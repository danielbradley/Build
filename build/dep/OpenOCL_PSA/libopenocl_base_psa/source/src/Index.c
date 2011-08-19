/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/Index.h"
#include "openocl/base/psa/Map.internal.h"
#include "openocl/base/psa/Hashtable.internal.h"

struct _IIndex
{
	IMap* map;
};

IIndex* new_Index()
{
	return new_Index_hashtable();
}

IIndex* new_Index_hashtable()
{
	IIndex* self = CRuntime_calloc( 1, sizeof( IIndex ) );
	self->map = (IMap*) new_Hashtable();
	return self;
}

void free_Index( IIndex* self )
{
	if ( self )
	{
		free_Map( self->map );
		CRuntime_free( self );
	}
}

void
Index_freeObjects( IIndex* self, void(*freeObject)(void*) )
{
	Map_freeObjects( self->map, freeObject );
}

void*
Index_add( IIndex* self, const char* key, const void* object )
{
	return Map_put( self->map, key, object );
}

bool
Index_addIfNew( IIndex* self, const char* key, const void* object )
{
	return Map_putIfNew( self->map, key, object );
}

void*
Index_remove_key( IIndex* self, const char* key )
{
	return Map_remove_key( self->map, key );
}

void*
Index_remove_value( IIndex* self, const void* value )
{
	return Map_remove_value( self->map, value );
}

const void*
Index_get( const IIndex* self, const char* key )
{
	return Map_get( self->map, key );
}

bool
Index_containsKey( const IIndex* self, const char* key )
{
	return Map_containsKey( self->map, key );
}

bool
Index_containsValue( const IIndex* self, const void* value )
{
	return Map_containsValue( self->map, value );
}

unsigned int
Index_getSize( const IIndex* self )
{
	return Map_getSize( self->map );
}

IIterator*
Index_iterator_keys( const IIndex* self )
{
	return Map_iterator_keys( self->map );
}

IIterator*
Index_iterator_values( const IIndex* self )
{
	return Map_iterator_values( self->map );
}

IIterator*
Index_iterator_alphabetical_keys( const IIndex* self )
{
	return Map_iterator_alphabetical_keys( self->map );
}

IIterator*
Index_iterator_alphabetical_values( const IIndex* self )
{
	return Map_iterator_alphabetical_values( self->map );
}
