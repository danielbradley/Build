/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_INDEX_H
#define OPENOCL_BASE_PSA_INDEX_H

#include "openocl/base/psa/Iterator.h"
#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _IIndex IIndex;

EXPORT	IIndex*      new_Index();
EXPORT	IIndex*      new_Index_hashtable();
EXPORT	void         free_Index( IIndex* self );

//  functions
EXPORT	void         Index_freeObjects( IIndex* self, void(*freeObject)(void*) );
EXPORT	void*        Index_add( IIndex* self, const char* key, const void* object );
EXPORT	bool         Index_addIfNew( IIndex* self, const char* key, const void* object );
EXPORT	void*        Index_remove_key( IIndex* self, const char* key );
EXPORT	void*        Index_remove_value( IIndex* self, const void* object );

//  constant functions
EXPORT	const void*  Index_get( const IIndex* self, const char* key );
EXPORT	bool         Index_containsKey( const IIndex* self, const char* key );
EXPORT	bool         Index_containsValue( const IIndex* self, const void* value );
EXPORT	unsigned int Index_getSize( const IIndex* self );

EXPORT	IIterator*   Index_iterator_keys( const IIndex* self );
EXPORT	IIterator*   Index_iterator_values( const IIndex* self );
EXPORT	IIterator*   Index_iterator_alphabetical_keys( const IIndex* self );
EXPORT	IIterator*   Index_iterator_alphabetical_values( const IIndex* self );

#ifdef __cplusplus
}
};};};
#endif

#endif

