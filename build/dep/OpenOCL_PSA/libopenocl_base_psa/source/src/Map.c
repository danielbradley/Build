/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/Map.protected.h"

void free_Map( IMap* self )
{
	self->free( self );
}

//------------------------------------------------------------------
//  functions
//------------------------------------------------------------------

void
Map_freeObjects( IMap* self, void(*freeObject)(void*) )
{
	self->freeObjects( self, freeObject );
}

void*
Map_put( IMap* self, const char* key, const void* object )
{
	return self->put( self, key, object );
}

bool
Map_putIfNew( IMap* self, const char* key, const void* object )
{
	return self->putIfNew( self, key, object );
}

void*
Map_remove_key( IMap* self, const char* key )
{
	return self->removeKey( self, key );
}

void*
Map_remove_value( IMap* self, const void* value )
{
	return self->removeValue( self, value );
}

//------------------------------------------------------------------
//  constant functions
//------------------------------------------------------------------

const void*
Map_get( const IMap* self, const char* key )
{
	return self->get( self, key );
}

bool
Map_containsKey( const IMap* self, const char* key )
{
	return self->containsKey( self, key );
}

bool
Map_containsValue( const IMap* self, const void* value )
{
	return self->containsValue( self, value );
}

unsigned int
Map_getSize( const IMap* self )
{
	return self->getSize( self );
}

IIterator*
Map_iterator_keys( const IMap* self )
{
	return self->iterator_keys( self );
}

IIterator*
Map_iterator_values( const IMap* self )
{
	return self->iterator_values( self );
}

IIterator*
Map_iterator_alphabetical_keys( const IMap* self )
{
	return self->iterator_alphabetical_keys( self );
}

IIterator*
Map_iterator_alphabetical_values( const IMap* self )
{
	return self->iterator_alphabetical_values( self );
}
