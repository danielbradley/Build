/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef ISLABS_BUILD_SONAME_H
#define ISLABS_BUILD_SONAME_H

#include "islabs/build.h"

#ifdef __cplusplus
using namespace openocl::base::psa;

	namespace islabs {
		namespace build {
	extern "C" {
#endif

struct _SoName
{
	char* long_name;
	char* short_name;
};

#ifdef __cplusplus
};};};
#endif

#endif
