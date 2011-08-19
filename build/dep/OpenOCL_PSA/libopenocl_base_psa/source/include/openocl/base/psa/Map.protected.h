/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_MAP_PROTECTED_H
#define OPENOCL_BASE_PSA_MAP_PROTECTED_H

#include "openocl/base/psa/Map.internal.h"

struct _IMap
{
	//  virtual functions
	void(*freeObjects)( IMap*, void(*freeObject)(void*) );
	void(*free)( IMap* );
	void*(*put)( IMap*, const char* key, const void* object );
	bool(*putIfNew)( IMap*, const char* key, const void* object );
	void*(*removeKey)( IMap*, const char* key );
	void*(*removeValue)( IMap*, const void* value );
	
	//  virtual const functions
	const void*(*get)( const IMap*, const char* key );
	bool(*containsKey)( const IMap*, const char* key );
	bool(*containsValue)( const IMap*, const void* value );
	unsigned int(*getSize)( const IMap* );
	
	//  virtual allocating constant functions
	IIterator*(*iterator_keys)( const IMap* );
	IIterator*(*iterator_values)( const IMap* );
	IIterator*(*iterator_alphabetical_keys)( const IMap* );
	IIterator*(*iterator_alphabetical_values)( const IMap* );
	
};

#endif
