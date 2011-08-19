/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_CRUNTIME_H
#define OPENOCL_BASE_PSA_CRUNTIME_H

#include "openocl/base/psa/export.h"
#include <stdlib.h>
//#include <sys/types.h>

	/*	These allow programs to include CRuntime for testing but reimplement their own
	 *  stubs for normal compilation.
	 */
	#ifndef OPENOCL_BASE_PSA_CRUNTIME_MALLOC
	#define OPENOCL_BASE_PSA_CRUNTIME_MALLOC
	#endif

	#ifndef OPENOCL_BASE_PSA_CRUNTIME_CALLOC
	#define OPENOCL_BASE_PSA_CRUNTIME_CALLOC
	#endif

	#ifndef OPENOCL_BASE_PSA_CRUNTIME_FREE
	#define OPENOCL_BASE_PSA_CRUNTIME_FREE
	#endif



	#ifdef __cplusplus
namespace openocl {
	namespace base {
		namespace psa {
extern "C"
{
	#else
		#ifndef BOOLEAN
		#define BOOLEAN
	typedef int bool;
		#endif
	#endif
	
/*
 *	Conditionally defined in "openocl/base/psa/debug.h"
 */
	#ifndef OPENOCL_BASE_PSA_DIRECT_MEMORY_ALLOCATION

EXPORT	void*	CRuntime_malloc( size_t size );
EXPORT	void*	CRuntime_calloc( size_t nmemb, size_t size );
EXPORT	void*	CRuntime_realloc( void* ptr, size_t size );
EXPORT	void*	CRuntime_recalloc( void* ptr, size_t old, size_t size );
EXPORT	void*	CRuntime_recalloc_type( void* ptr, size_t old, size_t size, size_t type );
EXPORT	void	CRuntime_free( void* ptr );

EXPORT	void	CRuntime_AlterAllocationCount( int delta );
EXPORT	long long CRuntime_GetAllocationCount();
EXPORT	void	CRuntime_SleepFor( unsigned int seconds );
EXPORT	void	CRuntime_MicroSleepFor( unsigned int microseconds );


	#endif

	#ifdef __cplusplus
}
};};};
	#endif
#endif

