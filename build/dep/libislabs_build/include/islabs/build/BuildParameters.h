/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_BUILDPARAMETERS_H
#define ISLABS_BUILD_BUILDPARAMETERS_H

#include "islabs/build.h"
#include "islabs/build/Global.h"

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

BuildParameters* new_BuildParameters( const Global* global );
void free_BuildParameters( BuildParameters* self );

void BuildParameters_setAll( BuildParameters* self, bool all );
void BuildParameters_setBuildPackages( BuildParameters* self, bool buildPackages );
void BuildParameters_setBuildTests( BuildParameters* self, bool buildTests );
void BuildParameters_setDeep( BuildParameters* self, bool deep );
void BuildParameters_setForce( BuildParameters* self, bool force );
void BuildParameters_setFull( BuildParameters* self, bool force );
void BuildParameters_setLog( BuildParameters* self, bool log );
void BuildParameters_setMonolithic( BuildParameters* self, bool monolithic );
void BuildParameters_setNear( BuildParameters* self, bool near );
void BuildParameters_setNo( BuildParameters* self, bool no );
void BuildParameters_setRecursive( BuildParameters* self, bool recursive );
void BuildParameters_setRelease( BuildParameters* self, bool release );
void BuildParameters_setRunTests( BuildParameters* self, bool runTests );
void BuildParameters_setRunTestsOnly( BuildParameters* self, bool runTestsOnly );

const Global* BuildParameters_getGlobal( const BuildParameters* self );
const char*   BuildParameters_getOSType( const BuildParameters* self );
const char*   BuildParameters_getPlatform( const BuildParameters* self );
const char**  BuildParameters_getPlatformSubparts( const BuildParameters* self );

bool BuildParameters_isAll( const BuildParameters* self );
bool BuildParameters_isBuildPackages( const BuildParameters* self );
bool BuildParameters_isBuildTests( const BuildParameters* self );
bool BuildParameters_isCopyLibs( const BuildParameters* self );
bool BuildParameters_isDeep( const BuildParameters* self );
bool BuildParameters_isForce( const BuildParameters* self );
bool BuildParameters_isFull( const BuildParameters* self );
bool BuildParameters_isLog( const BuildParameters* self );
bool BuildParameters_isMonolithic( const BuildParameters* self );
bool BuildParameters_isNear( const BuildParameters* self );
bool BuildParameters_isNo( const BuildParameters* self );
bool BuildParameters_isRecursive( const BuildParameters* self );
bool BuildParameters_isRelease( const BuildParameters* self );
bool BuildParameters_isRunTests( const BuildParameters* self );
bool BuildParameters_isRunTestsOnly( const BuildParameters* self );
bool BuildParameters_isWithProfiling( const BuildParameters* self );

unsigned int BuildParameters_getVerbosity( const BuildParameters* self );

void BuildParameters_print( const BuildParameters* self );

#ifdef __cplusplus
};};};
#endif

#endif
