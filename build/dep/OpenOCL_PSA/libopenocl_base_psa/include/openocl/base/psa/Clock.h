/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_CLOCK_H
#define OPENOCL_BASE_PSA_CLOCK_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
#endif

typedef struct _Clock IClock;

/*
 *  Lifecycle functions
 */
EXPORT	IClock* new_Clock();
EXPORT	void free_Clock( IClock* self );

/*
 *  Functions
 */
EXPORT	void               Clock_start( IClock* self );
EXPORT	void               Clock_stop( IClock* self );
EXPORT	unsigned long long Clock_getInterval( const IClock* self );

/*
 *  Class Methods
 */
EXPORT	int           Clock_GetTimeAsInt();
EXPORT	unsigned long Clock_GetUserTime();
EXPORT	unsigned long Clock_GetSystemTime();
EXPORT	unsigned long Clock_GetUptime();

EXPORT	unsigned long long Clock_GetMicroSecondsSinceEpoch();
EXPORT	unsigned long long Clock_GetSecondsSinceEpoch();

EXPORT	unsigned long Clock_GetClockCyclesUsed(); 

#ifdef __cplusplus
}
};};};
#endif
#endif
