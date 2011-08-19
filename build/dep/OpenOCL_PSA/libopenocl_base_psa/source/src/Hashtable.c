/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#include "openocl/base/psa/CRuntime.h"
#include "openocl/base/psa/CharString.h"
#include "openocl/base/psa/Hashtable.protected.h"
#include "openocl/base/psa/Node.internal.h"
#include "openocl/base/psa/Sort.h"

IHashtable* new_Hashtable()
{
	IHashtable* self = (IHashtable*) CRuntime_calloc( 1, sizeof( IHashtable ) );
	Hashtable_init( self );
	return self;
}

void free_Hashtable( IHashtable* self )
{
	Hashtable_final( self );
	CRuntime_free( self );
}

void
Hashtable_init( IHashtable* self )
{
	self->super.free            = (void(*)(IMap*)) free_Hashtable;
	self->super.freeObjects     = (void(*)(IMap*,void(*)(void*))) Hashtable_freeObjects;
	self->super.put             = (void*(*)(IMap*,const char*, const void*)) Hashtable_put;
	self->super.putIfNew        = (bool(*)(IMap*,const char*, const void*)) Hashtable_putIfNew;
	self->super.removeKey       = (void*(*)(IMap*,const char*)) Hashtable_remove_key;
	self->super.removeValue     = (void*(*)(IMap*,const void*)) Hashtable_remove_value;
	self->super.get             = (const void*(*)(const IMap*,const char*)) Hashtable_get;
	self->super.containsKey     = (bool(*)(const IMap*,const char*)) Hashtable_containsKey;
	self->super.containsValue   = (bool(*)(const IMap*,const void*)) Hashtable_containsValue;
	self->super.getSize         = (unsigned int(*)(const IMap*)) Hashtable_getSize;
	self->super.iterator_keys   = (IIterator*(*)(const IMap*)) Hashtable_iterator_keys;
	self->super.iterator_values = (IIterator*(*)(const IMap*)) Hashtable_iterator_values;
	self->super.iterator_alphabetical_keys   = (IIterator*(*)(const IMap*)) Hashtable_iterator_alphabetical_keys;
	self->super.iterator_alphabetical_values = (IIterator*(*)(const IMap*)) Hashtable_iterator_alphabetical_values;
	self->free            = free_Hashtable;
	self->freeObjects     = Hashtable_freeObjects;
	self->put             = Hashtable_put;
	self->putIfNew        = Hashtable_putIfNew;
	self->removeKey       = Hashtable_remove_key;
	self->removeValue     = Hashtable_remove_value;
	self->get             = Hashtable_get;
	self->containsKey     = Hashtable_containsKey;
	self->containsValue   = Hashtable_containsValue;
	self->getSize         = Hashtable_getSize;
	self->iterator_keys   = Hashtable_iterator_keys;
	self->iterator_values = Hashtable_iterator_values;
	self->iterator_alphabetical_keys   = Hashtable_iterator_alphabetical_keys;
	self->iterator_alphabetical_values = Hashtable_iterator_alphabetical_values;


	self->entries = CRuntime_calloc( HTABLE_SIZE, sizeof( void* ) );
}

void
Hashtable_final( IHashtable* self )
{
	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		if ( self->entries[i] )
		{
			free_Node( (INode*) self->entries[i] );
			self->entries[i] = NULL;
		}
	}
	CRuntime_free( self->entries );
}

void
Hashtable_freeObjects( IHashtable* self, void(*freeObject)(void*) )
{
	unsigned int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* node = self->entries[i];
		while ( node )
		{
			freeObject( (void*) Node_getObject( node ) );
			node = Node_getSibling( node );
		}
	}
}

void*
Hashtable_put( IHashtable* self, const char* key, const void* object )
{
	void* exists = NULL;
	INode* node;
	int hash = Hashtable_getHashValue( key );

	node = self->entries[hash];
	while ( node )
	{
		if ( 0 == CharString_compare( key, Node_getKey( node ) ) ) {
			exists = (void*) Node_getObject( node );
			Node_setObject( node, object );
			break;
		} else {
			node = (INode*) Node_getSibling( node );
		}
	}
	
	if ( !exists )
	{
		node = new_Node();
		Node_setKeyAndObject( node, key, object );
		Node_setSibling( node, self->entries[hash] );
		self->entries[hash] = node;
		self->size++;
	}

	return exists;
}

bool
Hashtable_putIfNew( IHashtable* self, const char* key, const void* object )
{
	bool exists = 0;
	INode* node;
	int hash = Hashtable_getHashValue( key );

	node = self->entries[hash];
	while ( node )
	{
		if ( 0 == CharString_compare( key, Node_getKey( node ) ) ) {
			exists = 1;
			break;
		} else {
			node = (INode*) Node_getSibling( node );
		}
	}
	
	if ( !exists )
	{
		node = new_Node();
		Node_setKeyAndObject( node, key, object );
		Node_setSibling( node, self->entries[hash] );
		self->entries[hash] = node;
		self->size++;
	}

	return exists;
}

void*
Hashtable_remove_key( IHashtable* self, const char* key )
{
	void* obj = NULL;
	int hash = Hashtable_getHashValue( key );
	
	const INode* previous = NULL;
	const INode* node;
	node = self->entries[hash];

	while ( node )
	{
		if ( 0 == CharString_compare( key, Node_getKey( node ) ) )
		{
			if ( previous )
			{
				Node_setSibling( (INode*) previous, (INode*) Node_getSibling( node ) );
			} else {
				self->entries[hash] = (INode*) Node_getSibling( node );
			}
			obj = (void*) Node_getObject( node );
			Node_setSibling( (INode*) node, (INode*) NULL );
			free_Node( (INode*) node );
			break;
		} else {
			previous = node;
			node = Node_getSibling( node );
		}
	}
	return obj;
}

void*
Hashtable_remove_value( IHashtable* self, const void* value )
{
	void* obj = NULL;
	int i;
	
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* previous = NULL;
		const INode* node;
		node = self->entries[i];

		while ( node )
		{
			if ( value == Node_getObject( node ) )
			{
				if ( previous )
				{
					Node_setSibling( (INode*) previous, (INode*) Node_getSibling( node ) );
				} else {
					self->entries[i] = (INode*) Node_getSibling( node );
				}
				obj = (void*) Node_getObject( node );
				Node_setSibling( (INode*) node, (INode*) NULL );
				free_Node( (INode*) node );
				i = HTABLE_SIZE;
				break;
			} else {
				previous = node;
				node = Node_getSibling( node );
			}
		}
	}
	return obj;
}

const void*
Hashtable_get( const IHashtable* self, const char* key )
{
	const void* obj = NULL;
	const INode* node;
	int hash = Hashtable_getHashValue( key );

	node = self->entries[hash];
	while ( node )
	{
		if ( 0 == CharString_compare( key, Node_getKey( node ) ) ) {
			obj = Node_getObject( node );
			break;
		} else {
			node = Node_getSibling( node );
		}
	}
	return obj;
}

bool
Hashtable_containsKey( const IHashtable* self, const char* key )
{
	bool contains = 0;
	int hash = Hashtable_getHashValue( key );
	
	const INode* node;
	node = self->entries[hash];
	while ( node )
	{
		if ( 0 == CharString_compare( key, Node_getKey( node ) ) ) {
			contains = 1;
			break;
		} else {
			node = Node_getSibling( node );
		}
	}
	return contains;
}
		
bool
Hashtable_containsValue( const IHashtable* self, const void* value )
{
	bool contains = 0;
	const INode* node;
	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		node = self->entries[i];
		while ( node )
		{
			if ( value == Node_getObject( node ) ) {
				contains = 1;
				i = HTABLE_SIZE;
				break;
			} else {
				node = Node_getSibling( node );
			}
		}
	}
	return contains;
}

int
Hashtable_getHashValue( const char* key )
{
	unsigned int max = CharString_getLength( key );
	unsigned int i;
	unsigned int hash = 0;
	for ( i=0; i < max; i++ )
	{
		hash = (hash + key[i]) % HTABLE_SIZE;
	}
	return hash;
}

unsigned int
Hashtable_getSize( const IHashtable* self )
{
	return self->size;
}

IIterator*
Hashtable_iterator_keys( const IHashtable* self )
{
	IIterator* it = new_Iterator();

	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* node = self->entries[i];
		while ( node )
		{
			Iterator_addReference( it, Node_getKey( node ) );
			node = Node_getSibling( node );
		}
	}
	
	return it;
}

IIterator*
Hashtable_iterator_values( const IHashtable* self )
{
	IIterator* it = new_Iterator();

	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* node = self->entries[i];
		while ( node )
		{
			Iterator_addReference( it, Node_getObject( node ) );
			node = Node_getSibling( node );
		}
	}
	
	return it;
}

IIterator*
Hashtable_iterator_alphabetical_keys( const IHashtable* self )
{
	IIterator* it;
	ISort* sort = new_Sort();

	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* node = self->entries[i];
		while ( node )
		{
			Sort_insert( sort, Node_getKey( node ), Node_getObject( node ) );
			node = Node_getSibling( node );
		}
	}
	
	it = Sort_iterator_keys( sort );
	free_Sort( sort );
	
	return it;
}

IIterator*
Hashtable_iterator_alphabetical_values( const IHashtable* self )
{
	IIterator* it = new_Iterator();

	int i;
	for ( i=0; i < HTABLE_SIZE; i++ )
	{
		const INode* node = self->entries[i];
		while ( node )
		{
			Iterator_addReference( it, Node_getObject( node ) );
			node = Node_getSibling( node );
		}
	}
	
	return it;
}
