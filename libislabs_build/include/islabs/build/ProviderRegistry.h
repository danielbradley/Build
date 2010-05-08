/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_PROVIDERREGISTRY_H
#define ISLABS_BUILD_PROVIDERREGISTRY_H

#include "islabs/build.h"
#include "islabs/build/providers/ToolSet.h"
#include "islabs/build/providers/ToolSetProvider.h"

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

ProviderRegistry* new_ProviderRegistry( const char* directory );
void free_ProviderRegistry( ProviderRegistry* self );

ToolSet*
ProviderRegistry_toolSet( const ProviderRegistry* self, const char* platform, const char* compiler );

#ifdef __cplusplus
};};};
#endif

#endif
