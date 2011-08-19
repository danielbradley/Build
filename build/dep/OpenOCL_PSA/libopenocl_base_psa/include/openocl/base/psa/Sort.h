/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_SORT_H
#define OPENOCL_BASE_PSA_SORT_H

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

typedef struct _ISort ISort;

EXPORT	ISort*		new_Sort();
EXPORT	void		free_Sort( ISort* self );

EXPORT	void		Sort_insert( ISort* self, const char* aKey, const void* aValue );

EXPORT	unsigned int	Sort_count( const ISort* self );
EXPORT	IIterator*	Sort_iterator_keys( const ISort* self );
EXPORT	IIterator*	Sort_iterator_values( const ISort* self );

#ifdef __cplusplus
}
};};};
#endif
#endif

