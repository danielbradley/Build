/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_SEQUENCE_H
#define OPENOCL_BASE_PSA_SEQUENCE_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _ISequence ISequence;

EXPORT	ISequence*	new_Sequence();
EXPORT	void		free_Sequence( ISequence* self );

EXPORT	void		Sequence_add( ISequence*const self, const void* anObject );
EXPORT	void		Sequence_insert( ISequence*const self, const void* anObject, unsigned int index );
EXPORT	const void*	Sequence_remove_element( ISequence*const self, const void* anElement );
EXPORT	const void*	Sequence_remove_from_index( ISequence*const self, unsigned int index );
EXPORT	const void*	Sequence_get( const ISequence* self, unsigned int index );

EXPORT	int		Sequence_contains( const ISequence* self, const void* anObject ); 
EXPORT	unsigned int	Sequence_count( const ISequence* self );

#ifdef __cplusplus
}
};};};
#endif
#endif

