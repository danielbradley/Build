/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_THREADS_PSA_MUTEX_H
#define OPENOCL_THREADS_PSA_MUTEX_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
	namespace openocl {
		namespace threads {
			namespace psa {
	extern "C" {
#endif

typedef struct _IMutex IMutex;

EXPORT	IMutex* new_Mutex();
EXPORT	void free_Mutex( IMutex* self );

EXPORT	bool Mutex_lock( IMutex* self );
EXPORT	bool Mutex_tryLock( IMutex* self );
EXPORT	bool Mutex_unlock( IMutex* self );

EXPORT	bool Mutex_isValid( IMutex* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
