/*
 *  Copyright (C) 2004-2005 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_BAG_H
#define OPENOCL_BASE_PSA_BAG_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _IBag IBag;

EXPORT	IBag*		new_Bag();
EXPORT	void		free_Bag( IBag* self );

EXPORT	void		Bag_add( IBag*const self, const void* anObject );
EXPORT	const void*	Bag_get( const IBag* self, unsigned int index );

EXPORT	int		Bag_contains( const IBag* self, const void* anObject ); 
EXPORT	unsigned int	Bag_count( const IBag* self );

#ifdef __cplusplus
}
};};};
#endif
#endif

