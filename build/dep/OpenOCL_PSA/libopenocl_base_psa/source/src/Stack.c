/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/Stack.h"
#include "openocl/base/psa/CharString.h"

#include <stdlib.h>
#include <string.h>

struct _IStack
{
	char** elements;
	unsigned int size;
	unsigned int count;
};

IStack* new_Stack()
{
	IStack* self = CRuntime_calloc( 1, sizeof( IStack ) );
	self->elements = CRuntime_calloc( DEFAULT_STACK_SIZE  + 1, sizeof( char* ) );
	self->size = DEFAULT_STACK_SIZE;
	self->count = 0;
	return self;
}

void free_Stack( IStack* self )
{
	char** tmp = self->elements;
	while ( NULL != *tmp )
	{
		CRuntime_free( *tmp );
		tmp++;
	}
	CRuntime_free( self->elements );
	CRuntime_free( self );
}

void Stack_addItem( IStack* self, char* item )
{
	if ( self->count == self->size )
	{
		Stack_expand( self );
	}
	self->elements[self->count] = item;
	self->count++;
}

void Stack_copyItem( IStack* self, const char* item )
{
	if ( self->count == self->size )
	{
		Stack_expand( self );
	}
	self->elements[self->count] = CharString_copy( item );
	self->count++;
}

char* Stack_pop( IStack* self )
{
	char* element = NULL;
	if ( 0 < self->count )
	{
		self->count--;
		element = self->elements[self->count];
		self->elements[self->count] = 0;
	}
	return element;
}

void Stack_expand( IStack* self )
{
	unsigned int new_size = (self->size * 2) + 1;

	self->elements = CRuntime_recalloc( (void**) self->elements, self->size, new_size );
	self->size = new_size;
}

/*
void Stack_expand( IStack* self )
{
	unsigned int new_size = self->size * 2;
	self->elements = CRuntime_realloc( self->elements, new_size + 1 );
	if ( self->elements )
	{
		memset( &self->elements[self->count], 0, self->size + 1 ); 
		self->size = new_size;
	}
}
*/
const char** Stack_getAsCharStringList( const IStack* self )
{
	return (const char**) self->elements;
}

