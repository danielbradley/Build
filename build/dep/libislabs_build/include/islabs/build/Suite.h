/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_SUITE_H
#define ISLABS_BUILD_SUITE_H

#include "islabs/build.h"
#include "islabs/build/Buildable.h"
#include "islabs/build/BuildParameters.h"
#include <openocl/io/psa/Directory.h>

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

// public functions
Suite* new_Suite( const IDirectory* dir );
void   free_Suite( Suite* self );
bool   Suite_buildTo( const Suite* self, const IDirectory* target, const BuildParameters* params, int depth );

#ifdef __cplusplus
};};};
#endif

#endif
