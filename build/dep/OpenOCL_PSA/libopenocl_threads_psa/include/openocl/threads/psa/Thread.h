/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_THREADS_PSA_THREAD_H
#define OPENOCL_THREADS_PSA_THREAD_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

#ifdef __cplusplus
	namespace openocl {
		namespace threads {
			namespace psa {
	extern "C" {
#endif

typedef struct _IThread IThread;

EXPORT	IThread* new_Thread( void*(*callback)(void*), void* aRunnable );
EXPORT	void free_Thread( IThread* self );

EXPORT	bool Thread_run( IThread* self );
EXPORT	bool Thread_wait( IThread* self );

//EXPORT  int  Thread_status( const IThread* self );

#ifdef __cplusplus
}
};};};
#endif

#endif
