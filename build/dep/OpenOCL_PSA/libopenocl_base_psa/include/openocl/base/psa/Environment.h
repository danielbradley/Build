/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_ENVIRONMENT_H
#define OPENOCL_BASE_PSA_ENVIRONMENT_H

#include "openocl/base/psa/export.h"
#include <stdarg.h>

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

typedef struct _IEnvironment IEnvironment;

EXPORT	IEnvironment* new_Environment( const char* argv_0 );
EXPORT	void          free_Environment( IEnvironment* self );

EXPORT	const char* Environment_getExecutableName( const IEnvironment* self );
EXPORT	const char* Environment_getExecutableLocation( const IEnvironment* self );
EXPORT	const char* Environment_getDirectoryContainingExecutable( const IEnvironment* self );

EXPORT	const char* Environment_getPath( const IEnvironment* self );
//EXPORT	const char* Environment_getOrigin( const IEnvironment* self );

EXPORT	char*		Environment_DirectoryContainingExecutable( const char* executable, const char* PATH );
EXPORT  char*       Environment_SearchPathFor( const char* PATH, const char* file );
EXPORT	char*		Environment_FollowLinks( const char* location, const char* directory );

EXPORT	void        Environment_SetEnvironmentValue( const char* key, const char* value, int overwrite );
EXPORT	const char* Environment_GetEnvironmentValue( const char* variable );
EXPORT  char        Environment_GetFileSeparator();
EXPORT  char        Environment_GetPathSeparator();

#ifdef __cplusplus
}
};};};
#endif
#endif
