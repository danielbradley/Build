/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_HASHTABLE_INTERNAL_H
#define OPENOCL_BASE_PSA_HASHTABLE_INTERNAL_H

#include "openocl/base/psa/Iterator.h"

typedef struct _IHashtable IHashtable;

EXPORT	IHashtable*  new_Hashtable();
EXPORT	void         free_Hashtable( IHashtable* self );

//  non-const functions
EXPORT	void         Hashtable_freeObjects( IHashtable* self, void(*freeObject)(void*) );
EXPORT	void*        Hashtable_put( IHashtable* self, const char* key, const void* object );
EXPORT	bool         Hashtable_putIfNew( IHashtable* self, const char* key, const void* object );
EXPORT	void*        Hashtable_remove_key( IHashtable* self, const char* key );
EXPORT	void*        Hashtable_remove_value( IHashtable* self, const void* value );

//  const functions
EXPORT	const void*  Hashtable_get( const IHashtable* self, const char* key );
EXPORT	bool         Hashtable_containsKey( const IHashtable* self, const char* key );
EXPORT	bool         Hashtable_containsValue( const IHashtable* self, const void* value );
EXPORT	unsigned int Hashtable_getSize( const IHashtable* self );

IIterator*   Hashtable_iterator_keys( const IHashtable* self );
IIterator*   Hashtable_iterator_values( const IHashtable* self );
IIterator*   Hashtable_iterator_alphabetical_keys( const IHashtable* self );
IIterator*   Hashtable_iterator_alphabetical_values( const IHashtable* self );

#endif
