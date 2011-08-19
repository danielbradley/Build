/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_PROCESS_H
#define OPENOCL_BASE_PSA_PROCESS_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa.h"

	#ifdef __cplusplus
		namespace openocl {
			namespace base {
				namespace psa {

		extern "C"
		{
	#endif

	typedef struct _IProcess IProcess;

	/*
	 *  Lifecycle functions
	 */
EXPORT		IProcess* new_Process( const char* executable, const char** arguments );
EXPORT		void free_Process( IProcess* self );

	/*
	 *  Functions
	 */
EXPORT		int Process_start( IProcess* self );
EXPORT		int Process_waitFor( IProcess* self );

	/*
	 *  Mutators
	 */
EXPORT		void Process_setSearchPath( IProcess* self, const char* aPath );
EXPORT		void Process_setUsePath( IProcess* self, int aBoolean );
EXPORT		void Process_setStandardIn( IProcess* self, const char* location );
EXPORT		void Process_setStandardOut( IProcess* self, const char* location );
EXPORT		void Process_setStandardError( IProcess* self, const char* location );

	/*
	 *  Accessors
	 */
EXPORT		const char*  Process_getExecutable( const IProcess* self );
EXPORT		const char** Process_getArguments( const IProcess* self );
EXPORT		unsigned int Process_getProcessID( const IProcess* self );
EXPORT		const char*  Process_getSearchPath( const IProcess* self );
EXPORT		int	     Process_getExitStatus( const IProcess* self );

EXPORT		bool         Process_hasFinished( const IProcess* self );
EXPORT		bool         Process_hasExitedNormally( const IProcess* self );

	/*
	 *  Class methods
	 */
EXPORT		unsigned int Process_GetCurrentID();
EXPORT		unsigned int Process_GetCurrentParentID();

	#ifdef __cplusplus
		}
		};};};
	#endif
#endif
