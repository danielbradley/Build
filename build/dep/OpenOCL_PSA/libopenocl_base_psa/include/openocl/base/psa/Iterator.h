/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_ITERATOR_H
#define OPENOCL_BASE_PSA_ITERATOR_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _IIterator IIterator;

EXPORT	IIterator*	new_Iterator();
EXPORT	void		free_Iterator( IIterator* self );

EXPORT	void		Iterator_addReference( IIterator* self, const void* element );
EXPORT	const void*	Iterator_next( IIterator* self );
EXPORT	bool		Iterator_hasNext( const IIterator* self );

#ifdef __cplusplus
}
};};};
#endif
#endif

