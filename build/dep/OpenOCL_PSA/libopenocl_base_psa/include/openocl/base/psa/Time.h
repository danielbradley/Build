/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_TIME_H
#define OPENOCL_BASE_PSA_TIME_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _ITime ITime;

/*
 *  Lifecycle functions
 */
EXPORT	ITime*      new_Time( unsigned long long );
EXPORT	void        free_Time( ITime* self );

EXPORT	ITime*      Time_copy( const ITime* self );
EXPORT	const char* Time_getTextual( const ITime* self );
EXPORT  unsigned long long Time_getSecondsSinceEpoch( const ITime* self );

EXPORT	ITime* Time_Now();

#ifdef __cplusplus
}
};};};
#endif
#endif
