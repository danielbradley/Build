/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_THREADS_PSA_CONDITIONVARIABLE_H
#define OPENOCL_THREADS_PSA_CONDITIONVARIABLE_H

#include "openocl/base/psa.h"
#include "openocl/threads/psa/Mutex.h"

#ifdef __cplusplus
	namespace openocl {
		namespace threads {
			namespace psa {
	extern "C" {
#endif

typedef struct _IConditionVariable IConditionVariable;

EXPORT	IConditionVariable* new_ConditionVariable();
EXPORT	void free_ConditionVariable( IConditionVariable* self );

EXPORT	bool ConditionVariable_wait( IConditionVariable* self, IMutex* aMutex );
EXPORT	bool ConditionVariable_timedWait( IConditionVariable* self, IMutex* aMutex, int seconds );
EXPORT	bool ConditionVariable_signal( IConditionVariable* self );
EXPORT	bool ConditionVariable_broadcast( IConditionVariable* self );

//EXPORT	IConditionVariable* ConditionVariable_copy( IConditionVariable* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
