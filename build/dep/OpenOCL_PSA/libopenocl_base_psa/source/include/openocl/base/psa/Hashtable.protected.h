/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_HASHTABLE_PROTECTED_H
#define OPENOCL_BASE_PSA_HASHTABLE_PROTECTED_H

#include "openocl/base/psa/Hashtable.internal.h"
#include "openocl/base/psa/Iterator.h"
#include "openocl/base/psa/Map.protected.h"

#define HTABLE_SIZE 256

void Hashtable_init( IHashtable* self );
void Hashtable_final( IHashtable* self );

int Hashtable_getHashValue( const char* key );

struct _IHashtable
{
	IMap super;
	
	//  virtual functions
	void(*free)( IHashtable* );
	void(*freeObjects)( IHashtable*, void(*freeObject)(void*) );
	void*(*put)( IHashtable*, const char* key, const void* object );
	bool(*putIfNew)( IHashtable*, const char* key, const void* object );
	void*(*removeKey)( IHashtable*, const char* key );
	void*(*removeValue)( IHashtable*, const void* value );

	//  virtual const functions
	const void*(*get)( const IHashtable*, const char* key );
	bool(*containsKey)( const IHashtable*, const char* key );
	bool(*containsValue)( const IHashtable*, const void* value );
	unsigned int(*getSize)( const IHashtable* );

	IIterator*(*iterator_keys)( const IHashtable* );
	IIterator*(*iterator_values)( const IHashtable* );
	IIterator*(*iterator_alphabetical_keys)( const IHashtable* );
	IIterator*(*iterator_alphabetical_values)( const IHashtable* );

	//  private members
	void** entries;
	unsigned int size;
};

#endif
