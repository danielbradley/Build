/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_MAP_INTERNAL_H
#define OPENOCL_BASE_PSA_MAP_INTERNAL_H

#include "openocl/base/psa/Iterator.h"
#include "openocl/base/psa.h"

typedef struct _IMap IMap;

void         free_Map( IMap* self );

//  functions
void         Map_freeObjects( IMap* self, void(*freeObject)(void*) );
void*        Map_put( IMap* self, const char* key, const void* object );
bool         Map_putIfNew( IMap* self, const char* key, const void* object );
void*        Map_remove_key( IMap* self, const char* key );
void*        Map_remove_value( IMap* self, const void* value );

//  constant functions
const void*  Map_get( const IMap* self, const char* key );
bool         Map_containsKey( const IMap* self, const char* key );
bool         Map_containsValue( const IMap* self, const void* value );
unsigned int Map_getSize( const IMap* self );

IIterator*   Map_iterator_keys( const IMap* self );
IIterator*   Map_iterator_values( const IMap* self );
IIterator*   Map_iterator_alphabetical_keys( const IMap* self );
IIterator*   Map_iterator_alphabetical_values( const IMap* self );

#endif
