/*
 *  Copyright (C) 2004 Daniel Robert Bradley. All rights reserved.
 *
 *  This software is redistributable under the terms of the GNU LGPL
 *  (Lesser General Public License).
 *
 *  I.S.Labs is a registered trademark of Daniel Robert Bradley
 */

#ifndef OPENOCL_BASE_PSA_SYSTEM_H
#define OPENOCL_BASE_PSA_SYSTEM_H

#include "openocl/base/psa/export.h"
#include "openocl/base/psa/Environment.h"
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

EXPORT void System_Initialize( int argc, char** argv );

/*
 *  System allocating functions
 */
EXPORT	char* System_CurrentDirectory();
EXPORT	char* System_CurrentVolume();
EXPORT	char* System_Hostname();

EXPORT	IEnvironment* System_GetInitialEnvironment();

#ifdef __cplusplus
}
};};};
#endif
#endif
