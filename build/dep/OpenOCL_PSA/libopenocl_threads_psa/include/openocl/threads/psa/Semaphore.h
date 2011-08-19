/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_THREADS_PSA_SEMAPHORE_H
#define OPENOCL_THREADS_PSA_SEMAPHORE_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
	namespace openocl {
		namespace threads {
			namespace psa {
	extern "C" {
#endif

typedef struct _ISemaphore ISemaphore;

EXPORT	ISemaphore* new_Semaphore();
EXPORT	ISemaphore* new_Semaphore_value( int start );
EXPORT	void free_Semaphore( ISemaphore* self );

EXPORT	void Semaphore_wait( ISemaphore* self );
EXPORT	void Semaphore_signal( ISemaphore* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
