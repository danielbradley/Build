/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_BUILDMANAGER_H
#define ISLABS_BUILD_BUILDMANAGER_H

#include "islabs/build.h"
#include "islabs/build/BuildParameters.h"
#include "islabs/build/Global.h"
#include <openocl/base/psa/Index.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

BuildManager* new_BuildManager( const char* topDirectory );
void free_BuildManager( BuildManager* self );

int     BuildManager_refresh( BuildManager* self );
int     BuildManager_buildTo( BuildManager* self, const BuildParameters* parameters, const char* target, int depth );

IIndex* BuildManager_getBuildablesIndex( BuildManager* self );
IIndex* BuildManager_getSuitesIndex( BuildManager* self );

int  BuildManager_buildTestsTo( const BuildManager* self, const BuildParameters* parameters, const char* target );
int  BuildManager_runTestsFrom( const BuildManager* self, const BuildParameters* parameters, const char* target );

void BuildManager_printTree( const BuildManager* self );

const char* BuildManager_GetVersion();

#ifdef __cplusplus
};};};
#endif

#endif
