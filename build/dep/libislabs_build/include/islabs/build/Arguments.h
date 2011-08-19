/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef _ISLABS_BUILD_ARGUMENTS_H_
#define _ISLABS_BUILD_ARGUMENTS_H_

#include "islabs/build.h"
#include <openocl/base/psa/List.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

Arguments*
Arguments_new( unsigned int argc, char** argv );

void
Arguments_delete( Arguments* self );

void
Arguments_discover( unsigned int argc, char** argv );

const IList*
Arguments_getDefines( const Arguments* self );

const char*
Arguments_getCommand( const Arguments* self );

const char*
Arguments_getPackageDir( const Arguments* self );

const char*
Arguments_getTargetDir( const Arguments* self );

unsigned int
Arguments_getVerbosity( const Arguments* self );

int
Arguments_getDepth( const Arguments* self );

int
Arguments_isFull( const Arguments* self );

int
Arguments_isTree( const Arguments* self );

int
Arguments_isRecursive( const Arguments* self );

int
Arguments_isDeep( const Arguments* self );

int
Arguments_isAll( const Arguments* self );

int
Arguments_isLog( const Arguments* self );

int
Arguments_isNew( const Arguments* self );

int
Arguments_isHelp( const Arguments* self );

int
Arguments_isInvalid( const Arguments* self );

int
Arguments_isNear( const Arguments* self );

int
Arguments_isNo( const Arguments* self );

int
Arguments_isForce( const Arguments* self );

int
Arguments_isLegacy( const Arguments* self );

int
Arguments_isCopyLibs( const Arguments* self );

int
Arguments_isBuildStatic( const Arguments* self );

int
Arguments_isDebug( const Arguments* self );

int
Arguments_isRunTests( const Arguments* self );

int
Arguments_isDebugLibc( const Arguments* self );

int
Arguments_isRelease( const Arguments* self );

int
Arguments_isDynamic( const Arguments* self );

int
Arguments_isVersion( const Arguments* self );

int
Arguments_isMonolithic( const Arguments* self );

int
Arguments_isBuildTests( const Arguments* self );

int
Arguments_isRunTestsOnly( const Arguments* self );

int
Arguments_isWithProfiling( const Arguments* self );

int
Arguments_isBuildTestsOnly( const Arguments* self );

#ifdef __cplusplus
};};};
#endif

#endif


