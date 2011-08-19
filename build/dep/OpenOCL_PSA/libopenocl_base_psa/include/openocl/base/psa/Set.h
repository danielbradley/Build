/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_SET_H
#define OPENOCL_BASE_PSA_SET_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _ISet ISet;

EXPORT	ISet*		new_Set();
EXPORT	void		free_Set( ISet* self );

EXPORT	void		Set_add( ISet* self, const void* anElement );
EXPORT	const void*	Set_get( const ISet* self, unsigned int index );

EXPORT	int		Set_contains( const ISet* self, const void* anElement ); 
EXPORT	unsigned int	Set_count( const ISet* self );
EXPORT	void		Set_union( ISet* self, const ISet* aSet );

#ifdef __cplusplus
}
};};};
#endif
#endif

